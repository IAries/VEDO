#ifndef _CDSPHERE_2_SPHEREAT_H
#define _CDSPHERE_2_SPHEREAT_H

#include <Common/Interfaces/CDSphere2Sphere.h>

class CDSphere_SphereAT : public CDSphere_Sphere
{

public:

	void Detect
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

};

#endif // _CDSPHERE_2_SPHEREAT_H
