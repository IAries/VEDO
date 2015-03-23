#ifndef _CDSPHERE_2_SPHEREAT_H
#define _CDSPHERE_2_SPHEREAT_H

#include <vedo/common/CDSphere2Sphere.h>

namespace vedo
{

class CDSphere_SphereAT: public CDSphere_Sphere
{

public:

	void Detect(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

};

}   // namespace vedo

#endif // _CDSPHERE_2_SPHEREAT_H
