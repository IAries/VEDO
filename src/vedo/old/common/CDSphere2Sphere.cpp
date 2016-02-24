#include <vedo/common/CDSphere2Sphere.h>
#include <vedo/common/DOSphere.h>
#include <aries/utility/Constants.h>

namespace vedo
{

CDSphere_Sphere::CDSphere_Sphere(): ContactDetector()
{
	cInfo.uShapeTypeSlave  = DOShapeType::Sphere;
	cInfo.uShapeTypeMaster = DOShapeType::Sphere;
}

void CDSphere_Sphere::CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Impact std::vector form slave to master
	Vector3df vIm = pdoMaster->GetDOStatus()->GetPosition() - pdoSlave->GetDOStatus()->GetPosition();

	if(this->pBC)
	{
		this->pBC->DifferenceBoundaryConditions(&vIm);
	}

	cInfo.vCenterToCenter = vIm;

	_float_t dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
	_float_t dRb = pdoMaster->GetDOModel()->GetShapeAttributes().sphere.radius;
	cInfo.dImpactDepth = dRa + dRb - vIm.length();

	if(cInfo.dImpactDepth > 0.0)
	{
		_float_t dS = dRa - cInfo.dImpactDepth * dRb / (dRa + dRb);
		cInfo.dOverlapArea = (dRa * dRa - dS * dS) * aries::math::_PI;
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

    cInfo.vImpactPoint
		= pdoSlave->GetDOStatus()->GetPosition()
		+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius - 0.5 * cInfo.dImpactDepth) * cInfo.vImpactDirection;
	if (cInfo.dImpactDepth > 0)
	{
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
