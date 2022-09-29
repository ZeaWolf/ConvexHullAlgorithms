#pragma once

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

struct vector
{
	float x;
	float y;
};

struct pixel
{
	float x;
	float y;
};

struct D2D1_POINT_2F
{
	float x;
	float y;
};