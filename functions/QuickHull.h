#pragma once
#include "Preset.h"
#include "CalculationHelper.h"

class QuickHull : Preset
{

public:

	std::vector<point> points;
	std::vector<point> hulls;

	QuickHull();
	~QuickHull();

	void Reset();

	void CalculateOutterHulls();
};