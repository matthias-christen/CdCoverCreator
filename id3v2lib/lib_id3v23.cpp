/* the id3v2.3 library.
 * (c)2002 by Samuel Abels (sam@manicsadness.com)
 * This project's homepage is: http://software.manicsadness.com/cantus
 *
 * This library is designed for easyest possible access to id3 V2 tags.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "stdafx.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "lib_id3v23.h"
#include "charset.h"


/***************************************************************************************
 * BELOW FOLLOW THE STATICS
 ***************************************************************************************/
// Return the last item of an doubly linked list
static DLL *
dll_last (DLL *list)
{
	if( list == NULL )
		return (NULL);
	
	while ( list->next != NULL )
		list = (DLL*) list->next;
	
	return (list);
}

// Append an item to the doubly linked list
static DLL *
dll_append (DLL *list, void *data)
{
	DLL *item = (DLL*) malloc (sizeof(DLL));
	DLL *lastitem = dll_last(list);
	
	item->data = data;
	item->next = NULL;
	
	if ( lastitem == NULL )
	{
		item->prev = NULL;
		return (item);
	}
	else
	{
		item->prev = lastitem;
		lastitem->next = item;
	}

	return list;
}
  
static DLL *
dll_remove (DLL *list, void *data)
{
	DLL *item = list;
	
	while ( item )
	{
		if ( item->data == data )
		{
			if ( item->prev == NULL 
				&& item->next == NULL )
			{
// No other items there? Then return a zero pointer.
				free (item);
				return (NULL);
			}
			if ( item->prev == NULL )
			{
// remove the first item of the list here...
				list = (DLL*) item->next;
				list->prev = NULL;
				free (item);
				
				break;
			}
			if ( item->next == NULL )
			{
// ...remove the last item of the list here...
				((DLL*)(item->prev))->next = NULL;
				free (item);
				
				break;
			}
// ...or other items here
			((DLL*)(item->prev))->next = item->next;
			((DLL*)(item->next))->prev = item->prev;
			free (item);
			
			break;
		}
		item = (DLL*) item->next;
	}
	
	return list;
}

// Free a doubly linked list
static DLL *
dll_free (DLL *list)
{
	DLL *item = list;
	DLL *current = NULL;
	
	while (item)
	{
		current = item;
		item = (DLL*) item->next;
		
		free (current);
	}
	
	return NULL;
}

/*
 * Converts all occurences of a CR/LF to LF
 */
static void
crlf2cr (char *source)
{
	char *psource = source;
	char destination[2048];

	if(source != NULL)
	{
		memset (destination, 0, 2048);
		for (psource = source; *psource != '\0'; psource++)
		{
			if(*psource == 13
				&& *(psource+1) == 10 )
			{
				psource++;
			}
			destination[strlen(destination)] = *psource;
		}
	}
	
	strncpy (source, destination, strlen(destination)+1);
}


/*
 * Converts all occurences of a LF to CR/LF
 */
static void
cr2crlf (char *source)
{
	char *psource = source;
	char destination[2048];
	
	if( source != NULL )
	{
		memset (destination, 0, 2048);
		for (psource = source; *psource != '\0'; psource++)
		{
			if (*psource == 10)
				destination[strlen(destination)] = 13;
			destination[strlen(destination)] = *psource;
		}
	}
	
	strncpy (source, destination, strlen(destination)+1);
}




/*
 * Reads the first ten bytes of an file and checks, if it's a valid ID3 V2.3 file
 * If it is, the header flags are stored in the tag struct.
 * Returns TRUE on a valid header, otherwise FALSE.
 */
