#include <vedo/njr/interfaces/Constants.h>
#include <vedo/common/interfaces/CDSphere2QuasiPlate.h>
#include <vedo/common/interfaces/DOFixedQuasiPlate.h>
#include <vedo/common/interfaces/DOSphere.h>

namespace vedo
{

void CDSphere_QuasiPlate::CalDistance
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Center of Slave (Sphere)
	njr::Vector3d vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Center of Master (QuasiPlate)
	njr::Vector3d vCb = pdoMaster->GetDOStatus()->GetPosition();

	// Local axials of Master (QuasiPlate)
	njr::Vector3d vOz = pdoMaster->GetDOStatus()->GetOrientationZ();
	njr::Vector3d vOx = pdoMaster->GetDOStatus()->GetOrientationX();
	njr::Vector3d vOy = vOz * vOx;

	// The projection of Ca on Vaxial
	njr::Vector3d vCap = vCa - (vCa - vCb).ProjectOn(vOz);

	// Half height of Master
	double dHHb
		= 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplate.height;
	// Half width of Master
	double dHWb
		= 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplate.width;
	// Half length of Master
	double dHLb
		= 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplate.length;

	double Dapx = (vCap - vCb) % vOx;

	if ( Dapx < -dHWb)
	{
		Dapx = -dHWb;
	}
	else if ( Dapx > dHWb)
	{
		Dapx = dHWb;
	}

	double Dapy = (vCap - vCb) % vOy;
	if ( Dapy < -dHLb)
	{
		Dapy = -dHLb;
	}
	else if (Dapy > dHLb)
	{
		Dapy = dHLb;
	}

	/**************************************************************************
     * The distance from vCaps to vCa is the shortest distance between surface
     * of Slave and Master
	 **************************************************************************/
	njr::Vector3d vCaps    = (vOx * Dapx) + (vOy * Dapy) + vCb;
	njr::Vector3d vIm      = vCaps - vCa;
	cInfo.vCenterToCenter  = vIm;

	double dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
	cInfo.dImpactDepth = dRa + dHHb - vIm.length();

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

void CDSphere_QuasiPlate::Detect
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_QuasiPlate::CalDistance(pdoSlave, pdoMaster);

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
