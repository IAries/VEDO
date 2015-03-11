#include <vedo/common/interfaces/CDSphere2Sphere.h>
#include <vedo/common/interfaces/DOSphere.h>
#include <vedo/njr/interfaces/Constants.h>

namespace vedo
{

void CDSphere_Sphere::CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Impact std::vector form slave to master
	njr::Vector3d vIm = pdoMaster->GetDOStatus()->GetPosition() - pdoSlave->GetDOStatus()->GetPosition();

	if(this->pBC)
	{
		this->pBC->DifferenceBoundaryConditions(&vIm);
	}

	cInfo.vCenterToCenter = vIm;

	vedo_float_t dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
	vedo_float_t dRb = pdoMaster->GetDOModel()->GetShapeAttributes().sphere.radius;
	cInfo.dImpactDepth = dRa + dRb - vIm.length();

	if(cInfo.dImpactDepth > 0.0)
	{
		vedo_float_t dS = dRa - cInfo.dImpactDepth * dRb / (dRa + dRb);
		cInfo.dOverlapArea = (dRa * dRa - dS * dS) * njr::dPI;
	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vImpactDirection = vIm.direction();
}

void CDSphere_Sphere::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_Sphere::CalDistance(pdoSlave, pdoMaster);

	if (cInfo.dImpactDepth > 0)
	{
	    cInfo.vImpactPoint
			= pdoSlave->GetDOStatus()->GetPosition()
			+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius - 0.5 * cInfo.dImpactDepth) * cInfo.vImpactDirection;
		cInfo.bUnBalance = (cInfo.bActive == false);
		cInfo.bActive = true;
	}
	else
	{
		cInfo.bActive    = false;
		cInfo.bUnBalance = false;
	}
}

}   // namespace vedo
