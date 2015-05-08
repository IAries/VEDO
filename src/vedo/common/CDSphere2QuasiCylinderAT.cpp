#include <vedo/common/CDSphere2QuasiCylinderAT.h>

namespace vedo
{

CDSphere_QuasiCylinderAT::CDSphere_QuasiCylinderAT(): CDSphere_QuasiCylinder()
{
	cInfo.uShapeTypeSlave  = DOShapeType::Sphere;
	cInfo.uShapeTypeMaster = DOShapeType::QuasiCylinder;
}

void CDSphere_QuasiCylinderAT::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_QuasiCylinder::CalDistance(pdoSlave, pdoMaster);

	Vector3df vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Radius of Slave
	_float_t dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;

	cInfo.vImpactPoint = vCa + (cInfo.vImpactDirection * (dRa - cInfo.dImpactDepth/2.0));
}

}   // namespace vedo
