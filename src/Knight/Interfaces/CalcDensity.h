#ifndef _CALC_DENSITY_H
#define _CALC_DENSITY_H

#include <Framework/Interfaces/DOWorld.h>

class CalcDensity
{

public:

	CalcDensity()
	{
	};

	double computeDensity
		(const vedo::DOWorld* pWorld,
		 double xmin,
		 double xmax,
		 double ymin,
		 double ymax,
		 double zmin,
		 double zmax ) const;

private:

	const static int RandomHitTestNum;

};

#endif // _CALC_DENSITY_H