static short int
check_header (FILE *mp3file, id3v2Tag *v2)
{
	unsigned char buf[10];
	
// get header (=first ten bytes of the file)
	fseek (mp3file, 0, SEEK_SET);
	if ( fread (buf, 1, 10, mp3file) < 10 )
		return (FALSE);

// a valid tag must begin with "ID3" followed by the version (checked below)
// followed by a flag byte, where the last fife bytes are unused and must be FALSE
	if ( memcmp(buf, "ID3", 3) != 0
		|| (buf[5] & 31) != 0 )
		return (FALSE);

// check if version is supported
	if ( buf[3] != 3
		|| buf[4] != 0 )
		return (FALSE);

// The next thing to come is the tag size. This are 4 bytes, the MSB should always be set to zero. check!
	if ( (buf[6] & 128) != 0
		|| (buf[7] & 128) != 0
		|| (buf[8] & 128) != 0
		|| (buf[9] & 128) != 0 )
		return (FALSE);
	
// The tag size is encoded to be syncsave, so I got to decode it.
// The tag size is the size of the complete tag EXCLUDING the 10-byte header.
 	v2->tag_size = buf[9] + (buf[8] << 7) + (buf[7] << 14) + (buf[6] << 21);

// ok, so were save. put the flags in the nicer struct.
	v2->unsync = (buf[5] & 128) >> 7;
	v2->has_extheader = (buf[5] & 64) >> 6;
	v2->is_experimental = (buf[5] & 32) >> 5;
	
	return (TRUE);
}





/*
 * Reads the extheader of a valid ID3V2.3 file and checks, if it's a valid.
 * If it is, the extheader flags are stored in the tag struct.
 * Returns TRUE on a valid extheader, otherwise FALSE.
 */
static short int
check_extheader (FILE *mp3file, id3v2Tag *v2)
{
	unsigned char buf[10];

// Read id3 extheader intro (5 bytes)
	fseek (mp3file, 10, SEEK_SET);
	if ( fread(buf, 1, 5, mp3file) < 5 )
		return (FALSE);
	
// First comes the extheader size. This are 4 bytes, the MSB should always be set to zero. check!
	if( (buf[0] & 128) != 0
		|| (buf[1] & 128) != 0
		|| (buf[2] & 128) != 0
		|| (buf[3] & 128) != 0 )
		return (FALSE);
	
// OK. In ID3V2.3 only six byte or ten byte extheaders are allowed.
	if( v2->extheader_size != 6 
		&& v2->extheader_size != 10 )
		return (FALSE);
	
// The first four bytes specify the extheader size.
	v2->extheader_size = buf[3] + (buf[2] << 7) + (buf[1] << 14) + (buf[0] << 21);
	
// The fifth byte specifies extendened flags. (in fact, only one flag is used for ID3V2.3
// The MSB of the byte 5 specifies, if there is CRC data to come, appended to the extheader.
	if( (buf[4] & 127) != 0
		|| buf[5] != 0 )
		return (FALSE);
	
	v2->crc_data_present = (buf[4] & 128) >> 7;
	
// if crc data is present, the extheader size must be ten bytes, otherwise 6.
	if ( (v2->extheader_size == 6 && v2->crc_data_present == TRUE)
		|| (v2->extheader_size == 10 && v2->crc_data_present == FALSE) )
		return (FALSE);
	
// now come four bytes specifying the padding size
	if ( (buf[6] & 128) != 0
		|| (buf[7] & 128) != 0
		|| (buf[8] & 128) != 0
		|| (buf[9] & 128) != 0 )
		return (FALSE);
	
	v2->padding_size = buf[9] + (buf[8] << 7) + (buf[7] << 14) + (buf[6] << 21);
	
// Now to the optional crc data.
	if( v2->crc_data_present )
	{
		if( fread (buf, 1, 4, mp3file) < 4 )
			return (FALSE);
		
		memcpy (v2->crc_data, buf, 4);
	}
	
	return (TRUE);
}





/*
 * Reads the complete frames of a valid ID3V2.3 file and checks, if they are valid.
 * If they are, the flags are stored in a DLL and appended to the tag struct.
 * Returns TRUE on success, otherwise FALSE.
 */
