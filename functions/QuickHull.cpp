#include "QuickHull.h"
QuickHull::QuickHull(std::vector<D2D1_POINT_2F> * _points, std::vector<D2D1_POINT_2F> * _results) : Preset()
{
	QuickHull::points = _points;
	QuickHull::results = _results;
}

QuickHull::~QuickHull()
{
}

void QuickHull::Calculate()
{
	CalculationHelper::GetOutterHulls(*QuickHull::points, QuickHull::results);
}






