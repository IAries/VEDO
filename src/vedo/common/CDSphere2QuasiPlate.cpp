#include <aries/utility/Constants.h>
#include <vedo/common/CDSphere2QuasiPlate.h>
#include <vedo/common/DOFixedQuasiPlate.h>
#include <vedo/common/DOSphere.h>

namespace vedo
{

CDSphere_QuasiPlate::CDSphere_QuasiPlate(): ContactDetector()
{
	cInfo.uShapeTypeSlave  = DOShapeType::Sphere;
	cInfo.uShapeTypeMaster = DOShapeType::QuasiPlate;
}

void CDSphere_QuasiPlate::CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Center of Slave (Sphere)
	Vector3df vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Center of Master (QuasiPlate)
	Vector3df vCb = pdoMaster->GetDOStatus()->GetPosition();

	// Local axials of Master (QuasiPlate)
	Vector3df vOz = pdoMaster->GetDOStatus()->GetOrientationZ();
	Vector3df vOx = pdoMaster->GetDOStatus()->GetOrientationX();
	Vector3df vOy = vOz.cross(vOx);

	// The projection of Ca on Vaxial
	Vector3df vCap = vCa - (vCa - vCb).project_on(vOz);

	// Half height of Master
	_float_t dHHb = 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplate.height;
	// Half width of Master
	_float_t dHWb = 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplate.width;
	// Half length of Master
	_float_t dHLb = 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplate.length;

	_float_t Dapx = (vCap - vCb).dot(vOx);

	if (Dapx < -dHWb)
	{
		Dapx = -dHWb;
	}
	else if (Dapx > dHWb)
	{
		Dapx = dHWb;
	}

	_float_t Dapy = (vCap - vCb).dot(vOy);
	if (Dapy < -dHLb)
	{
		Dapy = -dHLb;
	}
	else if (Dapy > dHLb)
	{
		Dapy = dHLb;
	}

    // The distance from vCaps to vCa is the shortest distance between surface of Slave and Master
	Vector3df vCaps    = (vOx * Dapx) + (vOy * Dapy) + vCb;
	Vector3df vIm      = vCaps - vCa;
	cInfo.vCenterToCenter  = vIm;

	_float_t dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
	cInfo.dImpactDepth = dRa + dHHb - vIm.length();

	if (cInfo.dImpactDepth > 0.0)
	{
		_float_t dS = dRa - cInfo.dImpactDepth;
		cInfo.dOverlapArea = (dRa * dRa - dS * dS) * aries::math::_PI;
	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vImpactDirection = vIm.direction();
}

void CDSphere_QuasiPlate::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_QuasiPlate::CalDistance(pdoSlave, pdoMaster);

    cInfo.vImpactPoint
		= pdoSlave->GetDOStatus()->GetPosition()
		+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius - 0.5 * cInfo.dImpactDepth) * cInfo.vImpactDirection;
	if (cInfo.dImpactDepth > 0)
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
