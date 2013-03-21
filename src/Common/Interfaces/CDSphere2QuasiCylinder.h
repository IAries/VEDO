#ifndef _CDSPHERE_2_QUASICYLINDER
#define _CDSPHERE_2_QUASICYLINDER

#include <FrameWork/Interfaces/ContactDetector.h>

namespace VEDO
{

class CDSphere_QuasiCylinder : public ContactDetector
{

public:

	void CalDistance
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	void Detect
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "CDSphere_QuasiCylinder";
	};
};

};   // namespace VEDO

#endif // _CDSPHERE_2_QUASICYLINDER