static short int
read_frames (FILE *mp3file, id3v2Tag *v2)
{
	unsigned char buf[10];
	int numframes = 0;
	unsigned int totalframesize = 0;
	id3v2Frame *frame = NULL;
	
// set the position to the first frame header (header = 10 bytes + extheadersize + 4 bytes "extheaderheader")
	if (v2->has_extheader)
		fseek (mp3file, 10 + v2->extheader_size + 4, SEEK_SET);
	else
		fseek (mp3file, 10, SEEK_SET);

// If the tag size is too small for frames, return with an error.
	if ( ((v2->tag_size + 10) - v2->padding_size) <= ftell(mp3file) )
		return FALSE;
	
// now read all the frames
	numframes = 0;
	v2->frames = NULL;
	while ( ftell (mp3file) < ((v2->tag_size + 10) - v2->padding_size) )
	{
		frame = (id3v2Frame*) calloc (1, sizeof(id3v2Frame));

// the frame header is ten bytes long
		if ( fread (buf, 1, 10, mp3file) < 10 )
			goto error;

// if we are already in the padding, we must no longer look for frames...
		if ( buf[0] == 0 && buf[1] == 0 && buf[2] ==  0 && buf[3] == 0 )
		{
			if (numframes == 0)
				goto error;
			
			free (frame);
			break;
		}
		
// first come four characters identifying the frame. It must be alphanumeric.
		if ( !isalnum(buf[0]) || !isalnum(buf[1]) || !isalnum(buf[2]) || !isalnum(buf[3]) )
			goto error;
		
		*(frame->id) = buf[0];
		*(frame->id + 1) = buf[1];
		*(frame->id + 2) = buf[2];
		*(frame->id + 3) = buf[3];
		
// then, the frame size is to come. Again, the four MSBs must be zero.
		if ( (buf[4] & 128) != 0
			|| (buf[5] & 128) != 0
			|| (buf[6] & 128) != 0
			|| (buf[7] & 128) != 0 )
			goto error;
		
		frame->datasize = buf[7] + (buf[6] << 7) + (buf[5] << 14) + (buf[4] << 21);
		
// A minimum size must be present!
		if ( frame->datasize <= 0 )
			goto error;
		
// The two following frame header flags have the 5 LSBs not set.
		if ( (buf[8] & 31) != 0
			|| (buf[9] & 31) != 0 )
			goto error;

// now, put the flags in the struct.
		frame->tagalter = (buf[8] & 128) >> 7;
		frame->filealter = (buf[8] & 64) >> 6;
		frame->readonly = (buf[8] & 32) >> 5;
		
		frame->compression = (buf[9] & 128) >> 7;
		frame->encryption = (buf[8] & 64) >> 6;
		frame->grouping = (buf[8] & 32) >> 5;
		
// ok, we are done with the frame header, so now we read the frame data.
		frame->data = (LPSTR) calloc (1, frame->datasize + 1);
		if( fread (frame->data, 1, frame->datasize, mp3file) < frame->datasize )
			goto error;
		
		numframes++;
		totalframesize += 10 + frame->datasize;
		
// we append it to a glist, which is appended to the v2 struct.
		v2->frames = dll_append (v2->frames, frame);
	}

// if we have no extheader, that means, we don't know how much padding we have!
// thus, i calculate it here.
	if ( !v2->has_extheader )
		v2->padding_size = v2->tag_size - totalframesize;
	
// A minimum of one frame is mandatory.
	if( numframes > 0 )
		return (TRUE);
	
error:
// cleanups in case of an error.
	if( frame && frame->data )
		free (frame->data);
	if( frame )
		free (frame);
	return (FALSE);
}















/*
 * Reads all tag information of a valid ID3V2.3 file.
 * When successful, the info is stored in the tag struct.
 * Returns:
 * 0 = success
 * 1 = Cannot open file.
 * 2 = No header or wrong version.
 * 3 = broken extheader.
 * 4 = broken frames.
 */
static int
get_id3v2tag_raw (id3v2Tag *v2, char *filename)
{
	FILE *mp3file = NULL;
	int error = 0;

// open file
	error = 1;
	mp3file = fopen (filename, "rb");
	if (!mp3file)
		goto done;
	
// check/get header
	error = 2;
	if( !check_header (mp3file, v2) )
		goto done;
	
// check/get extheader
	error = 3;
	if( v2->has_extheader
		&& !check_extheader (mp3file, v2) )
		goto done;

// get the content frames
	error = 4;
	if( !read_frames (mp3file, v2) )
		goto done;

	error = 0;
done:
	fclose (mp3file);
	return (error);
}
	
	
	
	
	
/*
 * Find one frames data and give bag its data in the correct format.
 * Returns TRUE on success, otherwise FALSE;
 */
