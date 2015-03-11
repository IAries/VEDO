#ifndef _CALC_DENSITY_H
#define _CALC_DENSITY_H

#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/constants/interfaces/Constants.h>

class CalcDensity
{

public:

	CalcDensity()
	{
	};

	vedo::vedo_float_t computeDensity
		(const vedo::DOWorld* pWorld,
		 vedo::vedo_float_t xmin,
		 vedo::vedo_float_t xmax,
		 vedo::vedo_float_t ymin,
		 vedo::vedo_float_t ymax,
		 vedo::vedo_float_t zmin,
		 vedo::vedo_float_t zmax ) const;

private:

	const static vedo::vedo_int_t RandomHitTestNum;

};

#endif // _CALC_DENSITY_H
