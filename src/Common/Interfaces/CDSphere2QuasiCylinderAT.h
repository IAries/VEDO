#ifndef _CDSPHERE_2_QUASICYLINDERAT_H
#define _CDSPHERE_2_QUASICYLINDERAT_H

#include <Common/Interfaces/CDSphere2QuasiCylinder.h>

class CDSphere_QuasiCylinderAT : public CDSphere_QuasiCylinder
{

public:

	void Detect
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

};

#endif // _CDSPHERE_2_QUASICYLINDERAT_H
