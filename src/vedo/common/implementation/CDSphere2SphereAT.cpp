#include <vedo/common/interfaces/CDSphere2SphereAT.h>

namespace vedo
{

void CDSphere_SphereAT::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_Sphere::CalDistance(pdoSlave, pdoMaster);

	// Center of Slave
	njr::Vector3d vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Radius of Slave
	vedo_float_t dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;

	cInfo.vImpactPoint
		= vCa + (cInfo.vImpactDirection * (dRa - cInfo.dImpactDepth/2.0));
}

}   // namespace vedo
