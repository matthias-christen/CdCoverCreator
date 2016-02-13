#ifndef CMP3INFO_H
#define CMP3INFO_H

//#include "steve.h"
#include "CFrameHeader.h"
#include "CVBitRate.h"

/* ----------------------------------------------------------
   CMP3Info class is your complete guide to the 
   MP3 file format in the C++ language. It's a large class
   with three different, quite large "sub-classes" so it's
   a fair amount of code to look into.

   This code will be well commented, so that everyone can
   understand, as it's made for the public and not for
   private use, although private use is allowed. :)

   all functions specified both in the header and .cpp file
   will have explanations in both locations.

   everything here by: Gustav "Grim Reaper" Munkby
                       http://home.swipnet.se/grd/
                       grd@swipnet.se
   ---------------------------------------------------------- */


class CMP3Info {

    public:

    // function to load a file into this structure
    // the argument passed is the path to a MP3 file
    int   loadInfo( char srcMP3[256] );


    // functions used to get information about the "file"
    int   getFileSize() { return fileSize; };
    void  getFileName(char* input);

    // information that is avaliable in FrameHeader & VBR header
    void  getVersion(char* input);
    int   getBitrate();
    int   getFrequency() { return header.getFrequency(); };
    void  getMode(char* input);
    
    int   getNumberOfFrames();

    // functions to calculate the length of the song
    // and to present it nicely
    int   getLengthInSeconds();
    void  getFormattedLength(char* input);


    // just to know what kind of file it is.
    BOOL  isVBitRate() { return VBitRate; };
    BOOL  isTagged() { return Tagged; };
    
    private:

    // this alerts some error messages, so that it's
    // easier to see what's wrong
    void showErrorMessage(UINT msg, char* szMsgPlus = "");

    // these are the "sub-classes"
    CFrameHeader header;
    CVBitRate    vbr;
    
    // just to know what kind of file it is
    BOOL VBitRate;
    BOOL Tagged;

    // the file information can not be found elsewhere
    char fileName[256];
    int fileSize;

};

#endif
