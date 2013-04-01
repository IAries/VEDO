#ifndef _CALC_COORDINATE_NUM_H
#define _CALC_COORDINATE_NUM_H

#include <FrameWork/Interfaces/DOWorld.h>
#include <string>

class CalcCoordinateNum
{

public:

	CalcCoordinateNum()
	{
	};

	double computeAvgCoordinateNum
		(const vedo::DOWorld* pWorld,
		 double xmin,
		 double xmax,
		 double ymin,
		 double ymax,
		 double zmin,
		 double zmax ) const;

	void outputCoordinateNumVTK
		(const vedo::DOWorld* pWorld, const std::string& filename) const;
};

#endif // _CALC_COORDINATE_NUM_H
