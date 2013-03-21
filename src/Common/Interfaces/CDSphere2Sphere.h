#ifndef _CDSPHERE_2_SPHERE_H
#define _CDSPHERE_2_SPHERE_H

#include <FrameWork/Interfaces/ContactDetector.h>

namespace VEDO
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

};   // namespace VEDO

#endif // _CDSPHERE_2_SPHERE_H
