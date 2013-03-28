#include <Common/Interfaces/CDSphere2QuasiCylinderAT.h>

namespace VEDO
{

void CDSphere_QuasiCylinderAT::Detect
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_QuasiCylinder::CalDistance(pdoSlave, pdoMaster);

	NJR::Vector3d vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Radius of Slave
	double dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;

	cInfo.vImpactPoint
		= vCa + (cInfo.vImpactDirection * (dRa - cInfo.dImpactDepth/2.0));
};

};   // namespace VEDO
