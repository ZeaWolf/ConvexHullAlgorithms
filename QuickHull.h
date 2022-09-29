#pragma once
#include "Preset.h"
#include "CalculationHelper.h"

class QuickHull : Preset
{

public:

	std::vector<D2D1_POINT_2F>* points;
	std::vector<D2D1_POINT_2F>* results;

	QuickHull(std::vector<D2D1_POINT_2F>* points, std::vector<D2D1_POINT_2F>* results);
	~QuickHull();

	void Calculate();
};