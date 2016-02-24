#include <vedo/common/CDSphere2SphereAT.h>

namespace vedo
{

CDSphere_SphereAT::CDSphere_SphereAT(): CDSphere_Sphere()
{
	cInfo.uShapeTypeSlave  = DOShapeType::Sphere;
	cInfo.uShapeTypeMaster = DOShapeType::Sphere;
}

void CDSphere_SphereAT::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_Sphere::CalDistance(pdoSlave, pdoMaster);

	// Center of Slave
	Vector3df vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Radius of Slave
	_float_t dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;

	cInfo.vImpactPoint
		= vCa + (cInfo.vImpactDirection * (dRa - cInfo.dImpactDepth/2.0));
}

}   // namespace vedo
