#include <FrameWork/Interfaces/Constants.h>
#include <Common/Interfaces/DOSphere.h>
#include <cmath>

namespace VEDO
{

DOSphere::DOSphere
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
//	double Radius = cpdoml->GetShapeAttributes().sphere.radius;
/*
	double mmi = 0.4 * dMass * Radius * Radius;
	vMassMomentInertia = NJR::Vector3d(mmi, mmi, mmi);
*/
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = cpdoml->GetMassMomentInertia();
};

void DOSphere::Response(double dt)
{
  		NJR::Vector3d V      = pDOStatus->GetVelocity();
		NJR::Vector3d AV     = pDOStatus->GetAngularVelocity();
		NJR::Vector3d P      = pDOStatus->GetPosition();
		NJR::Vector3d Ox     = pDOStatus->GetOrientationX();
		NJR::Vector3d Oz     = pDOStatus->GetOrientationZ();
		double      Radius = cpDOModel->GetShapeAttributes().sphere.radius;

        NJR::Vector3d dv     = 1.0 / dSudoMass * vImpact;
//		NJR::Vector3d dav    = (1.0/(0.4*dSudoMass*Radius*Radius)) * vAngularImpact;
		NJR::Vector3d dav    = 2.5 / dSudoMass / Radius / Radius * vAngularImpact;
		NJR::Vector3d dp     = dt * (V  + (0.5 * dv ));
		NJR::Vector3d dw     = dt * (AV + (0.5 * dav));

		pDOStatus->SetPosition(P + dp);
		pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
		pDOStatus->SetVelocity(V + dv);
		pDOStatus->SetAngularVelocity(AV + dav);
		ClearImpact();
};

double DOSphere::CrossAreaToSurface
	(double& a, double& b, double& c, double& d) const
{
	//Surface: ax+by+cz=d
	NJR::Vector3d vSurfaceNormal(a, b, c);
	NJR::Vector3d p = pDOStatus->GetPosition();
	double r = cpDOModel->GetShapeAttributes().sphere.radius;

	double dSphere2Surface = fabs(d-(p%vSurfaceNormal)/vSurfaceNormal.length());

	if(dSphere2Surface >= r)
	{
		return 0.0;
	}
	else
	{
		return (r * r - dSphere2Surface * dSphere2Surface) * NJR::dPI;
	}
};

};   // namespace VEDO
