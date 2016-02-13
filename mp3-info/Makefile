#  Non-portable Makefile for the test programs which are shipped along
# with GNUMP3d.
#
# Steve
# ---
#

CC             = g++
DEFAULT_CFLAGS =  -I. -g 
CFLAGS         = $(DEFAULT_CFLAGS)


#
#  Test the CUtility class
#
mp3-info: CFrameHeader.cpp  CMP3Info.cpp  CVBitRate.cpp  test.cpp
	$(CC) CFrameHeader.cpp  CMP3Info.cpp  CVBitRate.cpp  test.cpp $(CFLAGS) -o mp3-info $(LIBS) $(LDFLAGS)

#
# The binaries which we'll build.
#

clean:
	rm -f *~
	rm -f mp3-info

