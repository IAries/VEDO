#ifndef _CALC_COORDINATE_NUM_H
#define _CALC_COORDINATE_NUM_H

#include <vedo/framework/interfaces/DOWorld.h>
#include <string>

class CalcCoordinateNum
{

public:

	CalcCoordinateNum()
	{
	};

	vedo::vedo_float_t computeAvgCoordinateNum
		(const vedo::DOWorld* pWorld,
		 vedo::vedo_float_t xmin,
		 vedo::vedo_float_t xmax,
		 vedo::vedo_float_t ymin,
		 vedo::vedo_float_t ymax,
		 vedo::vedo_float_t zmin,
		 vedo::vedo_float_t zmax ) const;

	void outputCoordinateNumVTK
		(const vedo::DOWorld* pWorld, const std::string& filename) const;
};

#endif // _CALC_COORDINATE_NUM_H
