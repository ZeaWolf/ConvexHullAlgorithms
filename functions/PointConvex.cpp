#include "PointConvex.h"

PointConvex::PointConvex() : Preset()
{
	Reset();
}

PointConvex::~PointConvex()
{
}

void PointConvex::Reset()
{
	PointConvex::points = {};
	PointConvex::hulls = {};
	PointConvex::state = false;

	srand(time(NULL));
	point t = { (rand() % PointConvex::MAXWIDTH) / 1.33, (rand() % PointConvex::MAXWIDTH) / 1.33 };
	PointConvex::target = t;
	for (int i = 0; i < 15; i++)
	{
		int x = (rand() % PointConvex::MAXWIDTH) / 1.33;
		int y = (rand() % PointConvex::MAXHEIGHT) / 1.33;
		point p = { x, y };
		PointConvex::points.push_back(p);
	}
	IsPointInsideConvex(PointConvex::target);
}

void PointConvex::CalculateOutterHulls()
{
	PointConvex::hulls = CalculationHelper::GetOutterHulls(PointConvex::points);
}

bool PointConvex::IsPointInsideConvex(point t)
{
	PointConvex::CalculateOutterHulls();
	bool s = CalculationHelper::IsPointInsideConvex(hulls, t);
	PointConvex::state = s;
	return s;
}