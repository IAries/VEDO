#ifndef _CDQUASICYLINDER_2_QUASICYLINDER
#define _CDQUASICYLINDER_2_QUASICYLINDER

#include <FrameWork/Interfaces/ContactDetector.h>

class CDQuasiCylinder_QuasiCylinder : public ContactDetector
{

public:

	void CalDistance
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	void Detect
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "CDQuasiCylinder_QuasiCylinder";
	};
};

#endif // _CDQUASICYLINDER_2_QUASICYLINDER
