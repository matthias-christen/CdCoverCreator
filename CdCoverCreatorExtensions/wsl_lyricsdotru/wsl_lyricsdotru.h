
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WSL_lyricsdotru_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WSL_lyricsdotru_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef WSL_LYRICSDOTRU_EXPORTS
#define WSL_API extern "C" __declspec(dllexport)
#else
#define WSL_API __declspec(dllimport)
#endif

WSL_API LPSTR GetHost ();
WSL_API int GetLibVersion ();

WSL_API void GetLyricsRequest (LPSTR lpszTitle /* in */, LPSTR lpszArtist /* in */,
	LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszData /* out */);
WSL_API bool GetLyrics (LPSTR lpszResponse /* in */,
	LPSTR lpszArtist /* out */, LPSTR lpszAlbum /* out */, LPSTR lpszTitle /* out */,
	LPSTR lpszText /* out */, LPSTR lpszUrl /* out */, bool bCopyUrls /* in */, int* pnMethod /* out */,
	LPSTR lpszHeaders /* out */, LPSTR lpszHttpData /* out */);
