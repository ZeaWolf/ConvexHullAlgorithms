#include "PointConvex.h"
#include "QuickHull.h"
#include "Minkowski.h"

class Func
{
    public:

    Func(){}
    ~Func(){}

    static bool DoPointConvex(D2D1_POINT_2F target, std::vector<D2D1_POINT_2F> * points)
    {
        PointConvex * pc = new PointConvex();
        return pc->Calculate(target, points);
    }

    static void DoQuickhull(std::vector<D2D1_POINT_2F> * points,  std::vector<D2D1_POINT_2F> * results)
    {   
        QuickHull * qh = new QuickHull(points, results);
    }
    
    static void DoMinkowskiSum(std::vector<D2D1_POINT_2F> * arr1, std::vector<D2D1_POINT_2F> * arr2, std::vector<D2D1_POINT_2F> * results)
    {
        Minkowski *mk = new Minkowski();
        return mk->CalculateSum(*arr1, *arr2, results);
    }

    static void DoMinkowskiDiff(std::vector<D2D1_POINT_2F> * arr1, std::vector<D2D1_POINT_2F> * arr2, std::vector<D2D1_POINT_2F> * results)
    {
        Minkowski *mk = new Minkowski();
        return mk->CalculateSum(*arr1, *arr2, results);
    }   

    static bool GJK(std::vector<D2D1_POINT_2F> * arr1, std::vector<D2D1_POINT_2F> * arr2, std::vector<D2D1_POINT_2F> * results)
    {
        Minkowski *mk = new Minkowski();
        return mk->CalculateGJK(*arr1, *arr2, results);
    }
};