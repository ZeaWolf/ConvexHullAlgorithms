#pragma once
#include "PointConvex.h"
#include "QuickHull.h"
#include "Minkowski.h"

class Func
{
public:

    Func() {}
    ~Func() {}

    static bool DoPointConvex(D2D1_POINT_2F target, std::vector<D2D1_POINT_2F>* points);

    static void DoQuickhull(std::vector<D2D1_POINT_2F>* points, std::vector<D2D1_POINT_2F>* results);

    static void DoMinkowskiSum(std::vector<D2D1_POINT_2F>* arr1, std::vector<D2D1_POINT_2F>* arr2, std::vector<D2D1_POINT_2F>* results);

    static void DoMinkowskiDiff(std::vector<D2D1_POINT_2F>* arr1, std::vector<D2D1_POINT_2F>* arr2, std::vector<D2D1_POINT_2F>* results);

    static bool GJK(std::vector<D2D1_POINT_2F>* arr1, std::vector<D2D1_POINT_2F>* arr2, std::vector<D2D1_POINT_2F>* results);
};