#include <Common/Interfaces/CDQuasiCylinder2QuasiPlate.h>
#include <Common/Interfaces/DOFixedQuasiPlate.h>
#include <Common/Interfaces/DOQuasiCylinder.h>

void CDQuasiCylinder_QuasiPlate::CalDistance
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Do not consider sides of plate here
	// Center of Slave (QuasiCylinder)
	NJRvector3d vCa = pdoSlave->GetDOStatus()->GetPosition();
	double dHHa
		= 0.5*pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.height;
	NJRvector3d vAxiala = pdoSlave->GetDOStatus()->GetOrientationZ();
	NJRvector3d vCan1 = pdoSlave->GetDOStatus()->GetPosition()+dHHa*vAxiala;
	NJRvector3d vCan2 = pdoSlave->GetDOStatus()->GetPosition()-dHHa*vAxiala;

	// Center of Master (QuasiPlate)
	NJRvector3d vCb = pdoMaster->GetDOStatus()->GetPosition();

	// Local axials of Master (QuasiPlate)
	NJRvector3d vOz = pdoMaster->GetDOStatus()->GetOrientationZ();
	//NJRvector3d vOx = pdoMaster->GetDOStatus()->GetOrientationX();
	//NJRvector3d vOy = vOz * vOx;

	// The projection of Ca on Vaxial
	NJRvector3d vCapn1 = (vCan1 - vCb).ProjectOn(vOz);
	NJRvector3d vCapn2 = (vCan2 - vCb).ProjectOn(vOz);
	NJRvector3d vIm, vCaps;

	if (abs(vCapn1.length()-vCapn2.length())<1e-14){	//  if cylinder and plate are parallel
		vCaps = vCa - (vCa - vCb).ProjectOn(vOz);	//  use center of the cylinder
		vIm   = vCaps - vCa;
	}
	else if (vCapn1.length()>vCapn2.length()){	//  end 2 is closer to the plate
		vCaps = vCan2 - vCapn2;	//  use end 2 as a sphere
		vIm   = vCaps - vCan2;
	}
	else{	//  end 1 is closer to the plate
		vCaps = vCan1 - vCapn1;		//  use end 1 as a sphere
		vIm   = vCaps - vCan1;
	}
	
	// Half height of Master
	double dHHb
		= 0.5 * pdoMaster->GetDOModel()->GetShapeAttributes().quasiplate.height;
	
	
	/**************************************************************************
     * The distance from vCaps to vCa is the shortest distance between surface
     * of Slave and Master
	 **************************************************************************/
	
	cInfo.vCenterToCenter  = vIm;
	double dRa = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	cInfo.dImpactDepth = dRa + dHHb - vIm.length();

	if(cInfo.dImpactDepth > 0.0)
	{
		//double dS          = dHHa - cInfo.dImpactDepth;
		//cInfo.dOverlapArea = (dHHa * dHHa - dS * dS) * 3.14159267;
		cInfo.dOverlapArea = 0.0;
		if(pdoMaster->GetDOModel()->GetBehavior()=="bemplate")
		{
			cInfo.vImpactPoint
				= vCaps-(dHHb- cInfo.dImpactDepth)*vIm.direction();
		} else {
			cInfo.vImpactPoint
				= vCaps-(dHHb- 0.5 * cInfo.dImpactDepth)*vIm.direction();
		}
	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vImpactDirection = vIm.direction();
};

void CDQuasiCylinder_QuasiPlate::Detect
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDQuasiCylinder_QuasiPlate::CalDistance(pdoSlave, pdoMaster);

	if (cInfo.dImpactDepth > 0)
	{
	    /*cInfo.vImpactPoint
			= pdoSlave->GetDOStatus()->GetPosition()
			+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius
				- 0.5 * cInfo.dImpactDepth)
			* cInfo.vImpactDirection;*/
		cInfo.bUnBalance = (cInfo.bActive == false);
		cInfo.bActive    = true;
	}
	else
	{
		cInfo.bActive    = false;
		cInfo.bUnBalance = false;
	}
};
