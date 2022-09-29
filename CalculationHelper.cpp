#include "CalculationHelper.h"

D2D1_POINT_2F CalculationHelper::lowestPoint = { 0,0 };

CalculationHelper::CalculationHelper()
{
}

void CalculationHelper::GetOutterHulls(std::vector<D2D1_POINT_2F> points, std::vector<D2D1_POINT_2F>* results)
{
	points = SortByAngle(points);

	std::stack<D2D1_POINT_2F> stack;

	stack.push(points[0]);
	stack.push(points[1]);

	for (int i = 2; i < points.size(); i++)
	{
		D2D1_POINT_2F next = points[i];
		D2D1_POINT_2F p = stack.top();
		stack.pop();
		while (!stack.empty() && IsCCW(ToVector(stack.top(), p), ToVector(p, next)) == -1)
		{
			p = stack.top();
			stack.pop();
		}
		stack.push(p);
		stack.push(next);
	}
	int size = stack.size();
	for (int i = 0; i < size; i++)
	{
		results->push_back(stack.top());
		stack.pop();
	}
	std::reverse(results->begin(), results->end());
}

std::vector<D2D1_POINT_2F> CalculationHelper::SortByAngle(std::vector<D2D1_POINT_2F> points)
{
	CalculationHelper::lowestPoint = getLowestPoint(points);
	std::sort(points.begin(), points.end(), SortByAngleFunc);
	return points;
}

bool CalculationHelper::SortByAngleFunc(D2D1_POINT_2F a, D2D1_POINT_2F b)
{
	D2D1_POINT_2F lp = CalculationHelper::lowestPoint;
	if (isPointEqual(a, lp)) { return true; }
	if (isPointEqual(b, lp)) { return false; }

	int isCCW = IsCCW(ToVector(lp, a), ToVector(lp, b));

	if (isCCW == 0)
	{
		if (a.x == b.x)
		{
			return (a.y < b.y);
		}
		return pow((lp.x - a.x), 2) < pow((lp.x - b.x), 2);
	}
	if (isCCW == 1) { return true; }
	return false;
}

// a -> r -> c
int CalculationHelper::IsCCW(cvector a, cvector b)
{
	int dir = a.x * b.y - a.y * b.x;
	if (dir == 0) return 0;
	else if (dir > 0) return 1;
	return -1;
}

cvector CalculationHelper::ToVector(D2D1_POINT_2F a, D2D1_POINT_2F b)
{
	cvector v = { b.x - a.x, b.y - a.y };
	return v;
}

D2D1_POINT_2F CalculationHelper::getLowestPoint(std::vector<D2D1_POINT_2F> points)
{
	D2D1_POINT_2F minPoint = { 0, 100 };
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].y < minPoint.y)
		{
			minPoint = points[i];
		}
	}

	return minPoint;
}

bool CalculationHelper::isPointEqual(D2D1_POINT_2F a, D2D1_POINT_2F b)
{
	return (a.x == b.x && a.y == b.y);
}

bool CalculationHelper::IsPointInsideConvex(std::vector<D2D1_POINT_2F> points, D2D1_POINT_2F t)
{
	D2D1_POINT_2F lowestHull = CalculationHelper::getLowestPoint(points);

	if (t.y < lowestHull.y) return false;

	int lastIndexUnderTarget = CalculationHelper::GetLastIndexUnderTarget(points, lowestHull, t);

	// if the target stays behind of the first and last vector of the outter hullls
	if (lastIndexUnderTarget == 0 || lastIndexUnderTarget == points.size() - 1)
	{
		return false;
	}

	cvector pToNp = CalculationHelper::ToVector(points[lastIndexUnderTarget], points[lastIndexUnderTarget + 1]); // vector (point to the next point)
	cvector pToT = CalculationHelper::ToVector(points[lastIndexUnderTarget], t); // vector (point to target)

	bool result = (CalculationHelper::IsCCW(pToNp, pToT) != -1);

	return result;
}

D2D1_POINT_2F CalculationHelper::GetLeftMostPoint(std::vector<D2D1_POINT_2F> hulls)
{
	D2D1_POINT_2F lmp = { 101, 0 };
	for (int i = 0; i < hulls.size(); i++)
	{
		D2D1_POINT_2F p = hulls[i];
		if (p.x < lmp.x)
		{
			lmp = p;
		}
	}
	return lmp;
}

int CalculationHelper::GetLastIndexUnderTarget(std::vector<D2D1_POINT_2F> hulls, D2D1_POINT_2F lowestPoint, D2D1_POINT_2F target)
{
	cvector t = CalculationHelper::ToVector(lowestPoint, target);
	cvector v;
	int index = 0;

	for (int i = 1; i < hulls.size(); i++)
	{
		v = CalculationHelper::ToVector(hulls[0], hulls[i]);

		if (CalculationHelper::IsCCW(v, t) != -1)
		{
			index = i;
		}
		else { break; }
	}
	return index;
}