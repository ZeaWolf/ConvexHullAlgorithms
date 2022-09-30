#include "func.h"


bool Func::DoPointConvex(D2D1_POINT_2F target, std::vector<D2D1_POINT_2F>* points)
{
    PointConvex* pc = new PointConvex();
    return pc->Calculate(target, points);
}

void Func::DoQuickhull(std::vector<D2D1_POINT_2F>* points, std::vector<D2D1_POINT_2F>* results)
{
    QuickHull* qh = new QuickHull(points, results);
    qh->Calculate();
}

void Func::DoMinkowskiSum(std::vector<D2D1_POINT_2F>* arr1, std::vector<D2D1_POINT_2F>* arr2, std::vector<D2D1_POINT_2F>* results)
{
    Minkowski* mk = new Minkowski();
    mk->CalculateSum(*arr1, *arr2, results);
}

void Func::DoMinkowskiDiff(std::vector<D2D1_POINT_2F>* arr1, std::vector<D2D1_POINT_2F>* arr2, std::vector<D2D1_POINT_2F>* results)
{
    Minkowski* mk = new Minkowski();
    return mk->CalculateSum(*arr1, *arr2, results);
}

bool Func::GJK(std::vector<D2D1_POINT_2F>* arr1, std::vector<D2D1_POINT_2F>* arr2, std::vector<D2D1_POINT_2F>* results)
{
    Minkowski* mk = new Minkowski();
    return mk->CalculateGJK(*arr1, *arr2, results);
}
