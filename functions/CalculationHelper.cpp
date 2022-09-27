#include "CalculationHelper.h"

point CalculationHelper::lowestPoint = { 0,0 };

CalculationHelper::CalculationHelper()
{
}

std::vector<point> CalculationHelper::GetOutterHulls(std::vector<point> points)
{
	points = SortByAngle(points);

	std::stack<point> stack;

	stack.push(points[0]);
	stack.push(points[1]);

	for (int i = 2; i < points.size(); i++)
	{
		point next = points[i];
		point p = stack.top();
		stack.pop();
		while (!stack.empty() && IsCCW(ToVector(stack.top(), p), ToVector(p, next)) == -1)
		{
			p = stack.top();
			stack.pop();
		}
		stack.push(p);
		stack.push(next);
	}
	std::vector<point> hulls;
	int size = stack.size();
	for (int i = 0; i < size; i++)
	{
		hulls.push_back(stack.top());
		stack.pop();
	}
	std::reverse(hulls.begin(), hulls.end());
	return hulls;
}

std::vector<point> CalculationHelper::SortByAngle(std::vector<point> points)
{
	CalculationHelper::lowestPoint = getLowestPoint(points);
	std::sort(points.begin(), points.end(), SortByAngleFunc);
	return points;
}

bool CalculationHelper::SortByAngleFunc(point a, point b)
{
	point lp = CalculationHelper::lowestPoint;
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
int CalculationHelper::IsCCW(vector a, vector b)
{
	int dir = a.x * b.y - a.y * b.x;
	if (dir == 0) return 0;
	else if (dir > 0) return 1;
	return -1;
}

vector CalculationHelper::ToVector(point a, point b)
{
	vector v = { b.x - a.x, b.y - a.y };
	return v;
}

point CalculationHelper::getLowestPoint(std::vector<point> points)
{
	point minPoint = { 0, 100 };
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].y < minPoint.y)
		{
			minPoint = points[i];
		}
	}

	return minPoint;
}

bool CalculationHelper::isPointEqual(point a, point b)
{
	return (a.x == b.x && a.y == b.y);
}

bool CalculationHelper::IsPointInsideConvex(std::vector<point> hulls, point t)
{
	point lowestHull = CalculationHelper::getLowestPoint(hulls);

	if (t.y < lowestHull.y) return false;

	int lastIndexUnderTarget = CalculationHelper::GetLastIndexUnderTarget(hulls, lowestHull, t);

	// if the target stays behind of the first and last vector of the outter hullls
	if (lastIndexUnderTarget == 0 || lastIndexUnderTarget == hulls.size() - 1)
	{
		return false;
	}

	vector pToNp = CalculationHelper::ToVector(hulls[lastIndexUnderTarget], hulls[lastIndexUnderTarget + 1]); // vector (point to the next point)
	vector pToT = CalculationHelper::ToVector(hulls[lastIndexUnderTarget], t); // vector (point to target)

	bool result = (CalculationHelper::IsCCW(pToNp, pToT) != -1);

	return result;
}

point CalculationHelper::GetLeftMostPoint(std::vector<point> hulls)
{
	point lmp = { 101, 0 };
	for (int i = 0; i < hulls.size(); i++)
	{
		point p = hulls[i];
		if (p.x < lmp.x)
		{
			lmp = p;
		}
	}
	return lmp;
}

int CalculationHelper::GetLastIndexUnderTarget(std::vector<point> hulls, point lowestPoint, point target)
{
	vector t = CalculationHelper::ToVector(lowestPoint, target);
	vector v;
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