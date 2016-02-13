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

#ifndef DLL_H
#define DLL_H
typedef struct DLL_s
{
	void *prev;
	void *data;
	void *next;
} DLL;
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#ifndef id3Tag_def
#define id3Tag_def

typedef struct id3Tag_s
{
	char title[1024];
	char artist[1024];
	char album[1024];
	char year[5];
	char comment[1024];
	char track[3];
	char genre[512];
	unsigned int size;
	short int has_footer;
} id3Tag;

typedef struct id3v2Tag_s
{
// header
	int tag_size;
	short int unsync;
	short int has_extheader;
	short int is_experimental;
//extheader
	int extheader_size;
	int padding_size;
	short int crc_data_present;
	char crc_data[4];
// frames
	DLL *frames;
} id3v2Tag;

typedef struct id3v2Frame_s
{
	unsigned char id[4];
	int datasize;
	short int tagalter;
	short int filealter;
	short int readonly;
	short int compression;
	short int encryption;
	short int grouping;
	char *data;
} id3v2Frame;

#endif

extern int get_id3v2_tag(id3Tag *tag, char *filename);
extern int set_id3v2_tag(id3Tag *tag, char *filename);
extern int del_id3v2_tag(char *filename);
