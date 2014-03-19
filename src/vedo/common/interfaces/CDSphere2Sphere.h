#ifndef _CDSPHERE_2_SPHERE_H
#define _CDSPHERE_2_SPHERE_H

#include <vedo/framework/interfaces/ContactDetector.h>

namespace vedo
{

class CDSphere_Sphere : public ContactDetector
{

public:

	void CalDistance
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	void Detect
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "CDSphere_Sphere";
	};
};

};   // namespace vedo

#endif // _CDSPHERE_2_SPHERE_H
