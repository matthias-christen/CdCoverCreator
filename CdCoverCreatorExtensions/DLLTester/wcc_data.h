#include <list>

// WCC

typedef struct tagCoversImagesList
{
	LPSTR lpszName;
	LPSTR lpszCover, lpszBack, lpszInlay, lpszLabel;
} COVERSIMAGESLIST;

typedef struct tagCoversList
{
	std::list<COVERSIMAGESLIST*> listImages;	// array of cover images
	LPSTR lpszRequest;							// request for next page
} COVERSLIST;

typedef LPSTR (WCC_GETHOST) (void);
typedef int (WCC_GETLIBVERSION) (void);
typedef void (WCC_LOGIN) (LPSTR, int*, LPSTR, LPSTR);
typedef void (WCC_LOGOUT) (LPSTR, LPSTR, int*, LPSTR, LPSTR);
typedef void (WCC_GETREQUEST) (LPSTR, LPSTR, LPSTR, int*, LPSTR, LPSTR);
typedef void (WCC_GETIMAGEREQUEST) (LPSTR, LPSTR, LPSTR, LPSTR, int*, LPSTR, LPSTR);
typedef bool (WCC_GETLIST) (LPSTR, LPSTR, LPSTR, LPSTR, int*, LPSTR, LPSTR, int*);
typedef bool (WCC_GETIMAGEURL) (LPSTR, LPSTR, int*, LPSTR, LPSTR);

// WSL

typedef void (WSL_GETREQUEST) (LPSTR, LPSTR, LPSTR, int*, LPSTR, LPSTR);
typedef bool (WSL_GETLYRICS) (LPSTR, LPSTR, LPSTR, LPSTR, LPSTR, LPSTR, bool, int*, LPSTR, LPSTR);
