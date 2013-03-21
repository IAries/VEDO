#ifndef _CDSPHERE_2_SPHEREAT_H
#define _CDSPHERE_2_SPHEREAT_H

#include <Common/Interfaces/CDSphere2Sphere.h>

namespace VEDO
{

class CDSphere_SphereAT : public CDSphere_Sphere
{

public:

	void Detect
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

};

};   // namespace VEDO

#endif // _CDSPHERE_2_SPHEREAT_H
