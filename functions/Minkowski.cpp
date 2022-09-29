#include "Minkowski.h"

void Minkowski::Reset()
{
	Minkowski::pointsA = {};
	Minkowski::hullsA = {};

	Minkowski::pointsB = {};
	Minkowski::hullsB = {};
	Minkowski::sumPoints = {};
	Minkowski::diffPoints = {};
	
	Minkowski::isOverlap = false;

	for (int i = 0; i < 5; i++)
	{
		int x = (rand() % Minkowski::MAXWIDTH);
		int y = (rand() % Minkowski::MAXHEIGHT);
		point p = { x, y };
		Minkowski::pointsA.push_back(p);
	}

	for (int i = 0; i < 5; i++)
	{
		int x = (rand() % Minkowski::MAXWIDTH) + 30;
		int y = (rand() % Minkowski::MAXHEIGHT) + 30;
		point p = { x, y};
		Minkowski::pointsB.push_back(p);
	}

	Minkowski::hullsA = CalculationHelper::GetOutterHulls(pointsA);
	Minkowski::hullsB = CalculationHelper::GetOutterHulls(pointsB);
	Minkowski::Calculate();
}

void Minkowski::Calculate()
{
	Minkowski::isOverlap = false;
	for (int i = 0; i < Minkowski::pointsA.size(); i++)
	{
		for (int j = 0; j < Minkowski::pointsA.size(); j++)
		{
			point np1 = { Minkowski::pointsA[i].x + Minkowski::pointsB[j].x, Minkowski::pointsA[i].y + Minkowski::pointsB[j].y };
			point np2 = { Minkowski::pointsA[i].x - Minkowski::pointsB[j].x, Minkowski::pointsA[i].y - Minkowski::pointsB[j].y };
			Minkowski::sumPoints.push_back(np1);
			Minkowski::diffPoints.push_back(np2);
		}
	}

	Minkowski::sumPoints = CalculationHelper::GetOutterHulls(Minkowski::sumPoints);
	Minkowski::diffPoints = CalculationHelper::GetOutterHulls(Minkowski::diffPoints);


	Minkowski::isOverlap = CalculationHelper::IsPointInsideConvex(diffPoints, { 0,0 });
}
