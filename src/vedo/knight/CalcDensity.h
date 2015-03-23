#ifndef _CALC_DENSITY_H
#define _CALC_DENSITY_H

#include <vedo/framework/DOWorld.h>
#include <aries/utility/Constants.h>

class CalcDensity
{

public:

	CalcDensity()
	{
	};

	vedo::_float_t computeDensity
		(const vedo::DOWorld* pWorld,
		 vedo::_float_t xmin,
		 vedo::_float_t xmax,
		 vedo::_float_t ymin,
		 vedo::_float_t ymax,
		 vedo::_float_t zmin,
		 vedo::_float_t zmax ) const;

private:

	const static vedo::_uint_t RandomHitTestNum;

};

#endif // _CALC_DENSITY_H
