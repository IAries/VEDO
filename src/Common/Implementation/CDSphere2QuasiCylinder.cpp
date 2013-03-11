#include <Common/Interfaces/CDSphere2QuasiCylinder.h>
#include <Common/Interfaces/DOFixedQuasiCylinder.h>
#include <Common/Interfaces/DOSphere.h>
#include <cmath>

void CDSphere_QuasiCylinder::CalDistance
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Center of Slave (Sphere)
	NJRvector3d vCa = pdoSlave->GetDOStatus()->GetPosition();

	// Center of Master (QuasiCylinder)
	NJRvector3d vCb = pdoMaster->GetDOStatus()->GetPosition();

	// Half height of Master
	double dHHb
		= 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.height;

	// The center axial of Master (QuasiCylinder)
	NJRvector3d vAxial = pdoMaster->GetDOStatus()->GetOrientationZ();

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
	NJRvector3d vCaps = vCb + Dap * vAxial;
	NJRvector3d vIm   = vCaps - vCa;

	double dRs = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
	double dRc = pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	cInfo.dImpactDepth = dRs + dRc - vIm.length();

	if(cInfo.dImpactDepth > 0.0)
	{
		double dS          = dRs - cInfo.dImpactDepth * dRc / (dRs + dRc);
		cInfo.dOverlapArea = (dRs * dRs - dS * dS) * 3.14159267;
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
