#ifndef _CDSPHERE_2_QUASIPLATE_H
#define _CDSPHERE_2_QUASIPLATE_H

#include <vedo/framework/ContactDetector.h>

namespace vedo
{

class CDSphere_QuasiPlate: public ContactDetector
{

public:

	CDSphere_QuasiPlate();

	void CalDistance(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	void Detect(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "CDSphere_QuasiPlate";
	}
};

}   // namespace vedo

#endif // _CDSPHERE_2_QUASIPLATE_H
