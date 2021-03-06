#include <aries/utility/Constants.h>
#include <vedo/common/CDSphere2QuasiCylinder.h>
#include <vedo/common/DOFixedQuasiCylinder.h>
#include <vedo/common/DOSphere.h>
#include <cmath>

namespace vedo
{

CDSphere_QuasiCylinder::CDSphere_QuasiCylinder(): ContactDetector()
{
	cInfo.uShapeTypeSlave  = DOShapeType::Sphere;
	cInfo.uShapeTypeMaster = DOShapeType::QuasiCylinder;
}

void CDSphere_QuasiCylinder::CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Center of Slave (Sphere)
	Vector3df vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Center of Master (QuasiCylinder)
	Vector3df vCb = pdoMaster->GetDOStatus()->GetPosition();

	// Half height of Master
	_float_t dHHb = 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.height;

	// The center axial of Master (QuasiCylinder)
	Vector3df vAxial = pdoMaster->GetDOStatus()->GetOrientationZ();

	// The projected point of vCa on vAxial
	_float_t Dap = (vCa - vCb).dot(vAxial);
	if (Dap < -dHHb)
	{
		Dap = -dHHb;
	}
	else if (Dap > dHHb)
	{
		Dap = dHHb;
	}

    // The distance from vCaps to vCa is the shortest distance between surface of Slave and Master
	Vector3df vCaps = vCb + Dap * vAxial;
	Vector3df vIm   = vCaps - vCa;

	_float_t dRs = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
	_float_t dRc = pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	cInfo.dImpactDepth = dRs + dRc - vIm.length();

	if (cInfo.dImpactDepth > 0.0)
	{
		_float_t dS = dRs - cInfo.dImpactDepth * dRc / (dRs + dRc);
		cInfo.dOverlapArea = (dRs * dRs - dS * dS) * aries::math::_PI;
	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vCenterToCenter  = vIm;
	cInfo.vImpactDirection = vIm.direction();
}

void CDSphere_QuasiCylinder::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_QuasiCylinder::CalDistance(pdoSlave, pdoMaster);

	cInfo.vImpactPoint
		= pdoSlave->GetDOStatus()->GetPosition()
		+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius - 0.5 * cInfo.dImpactDepth) * cInfo.vImpactDirection;
	if (cInfo.dImpactDepth > 0.0)
	{
		cInfo.bUnBalance = (cInfo.bActive == false);
		cInfo.bActive    = true;
	}
	else
	{
		cInfo.bActive    = false;
		cInfo.bUnBalance = false;
	}
}

}   // namespace vedo
