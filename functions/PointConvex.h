#include "Preset.h"
#include "CalculationHelper.h"
#include <vector>
#include <cstdlib>

class PointConvex : Preset
{
public:
	PointConvex();
	~PointConvex();

	bool Calculate(D2D1_POINT_2F t, std::vector<D2D1_POINT_2F> * pts);
};