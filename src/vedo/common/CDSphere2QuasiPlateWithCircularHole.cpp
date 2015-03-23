#include <aries/utility/Constants.h>
#include <vedo/common/CDSphere2QuasiPlateWithCircularHole.h>
#include <vedo/common/DOQuasiPlateWithCircularHole.h>
#include <vedo/common/DOSphere.h>
#include <cmath>

namespace vedo
{

void CDSphere_QuasiPlateWithCircularHole::CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Center of Slave (Sphere)
	aries::Vector3df vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Center of Master (QuasiPlateWithCircularHole)
	aries::Vector3df vCb = pdoMaster->GetDOStatus()->GetPosition();

	// Local axials of Master (QuasiPlateWithCircularHole)
	aries::Vector3df vOz = pdoMaster->GetDOStatus()->GetOrientationZ();
	aries::Vector3df vOx = pdoMaster->GetDOStatus()->GetOrientationX();
	aries::Vector3df vOy = vOz.cross(vOx);

	// The projection of Ca on Vaxial
	aries::Vector3df vCap = vCa - (vCa - vCb).project_on(vOz);

	// Half height of Master
	_float_t dHHb         = 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplatewithcircularhole.height;
	// Half width of Master
	_float_t dHWb         = 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplatewithcircularhole.width;
	// Half length of Master
	_float_t dHLb         = 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplatewithcircularhole.length;
	// Radius of hole
	_float_t dHoleRadius  = pdoMaster->GetDOModel()->GetShapeAttributes().quasiplatewithcircularhole.holeradius;
	// X-offset of hole¡A·f°t dHWb
	_float_t dHoleXOffset = pdoMaster->GetDOModel()->GetShapeAttributes().quasiplatewithcircularhole.holexoffset;
	// Y-offset of hole¡A·f°t dHLb
	_float_t dHoleYOffset = pdoMaster->GetDOModel()->GetShapeAttributes().quasiplatewithcircularhole.holeyoffset;

	// Center of hole
	aries::Vector3df vCh  = vCb + aries::Vector3df(dHoleXOffset, dHoleYOffset, 0.0);
    aries::Vector3df vCha = vCa - vCh;

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

	_float_t Dahx = vCha.dot(vOx);
	_float_t Dahy = vCha.dot(vOy);

	_float_t Dah  = std::sqrt(Dahx * Dahx + Dahy * Dahy);
    if (Dah < dHoleRadius)
    {
        if (Dah == 0.0)
        {
            Dapx = dHoleXOffset;
            Dapy = dHoleYOffset;
        }
        else
        {
            Dapx = dHoleXOffset + Dahx / Dah * dHoleRadius;
            Dapy = dHoleYOffset + Dahy / Dah * dHoleRadius;
        }
    }

    // The distance from vCaps to vCa is the shortest distance between surface of Slave and Master
	aries::Vector3df vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
	aries::Vector3df vIm = vCaps - vCa;
	cInfo.vCenterToCenter = vIm;

	_float_t dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
    if (Dah == 0.0)
	{
		cInfo.dImpactDepth = dRa + dHHb - std::sqrt(vIm.length() * vIm.length() + dHoleRadius * dHoleRadius);
	}
    else
    {
        cInfo.dImpactDepth = dRa + dHHb - vIm.length();
    }

	if (cInfo.dImpactDepth > 0.0)
	{
		_float_t dS = dRa - cInfo.dImpactDepth;
		cInfo.dOverlapArea = (dRa * dRa - dS * dS) * aries::fPI;
	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vImpactDirection = vIm.direction();
}

void CDSphere_QuasiPlateWithCircularHole::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_QuasiPlateWithCircularHole::CalDistance(pdoSlave, pdoMaster);

	if (cInfo.dImpactDepth > 0)
	{
	    cInfo.vImpactPoint
			= pdoSlave->GetDOStatus()->GetPosition()
			+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius - 0.5 * cInfo.dImpactDepth) * cInfo.vImpactDirection;
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
