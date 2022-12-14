#pragma once
#include "Preset.h"
#include "CalculationHelper.h"

class Minkowski : Preset
{

public:
	Minkowski() {}
	~Minkowski() {}

	void CalculateSum(std::vector<D2D1_POINT_2F> arr1, std::vector<D2D1_POINT_2F> arr2, std::vector<D2D1_POINT_2F>* results);
	void CalculateDiff(std::vector<D2D1_POINT_2F> arr1, std::vector<D2D1_POINT_2F> arr2, std::vector<D2D1_POINT_2F>* results);
	bool CalculateGJK(std::vector<D2D1_POINT_2F> arr1, std::vector<D2D1_POINT_2F> arr2, std::vector<D2D1_POINT_2F>* results);
};