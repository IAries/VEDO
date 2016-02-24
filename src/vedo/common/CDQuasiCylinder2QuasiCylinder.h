#ifndef _QUASICYLINDER_2_QUASICYLINDER
#define _QUASICYLINDER_2_QUASICYLINDER

#include <vedo/framework/ContactDetector.h>

namespace vedo
{

class CDQuasiCylinder_QuasiCylinder : public ContactDetector
{

public:

	CDQuasiCylinder_QuasiCylinder();

	void CalDistance(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	void Detect(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "QuasiCylinder_QuasiCylinder";
	}
};

}   // namespace vedo

#endif // _QUASICYLINDER_2_QUASICYLINDER
