#include "PointConvex.h"


bool PointConvex::Calculate(D2D1_POINT_2F target, std::vector<D2D1_POINT_2F>* points)
{
	//D2D1_POINT_2F i = { -3.0, -13.0 };
	//D2D1_POINT_2F a = { -3.0, -11.0 };
	//D2D1_POINT_2F b = { -3.10461671, -9.83550072 };

	//std::vector<D2D1_POINT_2F> abc;
	//abc.push_back(i);
	//abc.push_back(a);
	//abc.push_back(b);

	//abc = CalculationHelper::SortByAngle(abc);

	//if (CalculationHelper::IsCCW(CalculationHelper::ToVector(i, a), CalculationHelper::ToVector(i, b)) == 1)
	//{
	//	return true;
	//}
	//return false;
	return CalculationHelper::IsPointInsideConvex(*points, target);
}