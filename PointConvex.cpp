#include "PointConvex.h"


bool PointConvex::Calculate(D2D1_POINT_2F target, std::vector<D2D1_POINT_2F>* points)
{
	return CalculationHelper::IsPointInsideConvex(*points, target);
}