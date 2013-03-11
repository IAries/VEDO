#include <Common/Interfaces/CDQuasiCylinder2QuasiCylinder.h>
#include <Common/Interfaces/DOQuasiCylinder.h>
#include <cmath>

void CDQuasiCylinder_QuasiCylinder::CalDistance
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// Center of Slave (QuasiCylinder)
	NJRvector3d vCs = pdoSlave->GetDOStatus()->GetPosition();

	// Center of Master (QuasiCylinder)
	NJRvector3d vCm = pdoMaster->GetDOStatus()->GetPosition();
	
	// Impact vector form slave to master
	NJRvector3d vW = vCm-vCs;
	// height
	double dHm
		= pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.height;
	double dHs
		= pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.height;

	// The axial direction of QuasiCylinders
	NJRvector3d vAxialm = pdoMaster->GetDOStatus()->GetOrientationZ();
	NJRvector3d vAxials = pdoSlave->GetDOStatus()->GetOrientationZ();

	double b = vAxialm.Dot(vAxials);
	double d = vAxialm.Dot(vW);
	double e = vAxials.Dot(vW);
	double Gm =0.0;
	double Gs = 0.0;

	if (1.0-b*b<1e-14){	// if two cylinders are parallel 
		NJRvector3d n1=vCm+0.5*dHm*vAxialm-vCs;
		NJRvector3d n2=vCm-0.5*dHm*vAxialm-vCs;
		double tempn1 = n1.Dot(vAxials);
		double tempn2 = n2.Dot(vAxials);
		if (tempn1>0.5*dHs)
			tempn1 = 0.5*dHs;
		if (tempn1<-0.5*dHs)
			tempn1 = -0.5*dHs;
		if (tempn2>0.5*dHs)
			tempn2 = 0.5*dHs;
		if (tempn2<-0.5*dHs)
			tempn2 = -0.5*dHs;
		Gs = 0.5*(tempn1+tempn2);
		Gm = -d+b*Gs;
		if (Gm>0.5*dHm)
			Gm=0.5*dHm;
		if (Gm<-0.5*dHm)
			Gm=-0.5*dHm;
	}
	else{	// if two cylinders are not parallel 
		Gm = (d-b*e)/(b*b-1);
		Gs = (b*d-e)/(b*b-1);
		if (Gs>0.5*dHs){
			Gs=0.5*dHs;
			Gm=0.5*dHs*b-d;
			/*if (Gm>0.5*dHm){
				Gm = 0.5*dHm;
				//Gs = 0.5*dHm*b+e;
			}
			if (Gm<-0.5*dHm){
				Gm = -0.5*dHm;
				//Gs = -0.5*dHm*b+e;
			}*/
		}
		if (Gs<-0.5*dHs){
			Gs=-0.5*dHs;
			Gm=-0.5*dHs*b-d;
			/*if (Gm>0.5*dHm){
				Gm = 0.5*dHm;
				//Gs = 0.5*dHm*b+e;
			}
			if (Gm<-0.5*dHm){
				Gm = -0.5*dHm;
				//Gs = -0.5*dHm*b+e;
			}*/
		}
		if (Gm>=0.5*dHm-1e-14){
			Gm = 0.5*dHm;
			Gs = 0.5*dHm*b+e;
			if (Gs>0.5*dHs)
				Gs=0.5*dHs;
			if (Gs<-0.5*dHs)
				Gs=-0.5*dHs;
		}
		if (Gm<=-0.5*dHm+1e-14){
			Gm = -0.5*dHm;
			Gs = -0.5*dHm*b+e;
			if (Gs>0.5*dHs)
				Gs=0.5*dHs;
			if (Gs<-0.5*dHs)
				Gs=-0.5*dHs;
		}
	}
		
	/**************************************************************************
     * The distance from vCaps to vCs is the shortest distance between surface
     * of Slave and Master
	 **************************************************************************/
	NJRvector3d vIm  = vW + Gm*vAxialm - Gs*vAxials;

	double dRs = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	double dRm = pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	cInfo.dImpactDepth = dRs + dRm - vIm.length();

	if(cInfo.dImpactDepth > 0.0)
	{
		//double dS          = dRs - cInfo.dImpactDepth * dRm / (dRs + dRm);
		//cInfo.dOverlapArea = (dRs * dRa - dS * dS) * 3.14159267;
		cInfo.dOverlapArea = 0.0;
		cInfo.vImpactPoint = vCs+Gs*vAxials+(dRs- dRm/(dRs+dRm) * cInfo.dImpactDepth)*vIm.direction();

	}
	else
	{
		cInfo.dOverlapArea = 0.0;
	}

	cInfo.vCenterToCenter  = vIm;
	cInfo.vImpactDirection = vIm.direction();
};

void CDQuasiCylinder_QuasiCylinder::Detect
	(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDQuasiCylinder_QuasiCylinder::CalDistance(pdoSlave, pdoMaster);

	if (cInfo.dImpactDepth > 0.0)
	{
		//cInfo.vImpactPoint
		//	= pdoSlave->GetDOStatus()->GetPosition()
		//	+ (pdoSlave->GetDOModel()->GetShapeAttributes().sphere.radius
		//		- 0.5 * cInfo.dImpactDepth)
		//	* cInfo.vImpactDirection;
		cInfo.bUnBalance = (cInfo.bActive == false);
		cInfo.bActive = true;
	}
	else
	{
		cInfo.bActive    = false;
		cInfo.bUnBalance = false;
	}
};
