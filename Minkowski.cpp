#include "Minkowski.h"


void Minkowski::CalculateSum(std::vector<D2D1_POINT_2F> arr1, std::vector<D2D1_POINT_2F> arr2, std::vector<D2D1_POINT_2F>* results)
{
	for (int i = 0; i < arr1.size(); i++)
	{
		for (int j = 0; j < arr2.size(); j++)
		{
			D2D1_POINT_2F np1 = { arr1[i].x + arr2[j].x, arr1[i].y + arr2[j].y };
			results->push_back(np1);
		}
	}
}

void Minkowski::CalculateDiff(std::vector<D2D1_POINT_2F> arr1, std::vector<D2D1_POINT_2F> arr2, std::vector<D2D1_POINT_2F>* results)
{
	for (int i = 0; i < arr1.size(); i++)
	{
		for (int j = 0; j < arr2.size(); j++)
		{
			D2D1_POINT_2F np1 = { arr1[i].x - arr2[j].x, arr1[i].y - arr2[j].y };
			results->push_back(np1);
		}
	}
}

bool Minkowski::CalculateGJK(std::vector<D2D1_POINT_2F> arr1, std::vector<D2D1_POINT_2F> arr2, std::vector<D2D1_POINT_2F>* results)
{
	std::vector<D2D1_POINT_2F> tempResult;
	CalculateDiff(arr1, arr2, &tempResult);
	CalculationHelper::GetOutterHulls(tempResult, results);
	return CalculationHelper::IsPointInsideConvex(*results, { 0, 0 });
}
