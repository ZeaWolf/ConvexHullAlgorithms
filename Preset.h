#pragma once
#pragma comment(lib, "d2d1")
#include <d2d1.h>

class Preset
{

public:
	int MAXWIDTH;
	int MAXHEIGHT;

	Preset()
	{
		MAXWIDTH = 100;
		MAXHEIGHT = 100;
	}
};

struct point
{
	float x;
	float y;
};

struct cvector
{
	float x;
	float y;
};

struct pixel
{
	float x;
	float y;
};
