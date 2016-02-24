#ifndef _CALC_COORDINATE_NUM_H
#define _CALC_COORDINATE_NUM_H

#include <vedo/framework/DOWorld.h>
#include <string>

class CalcCoordinateNum
{

public:

	CalcCoordinateNum()
	{
	};

	vedo::_float_t computeAvgCoordinateNum
		(const vedo::DOWorld* pWorld,
		 vedo::_float_t xmin,
		 vedo::_float_t xmax,
		 vedo::_float_t ymin,
		 vedo::_float_t ymax,
		 vedo::_float_t zmin,
		 vedo::_float_t zmax ) const;

	void outputCoordinateNumVTK
		(const vedo::DOWorld* pWorld, const std::string& filename) const;
};

#endif // _CALC_COORDINATE_NUM_H