static short int
frame_find (id3v2Tag *v2, char *name, char *value)
{
	DLL *curframe = NULL;
	id3v2Frame *frame = NULL;
	
// we parse through the whole list of frames, giving back the correct frame value.
	curframe = v2->frames;
	while ( curframe )
	{
		frame = (id3v2Frame *)curframe->data;

// Just to be sure...
		if( frame->datasize <= 0 )
			goto nextframe;
		
// Matches the users request? Otherwise try the next frame.
		if( memcmp (frame->id, name, 4) != 0 )
			goto nextframe;
		
// This types don't need much change, just give the whole data back to the user according to the encoding.
// The first byte is the encoding.
// TPE1: Artist
// TIT2: Song Title
// TALB: Album Title
// TYER: Year
// TRCK: Track
// TCON: Genre
// COMM: Comment
		if ( memcmp (frame->id, "TPE1", 4) == 0
			|| memcmp (frame->id, "TIT2", 4) == 0
			|| memcmp (frame->id, "TALB", 4) == 0
			|| memcmp (frame->id, "TYER", 4) == 0
			|| memcmp (frame->id, "TRCK", 4) == 0
			|| memcmp (frame->id, "TCON", 4) == 0
			|| memcmp (frame->id, "TALB", 4) == 0 )
		{
			if ( *frame->data == 0 )
				memcpy(value, frame->data + 1, frame->datasize - 1);
			if ( *frame->data == 1 )
			{
				char* nulltermvalue = new char[frame->datasize];
				char *isovalue = NULL;
			
// the tag is not null terminated, so i have to create a null terminated string first.
				memset (nulltermvalue, 0, frame->datasize);
				memcpy (nulltermvalue, frame->data + 1, frame->datasize - 1);
				
// Convert from UTF to ISO and copy to the users variable.
				isovalue = convert_from_utf8 (nulltermvalue);
				strncpy (value, isovalue, sizeof(value) - 1);

				free (isovalue);
				delete nulltermvalue;
			}
			
// change linefeeds to a single "return" key.
			crlf2cr (value);
			return (TRUE);
		}
		
// The comment requires special handling.
// Its data has: One byte "encoding" (0 = ISO-8859-1, 1 = UNICODE)
// followed by the language (three bytes, e.g. "eng"),
// followed by a short description,
// then a NULL,
// and the full description
// By now, i simply drop the short description
		if( memcmp(frame->id, "COMM", 4) == 0 )
		{
// check for the right format. (minsize 5, must contain a "\0" after the language)
			if ( frame->datasize < 5 )
				goto nextframe;
			if ( !memchr (frame->data + 4, '\0', frame->datasize - 4) )
				goto nextframe;

// now, give the data back to the user, according to the encoding.
			if ( *frame->data == 0 )
				memcpy (value, frame->data + 5, frame->datasize - 5);
			if ( *frame->data == 1 )
			{
				char* nulltermvalue = new char[frame->datasize];
				char *isovalue = NULL;
			
// the tag is not null terminated, so i have to create a null terminated string first.
				memset (nulltermvalue, 0, frame->datasize);
				memcpy (nulltermvalue, frame->data + 5, frame->datasize - 5);
				
// Convert from UTF to ISO and copy to the users variable.
				isovalue = convert_from_utf8 (nulltermvalue);
				strncpy (value, isovalue, sizeof(value) - 1);

				free (isovalue);
				delete nulltermvalue;
			}
				
// change linefeeds to a single "return" key.
			crlf2cr (value);
			return TRUE;
		}
		
nextframe:
		curframe = (DLL*) curframe->next;
	}
	
	return FALSE;
}
	



/*
 * Remove one frame out of the id3v2Tag struct
 * Returns TRUE on success, otherwise FALSE;
 */
static short int
frame_remove (id3v2Tag *v2, char *name)
{
	id3v2Frame *frame = NULL;
	DLL *curframe = NULL;
	DLL *tempframe = NULL;
	
// Parse through the list of frames.
	curframe = v2->frames;
	while ( curframe )
	{
		frame = (id3v2Frame *)curframe->data;
		tempframe = curframe;
		curframe = (DLL*) curframe->next;
		
		if ( memcmp (frame->id, name, 4) == 0 )
		{
// we have found the item! removing will NOT shrink the tag, but increase the padding.
			v2->padding_size += (frame->datasize + 10);
// and free memory.
			v2->frames = dll_remove (v2->frames, tempframe->data);
			free (frame->data);
			free (frame);
			
			return TRUE;
		}
	}
	
	return FALSE;
}



/*
 * Add a frame to the framelist. If the frame name is already in the list, it will be replaced.
 * Returns:
 * TRUE: The tag size HAS BEEN increased.
 * FALSE: The tag size has NOT been increased.
 */
