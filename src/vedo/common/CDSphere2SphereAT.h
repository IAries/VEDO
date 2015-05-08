#ifndef _CDSPHERE_2_SPHEREAT_H
#define _CDSPHERE_2_SPHEREAT_H

#include <vedo/common/CDSphere2Sphere.h>

namespace vedo
{

class CDSphere_SphereAT: public CDSphere_Sphere
{

public:

	CDSphere_SphereAT();

	void Detect(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "CDSphere_SphereAT";
	}
};

}   // namespace vedo

#endif // _CDSPHERE_2_SPHEREAT_H
