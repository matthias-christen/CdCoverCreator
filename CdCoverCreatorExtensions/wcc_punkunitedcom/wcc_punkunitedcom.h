// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the WCC_PUNKUNITEDCOM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// WCC_PUNKUNITEDCOM_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef WCC_PUNKUNITEDCOM_EXPORTS
#define WCC_API extern "C" __declspec(dllexport)
#else
#define WCC_API __declspec(dllimport)
#endif

WCC_API LPSTR GetHost ();
WCC_API int GetLibVersion ();

WCC_API void Login (LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszData /* out */);
WCC_API void Logout (LPSTR lpszCookie /* in */,
	LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszData /* out */);

WCC_API void GetListCoversRequest (LPSTR lpszLetter /* in */, LPSTR lpszCookie /* in */,
	LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszData /* out */);
WCC_API bool ListCovers (LPSTR lpszResponse /* in */, LPSTR lpszLetter /* in */,
	LPSTR lpszData /* out */,
	LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszHttpData /* out */,
	int* pnPages /* out */);

WCC_API void GetSearchCoversRequest (LPSTR lpszKeywords /* in */, LPSTR lpszCookie /* in */,
	LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszData /* out */);
WCC_API bool SearchCovers (LPSTR lpszResponse /* in */, LPSTR lpszKeywords /* in */,
	LPSTR lpszData /* out */,
	LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszHttpData /* out */,
	int* pnPages /* out */);

WCC_API void GetImageRequest (LPSTR lpszName /* in */, LPSTR lpszLetter /* in */, LPSTR lpszCookie /* in */, 
	LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszData /* out */);
WCC_API bool GetImageURL (LPSTR lpszResponse /* in */,
	LPSTR lpszUrl /* out */, int* pnMethod /* out */, LPSTR lpszHeaders /* out */, LPSTR lpszData /* out */);