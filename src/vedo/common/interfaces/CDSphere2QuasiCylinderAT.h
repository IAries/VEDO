#ifndef _CDSPHERE_2_QUASICYLINDERAT_H
#define _CDSPHERE_2_QUASICYLINDERAT_H

#include <vedo/common/interfaces/CDSphere2QuasiCylinder.h>

namespace vedo
{

class CDSphere_QuasiCylinderAT : public CDSphere_QuasiCylinder
{

public:

	void Detect
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

};

};   // namespace vedo

#endif // _CDSPHERE_2_QUASICYLINDERAT_H
