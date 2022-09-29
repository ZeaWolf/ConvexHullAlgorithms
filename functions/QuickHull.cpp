#include "QuickHull.h"
QuickHull::QuickHull() : Preset()
{
	Reset();
}

QuickHull::~QuickHull()
{
}

void QuickHull::Reset()
{
	QuickHull::points = {};
	QuickHull::hulls = {};

	srand(time(NULL));
	point t = { (rand() % QuickHull::MAXWIDTH) / 1.33, (rand() % QuickHull::MAXWIDTH) / 1.33 };

	for (int i = 0; i < 15; i++)
	{
		int x = (rand() % QuickHull::MAXWIDTH) / 1.33;
		int y = (rand() % QuickHull::MAXHEIGHT) / 1.33;
		point p = { x, y };
		QuickHull::points.push_back(p);
	}
	QuickHull::CalculateOutterHulls();
}

void QuickHull::CalculateOutterHulls()
{
	QuickHull::hulls = CalculationHelper::GetOutterHulls(QuickHull::points);
}






