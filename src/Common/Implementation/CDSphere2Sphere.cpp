#include <Common/Interfaces/CDSphere2Sphere.h>
#include <Common/Interfaces/DOSphere.h>

using namespace std;

void CDSphere_Sphere::CalDistance
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Impact vector form slave to master
	NJRvector3d vIm
		= pdoMaster->GetDOStatus()->GetPosition()
		- pdoSlave->GetDOStatus()->GetPosition();

	if(this->pBC)
		this->pBC->DifferenceBoundaryConditions(&vIm);

	cInfo.vCenterToCenter = vIm;

	double dRa = pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius;
	double dRb = pdoMaster->GetDOModel()->GetShapeAttributes().sphere.radius;
	cInfo.dImpactDepth = dRa + dRb - vIm.length();

	if(cInfo.dImpactDepth > 0.0)
	{
		double dS          = dRa - cInfo.dImpactDepth * dRb / (dRa + dRb);
		cInfo.dOverlapArea = (dRa * dRa - dS * dS) * 3.14159267;
	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vImpactDirection = vIm.direction();
};

void CDSphere_Sphere::Detect
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDSphere_Sphere::CalDistance(pdoSlave, pdoMaster);

	if (cInfo.dImpactDepth > 0)
	{
	    cInfo.vImpactPoint
			= pdoSlave->GetDOStatus()->GetPosition()
			+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius
				- 0.5 * cInfo.dImpactDepth                               )
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