static short int
frame_set (id3v2Tag *v2, char *name, char *value)
{
	id3v2Frame *frame = NULL;
	short int sizechange = FALSE;
	
// prevent the user to send CR/LF, which is forbidden.
	cr2crlf (value);
// eventually remove an existing item!
	frame_remove (v2, name);

// alloc space for the new frame.
	frame = (id3v2Frame*) malloc (sizeof(id3v2Frame));
	
	memcpy (frame->id, name, 4);
	frame->datasize = strlen (value);
	frame->tagalter = 0;
	frame->filealter = 0;
	frame->readonly = 0;
	frame->compression = 0;
	frame->encryption = 0;
	frame->grouping = 0;

// The comment requires special handling. If you need to know why, look at the documentation
// of the "frame_find" function above.
	if( memcmp (frame->id, "COMM", 4) == 0 )
	{
		char* fullvalue = new char[frame->datasize + 6];
		
		sprintf(fullvalue, "%ceng%c%s", 0, 0, value);
		
		frame->datasize += 5;
		frame->data = (LPSTR) malloc (frame->datasize);
		memcpy (frame->data, fullvalue, frame->datasize);

		delete fullvalue;
	}
	else
	{
		char* fullvalue = new char[frame->datasize + 2];
		
		sprintf (fullvalue, "%c%s", 0, value);
		
		frame->datasize += 1;
		frame->data = (LPSTR) malloc (frame->datasize);
		memcpy (frame->data, fullvalue, frame->datasize);

		delete fullvalue;
	}
	
// Ok. This decreases the available padding. If we have no padding left, we must increase the padding (and thus, the tag).
	if( v2->padding_size - (frame->datasize + 10) <= 0 )
	{
// add: framesize + frameheadersize + padding.
		v2->padding_size += frame->datasize + 10 + 1024;
		v2->tag_size += frame->datasize + 10 + 1024;
		
		sizechange = TRUE;
	}
	
// In every case, we must subtract the new allocated space from the padding.
	v2->padding_size -= frame->datasize + 10;
	
	v2->frames = dll_append (v2->frames, frame);
	
	return sizechange;
}



/*
 * Create raw header.
 * Returns:
 * TRUE: successful.
 * FALSE: unsuccessful.
 */
static int
create_header_raw (char *raw, id3v2Tag *v2)
{
// now we are going to write the tags raw data into the raw string
	memset (raw, 0, v2->tag_size + 10);
// ID3 identifier bytes
	memcpy (raw, "ID3", 3);
	raw += 3;
// major version byte
	*raw++ = 3;
// minor version byte
	*raw++ = 0;
// Flags byte
	*raw++ = ((v2->unsync & 1) << 7)
		| ((v2->has_extheader & 1) << 6)
		| ((v2->is_experimental & 1) << 5);
// Tag size. It must be syncsafe!
	*raw++ = ((v2->tag_size & 0x800000) >> 23) | (((v2->tag_size & 0x7f000000) >> 24) << 1);
	*raw++ = ((v2->tag_size & 0x8000) >> 15) | (((v2->tag_size & 0x7f0000) >> 16) << 1);
	*raw++ = ((v2->tag_size & 0x80) >> 7) | (((v2->tag_size & 0x7f00) >> 8) << 1);
	*raw++ = (v2->tag_size & 0x7f);
	
	return TRUE;
}




/*
 * Generates the frames. btw.: ID3 sucks!
 * Returns: TRUE if succesful, otherwise FALSE.
 */
static short int
create_frames_raw (char *raw, id3v2Tag *v2)
{
	id3v2Frame *frame = NULL;
	DLL *curframe = NULL;
	
// if we have no frames, just quit.
	if ( v2->frames == NULL )
		return FALSE;
	
// the header and extheader have already been written.
	raw += 10;
	if ( v2->has_extheader )
		raw += 4 + v2->extheader_size;
	
	curframe = v2->frames;
	while ( curframe )
	{
		frame = (id3v2Frame *)curframe->data;

// secure is secure
		if ( frame->datasize <= 0 )
			goto nextframe;
		
// add the frame id
		memcpy(raw, frame->id, 4);
		raw += 4;
// add the frame size (syncsafe)
		*raw++ = ((frame->datasize & 0x800000) >> 23) | (((frame->datasize & 0x7f000000) >> 24) << 1);
		*raw++ = ((frame->datasize & 0x8000) >> 15) | (((frame->datasize & 0x7f0000) >> 16) << 1);
		*raw++ = ((frame->datasize & 0x80) >> 7) | (((frame->datasize & 0x7f00) >> 8) << 1);
		*raw++ = (frame->datasize & 0x7f);

// The two flagbytes
		*raw++ = ((frame->tagalter & 1) << 7)
			| ((frame->filealter & 1) << 6)
			| ((frame->readonly & 1) << 5);
		
		*raw++ = ((frame->compression & 1) << 7)
			| ((frame->encryption & 1) << 6)
			| ((frame->grouping & 1) << 5);

// now the frame data.
		memcpy(raw, frame->data, frame->datasize);
		raw += frame->datasize;

nextframe:
		curframe = (DLL*) curframe->next;
	}
	
	return TRUE;
}

