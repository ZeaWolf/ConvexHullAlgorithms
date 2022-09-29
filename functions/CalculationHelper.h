#pragma once
#include "Preset.h"
#include <vector>
#include <algorithm>
#include <stack>
#include <iostream>
#include <cmath>

class CalculationHelper
{
public:
	static D2D1_POINT_2F lowestPoint;

	CalculationHelper();

	static void GetOutterHulls(std::vector<D2D1_POINT_2F> points, std::vector<D2D1_POINT_2F> * results);

	static std::vector<D2D1_POINT_2F> SortByAngle(std::vector<D2D1_POINT_2F> points);
	static bool SortByAngleFunc(D2D1_POINT_2F a, D2D1_POINT_2F b);

	// a -> r -> c
	static int IsCCW(vector a, vector b);

	static vector ToVector(D2D1_POINT_2F a, D2D1_POINT_2F b);

	static D2D1_POINT_2F getLowestPoint(std::vector<D2D1_POINT_2F> points);

	static bool isPointEqual(D2D1_POINT_2F a, D2D1_POINT_2F b);

	static bool IsPointInsideConvex(std::vector<D2D1_POINT_2F> hulls, D2D1_POINT_2F t);

	static D2D1_POINT_2F GetLeftMostPoint(std::vector<D2D1_POINT_2F> hulls);

	static int GetLastIndexUnderTarget(std::vector<D2D1_POINT_2F> hulls, D2D1_POINT_2F lowestPoint, D2D1_POINT_2F target);
};