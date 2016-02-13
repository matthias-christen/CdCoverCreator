#pragma once

#include <Gdiplus.h>

#define MAX_TICKS 16

enum GradientType
{
	Linear = 0,
	Radial,
	Rectangular,
	Diamond,
	Triangular,
	Radial_TopLeft,
	Wheel
};

typedef struct tagLinearGradientData
{
	COLORREF colors[MAX_TICKS];
	float positions[MAX_TICKS];
	int nNumColors;
	double dAngle;
	GradientType type;
} LINEARGRADIENTDATA;

#define PI 3.1415926535897932384626433832795

double normalize_rad (double rad);
int normalize_deg (int nDeg);
double deg2rad (int nDeg);
int rad2deg (double rad);

bool compare_lineargradientdata (LINEARGRADIENTDATA* pLgd1, LINEARGRADIENTDATA* pLgd2);

class CGradient
{
public:
	CGradient(void);
	~CGradient(void);

	static Gdiplus::LinearGradientBrush* CreateLinearGradientBrush (CRect rectGradient, double dAngle, Gdiplus::Color* colors, float* positions, int nNumColors);
	static Gdiplus::LinearGradientBrush* CreateLinearGradientBrush (CRect rectGradient, double dAngle, COLORREF* colors, float* positions, int nNumColors);
	static Gdiplus::LinearGradientBrush* CreateLinearGradientBrush (CRect rectGradient, double dAngle, COLORREF colStart, COLORREF colEnd);
	static Gdiplus::Brush* CreateGradientBrush (CRect rectGradient, double dAngle, Gdiplus::Color* colors, float* positions, int nNumColors, GradientType type, Gdiplus::GraphicsPath** ppPath = NULL);
	static Gdiplus::Brush* CreateGradientBrush (CRect rectGradient, LINEARGRADIENTDATA lgd, Gdiplus::GraphicsPath** ppPath = NULL);
};