/***************************************************************************************
 * END OF STATICS
 ***************************************************************************************/







/*
 * Purpose: Reads the ID3 tag from a file.
 * Parameters: tag - The structure to store the tag in, filename - The name of the file to operate on.
 * Returns:
 * 0 if successful,
 * 1 if an error occured when opening the file
 * 2 if error while reading tag.
 * 3 if no TAG found.
 */
int
get_id3v2_tag (id3Tag *tag, char *filename)
{
	id3v2Tag *v2 = (id3v2Tag*) calloc (1, sizeof(id3v2Tag));
	DLL *curframe = NULL;
	int error = 0;
	
// Read the tag.
	error = get_id3v2tag_raw (v2, filename);
	
// Init the users tag
	memset (tag, 0, sizeof (id3Tag));
	
	if( error == 0 )
	{
// if we have a valid tag we copy the raw data to the users struct
		tag->size = v2->tag_size;
		
		frame_find (v2, "TPE1", tag->artist);
		frame_find (v2, "TIT2", tag->title);
		frame_find (v2, "TALB", tag->album);
		frame_find (v2, "TYER", tag->year);
		frame_find (v2, "COMM", tag->comment);
		frame_find (v2, "TRCK", tag->track);
		frame_find (v2, "TCON", tag->genre);
	}
	
// Free all the stuff
	if (v2->frames)
	{
		id3v2Frame *frame = NULL;
		
		curframe = v2->frames;
		while ( curframe )
		{
			frame = (id3v2Frame *)curframe->data;
			
			free (frame->data);
			free (frame);
			
			curframe = (DLL*) curframe->next;
		}
		v2->frames = dll_free (v2->frames);
	}
	
	free (v2);
	return (error);
}









/*
 * Purpose: Clear the ID3 tag of a file.
 * Parameters: a filename.
 * Returns:
 * 0 if successful,
 * 1 if an error occured when opening the file
 * 2 if an error while reading/writing the tag.
 */
int
del_id3v2_tag (char *filename)
{
	id3v2Tag *v2 = (id3v2Tag*) calloc (1, sizeof(id3v2Tag));
	long file_len;
	FILE *file;
	int read;
	void *ptr;

// check if an valid old id3v2 tag is present
// In these two error-cases we don't know how big the tag is.
	if( get_id3v2tag_raw (v2, filename) == 1
		|| get_id3v2tag_raw (v2, filename) == 2 )
		return(0);
	
	ptr =  malloc (4096);

// open file read/write
	file = fopen (filename, "r+b");
	if (!file)
		return (1);

	fseek (file, 0, SEEK_END);
	file_len = ftell (file);
	if ( file_len < 11
		|| v2->tag_size < 11 )
		return (2);

// set anything but the header in tag to zero. ill not really remove the tag,
// because this would be much slower and if we write a new tag, this would mean we´d have to
// rewrite the complete tag.
	fseek (file, 10, SEEK_SET);
	for (read = 0; read < v2->tag_size - 10; read++)
		fputc (0, file);

	fflush (file);
	fclose (file);
	free (ptr);
	free (v2);
	
	return(0);
}

