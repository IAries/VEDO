#include <vedo/njr/interfaces/Constants.h>
#include <vedo/common/interfaces/CDSphere2QuasiCylinder.h>
#include <vedo/common/interfaces/DOFixedQuasiCylinder.h>
#include <vedo/common/interfaces/DOSphere.h>
#include <cmath>

namespace vedo
{

void CDSphere_QuasiCylinder::CalDistance
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Center of Slave (Sphere)
	njr::Vector3d vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Center of Master (QuasiCylinder)
	njr::Vector3d vCb = pdoMaster->GetDOStatus()->GetPosition();

	// Half height of Master
	double dHHb
		= 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.height;

	// The center axial of Master (QuasiCylinder)
	njr::Vector3d vAxial = pdoMaster->GetDOStatus()->GetOrientationZ();

	// The projected point of vCa on vAxial
	double Dap = (vCa - vCb) % vAxial;
	if (Dap < -dHHb)
	{
		Dap = -dHHb;
	}
	else if ( Dap > dHHb)
	{
		Dap = dHHb;
	}

	/**************************************************************************
     * The distance from vCaps to vCa is the shortest distance between surface
     * of Slave and Master
	 **************************************************************************/
	njr::Vector3d vCaps = vCb + Dap * vAxial;
	njr::Vector3d vIm   = vCaps - vCa;

	double dRs = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
	double dRc = pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	cInfo.dImpactDepth = dRs + dRc - vIm.length();

	if(cInfo.dImpactDepth > 0.0)
	{
		double dS          = dRs - cInfo.dImpactDepth * dRc / (dRs + dRc);
		cInfo.dOverlapArea = (dRs * dRs - dS * dS) * njr::dPI;
	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vCenterToCenter  = vIm;
	cInfo.vImpactDirection = vIm.direction();
};

void CDSphere_QuasiCylinder::Detect
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_QuasiCylinder::CalDistance(pdoSlave, pdoMaster);

	if (cInfo.dImpactDepth > 0.0)
	{
		cInfo.vImpactPoint
			= pdoSlave->GetDOStatus()->GetPosition()
			+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius
				- 0.5 * cInfo.dImpactDepth)
			* cInfo.vImpactDirection;
		cInfo.bUnBalance = (cInfo.bActive == false);
		cInfo.bActive = true;
	}
	else
	{
		cInfo.bActive    = false;
		cInfo.bUnBalance = false;
	}
};

};   // namespace vedo
