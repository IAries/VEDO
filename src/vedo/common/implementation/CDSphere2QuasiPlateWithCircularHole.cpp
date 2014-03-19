#include <vedo/njr/interfaces/Constants.h>
#include <vedo/common/interfaces/CDSphere2QuasiPlateWithCircularHole.h>
#include <vedo/common/interfaces/DOQuasiPlateWithCircularHole.h>
#include <vedo/common/interfaces/DOSphere.h>
#include <cmath>

namespace vedo
{

void CDSphere_QuasiPlateWithCircularHole::CalDistance
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Center of Slave (Sphere)
	njr::Vector3d vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Center of Master (QuasiPlateWithCircularHole)
	njr::Vector3d vCb = pdoMaster->GetDOStatus()->GetPosition();

	// Local axials of Master (QuasiPlateWithCircularHole)
	njr::Vector3d vOz = pdoMaster->GetDOStatus()->GetOrientationZ();
	njr::Vector3d vOx = pdoMaster->GetDOStatus()->GetOrientationX();
	njr::Vector3d vOy = vOz * vOx;

	// The projection of Ca on Vaxial
	njr::Vector3d vCap = vCa - (vCa - vCb).ProjectOn(vOz);

	// Half height of Master
	double dHHb
		= 0.5
		* pdoMaster
			->GetDOModel()
				->GetShapeAttributes().quasiplatewithcircularhole.height;
	// Half width of Master
	double dHWb
		= 0.5
		* pdoMaster
			->GetDOModel()
				->GetShapeAttributes().quasiplatewithcircularhole.width;
	// Half length of Master
	double dHLb
		= 0.5
		* pdoMaster
			->GetDOModel()
				->GetShapeAttributes().quasiplatewithcircularhole.length;
    // Radius of hole
	double dHoleRadius
		= pdoMaster
			->GetDOModel()
				->GetShapeAttributes().quasiplatewithcircularhole.holeradius;

    // X-offset of hole¡A·f°t dHWb
	double dHoleXOffset
		= pdoMaster
			->GetDOModel()
				->GetShapeAttributes().quasiplatewithcircularhole.holexoffset;

    // Y-offset of hole¡A·f°t dHLb
	double dHoleYOffset
		= pdoMaster
			->GetDOModel()
				->GetShapeAttributes().quasiplatewithcircularhole.holeyoffset;

	// Center of hole
	njr::Vector3d vCh  = vCb + njr::Vector3d(dHoleXOffset, dHoleYOffset, 0.0);
    njr::Vector3d vCha = vCa - vCh;

	double Dapx = (vCap - vCb).Dot(vOx);
	if (Dapx < -dHWb)
	{
		Dapx = -dHWb;
	}
	else if (Dapx > dHWb)
	{
		Dapx = dHWb;
	}

	double Dapy = (vCap - vCb).Dot(vOy);
	if (Dapy < -dHLb)
	{
		Dapy = -dHLb;
	}
	else if (Dapy > dHLb)
	{
		Dapy = dHLb;
	}

	double Dahx = vCha.Dot(vOx);
	double Dahy = vCha.Dot(vOy);

	double Dah  = std::sqrt(Dahx * Dahx + Dahy * Dahy);
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

	/**************************************************************************
     * The distance from vCaps to vCa is the shortest distance between surface
     * of Slave and Master
	 **************************************************************************/
	njr::Vector3d vCaps    = (vOx * Dapx) + (vOy * Dapy) + vCb;
	njr::Vector3d vIm      = vCaps - vCa;
	cInfo.vCenterToCenter  = vIm;

	double dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
    if (Dah == 0.0)
	{
		cInfo.dImpactDepth
			= dRa + dHHb
			- std::sqrt
				(vIm.length() * vIm.length() + dHoleRadius * dHoleRadius);
	}
    else
    {
        cInfo.dImpactDepth = dRa + dHHb - vIm.length();
    }

	if(cInfo.dImpactDepth > 0.0)
	{
		double dS          = dRa - cInfo.dImpactDepth;
		cInfo.dOverlapArea = (dRa * dRa - dS * dS) * njr::dPI;
	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vImpactDirection = vIm.direction();
};

void CDSphere_QuasiPlateWithCircularHole::Detect
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_QuasiPlateWithCircularHole::CalDistance(pdoSlave, pdoMaster);

	if (cInfo.dImpactDepth > 0)
	{
	    cInfo.vImpactPoint
			= pdoSlave->GetDOStatus()->GetPosition()
			+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius
				- 0.5 * cInfo.dImpactDepth)
			* cInfo.vImpactDirection;
		cInfo.bUnBalance = (cInfo.bActive == false);
		cInfo.bActive    = true;
	}
	else
	{
		cInfo.bActive    = false;
		cInfo.bUnBalance = false;
	}
};

};   // namespace vedo
