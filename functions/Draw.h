#pragma once

#include <windows.h>
#include "Preset.h"
#include <d2d1.h>
#include <string>
#pragma comment(lib, "d2d1")

class Draw
{

public:
	HWND hwnd;
	ID2D1Factory* pFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1SolidColorBrush* pBrush;

	int magnify;
	int xff;
	int yff;

	Draw(HWND h) : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
	{
		hwnd = h;
		magnify = 1;
		xff = 0;
		yff = 0;
	}

	void Reset();

	D2D1_COLOR_F ConvertString(std::string colorStr);
	
	void DrawPoint(point p, std::string color);

	void DrawLine(point p1, point p2, std::string color);

	void BeginDraw();

	void EndDraw();

	pixel PointToPixel(point pt);
	point PixelToPoint(pixel px);
};