#ifndef _CDSPHERE_2_QUASICYLINDERAT_H
#define _CDSPHERE_2_QUASICYLINDERAT_H

#include <vedo/common/CDSphere2QuasiCylinder.h>

namespace vedo
{

class CDSphere_QuasiCylinderAT: public CDSphere_QuasiCylinder
{

public:

	CDSphere_QuasiCylinderAT();

	void Detect(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "CDSphere_QuasiCylinderAT";
	}

};

}   // namespace vedo

#endif // _CDSPHERE_2_QUASICYLINDERAT_H
