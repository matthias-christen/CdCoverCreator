// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0500	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#define _USE_MATH_DEFINES

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <MMSystem.h>

#include <afxinet.h>
#include <afxsock.h>

#include <afxpriv.h>
#include <htmlhelp.h>

#include "Help.h"

enum TrackType
{
	TrackNum=0,
	TrackLen,
	TrackTitle,
	TrackRemark
};
#define NUM_TRACK_TYPES 4

enum RenderType
{
	Cover=0,
	Back,
	Inlay,
	Label,
	Booklet
};
#define NUM_RENDER_TYPES 5

// CG: The following line was added by the Windows Multimedia component.
#pragma comment(lib, "winmm.lib")

#include <imm.h>
#include <afxdlgs.h>
#include <afxdhtml.h>
#import "riched20.dll" raw_interfaces_only, raw_native_types, no_namespace, named_guids

//////////////////////////////////////////////////////////////
// Is this the Peroutka & Fedorisin release?
//#define PEROUTKA_FEDORISIN
//#define TESTING_VERSION

#ifdef PEROUTKA_FEDORISIN
	#pragma setlocale("polish")
#endif