int set_id3v2_tag (id3Tag *tag, char *filename)
{
	id3v2Tag *v2 = (id3v2Tag*) malloc (sizeof(id3v2Tag));
	id3v2Frame *frame = NULL;
	unsigned char *rawdata = NULL;
	DLL *curframe = NULL;
	int oldsize = 0;
	char track[3];
	int error = 0;

// Try to get the content of an old tag
	error = 1;
	memset (v2, 0, sizeof(id3v2Tag));
	get_id3v2tag_raw (v2, filename);
	
	oldsize = v2->tag_size;

// If the old tag had an extheader, ill add its size to my tag, because i don't plan to add it again.
	if ( v2->has_extheader )
	{
		v2->padding_size += v2->extheader_size;
		v2->has_extheader = FALSE;
		v2->extheader_size = 0;
	}
	
// first of all ill generate a valid id3v2 tag struct out of the tag struct we got by the user.
// Set the flags...
	v2->unsync = FALSE;
	v2->is_experimental = FALSE;
	v2->crc_data_present = FALSE;
	
// Set the contentframes
	frame_set (v2, "TIT2", tag->title);
	frame_set (v2, "TPE1", tag->artist);
	frame_set (v2, "TALB", tag->album);
	frame_set (v2, "TYER", tag->year);
	frame_set (v2, "COMM", tag->comment);
	frame_set (v2, "TCON", tag->genre);
	if ( atoi (tag->track) < 10 )
		sprintf (track, "0%i", atoi(tag->track));
	else
		sprintf (track, "%i", atoi(tag->track));
	frame_set (v2, "TRCK", track);
	
// Create a header in the raw data string
	rawdata = (BYTE*) calloc (1, v2->tag_size + 10);
	create_header_raw ((LPSTR) rawdata, v2);

// Create frames raw data.
	create_frames_raw ((LPSTR) rawdata, v2);
	
// is the new tag bigger than the old one? Then we'll have to completely rewrite the file...
	if ( v2->tag_size > oldsize )
	{
		FILE *file = NULL;
		FILE *tempfile = NULL;
		char *tempfilename = NULL;
		int read = 0;
		char buf[4096];

// Open a tempfile
		error = 2;
		tempfilename = (LPSTR) malloc (strlen (filename) + 11);
		sprintf (tempfilename, "%s%s", filename, ".tempXXXXX");
		if( !(tempfile = fopen(tempfilename, "wb")) )
		{
			remove (tempfilename);
			free (tempfilename);
			goto done;
		}
		
// Write the tag to the tempfile.
		error = 3;
		fseek (tempfile, 0, SEEK_SET);
		if( fwrite (rawdata, 1, v2->tag_size + 10, tempfile) < v2->tag_size )
		{
			fclose (tempfile);
			remove (tempfilename);
			free (tempfilename);
			goto done;
		}
		
// Open the mp3file.
		error = 4;
		if( !(file = fopen(filename, "r+b")) )
		{
			fclose (file);
			remove (tempfilename);
			free (tempfilename);
			goto done;
		}
// skip the old tag (if one existed)
		fseek (file, oldsize? oldsize + 10 : oldsize, SEEK_SET);
		
// copy the rest of the file to the tempfile.
		while ( !feof(file) )
		{
			error = 5;
			read = fread (buf, 1, 4096, file);
			if( fwrite (buf, 1, read, tempfile) != read
				&& !feof (file) )
			{
				remove (tempfilename);
				free (tempfilename);
				fflush (tempfile);
				fclose (tempfile);
				fflush (file);
				fclose (file);
				goto done;
			}
		}

		fflush (file);
		fclose (file);
		fflush (tempfile);
		fclose (tempfile);
		
// rename the tempfile, so it is the mp3file.
		rename (tempfilename, filename);
		free (tempfilename);
	}
	else
	{
		FILE *file = NULL;
		
// If the old tag was bigger than the new one, we can simply overwrite it!
// open.
		error = 10;
		if( !(file = fopen(filename, "r+b")) )
			goto done;

// write.
		error = 11;
		fseek (file, 0, SEEK_SET);
		if( fwrite (rawdata, 1, v2->tag_size + 10, file) < v2->tag_size )
		{
			fflush (file);
			fclose (file);
			goto done;
		}
		
		fflush (file);
		fclose (file);
	}
	
	error = 0;
	
done:
// Free all the stuff
	curframe = v2->frames;
	while ( curframe )
	{
		frame = (id3v2Frame *)curframe->data;
		
		free (frame->data);
		free (frame);
		
		curframe = (DLL*) curframe->next;
	}
	dll_free (v2->frames);

	if ( rawdata != NULL )
		free (rawdata);
	free (v2);
	return (error);
}
