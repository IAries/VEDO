#ifndef _QUASICYLINDER_2_QUASICYLINDER
#define _QUASICYLINDER_2_QUASICYLINDER

#include <vedo/framework/interfaces/ContactDetector.h>

namespace vedo
{

class CDQuasiCylinder_QuasiCylinder : public ContactDetector
{

public:

	void CalDistance(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	void Detect(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "QuasiCylinder_QuasiCylinder";
	}
};

}   // namespace vedo

#endif // _QUASICYLINDER_2_QUASICYLINDER
