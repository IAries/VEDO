#include <Framework/Interfaces/Constants.h>
#include <Common/Interfaces/DOSphere.h>
#include <cmath>

namespace vedo
{

DOSphere::DOSphere
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
//	double Radius = cpdoml->GetShapeAttributes().sphere.radius;
/*
	double mmi = 0.4 * dMass * Radius * Radius;
	vMassMomentInertia = njr::Vector3d(mmi, mmi, mmi);
*/
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = cpdoml->GetMassMomentInertia();
};

void DOSphere::Response(double dt)
{
  		njr::Vector3d V      = pDOStatus->GetVelocity();
		njr::Vector3d AV     = pDOStatus->GetAngularVelocity();
		njr::Vector3d P      = pDOStatus->GetPosition();
		njr::Vector3d Ox     = pDOStatus->GetOrientationX();
		njr::Vector3d Oz     = pDOStatus->GetOrientationZ();
		double      Radius = cpDOModel->GetShapeAttributes().sphere.radius;

		njr::Vector3d dv     = 1.0 / dSudoMass * vImpact;
//		njr::Vector3d dav    = (1.0/(0.4*dSudoMass*Radius*Radius)) * vAngularImpact;
		njr::Vector3d dav    = 2.5 / dSudoMass / Radius / Radius * vAngularImpact;
		njr::Vector3d dp     = dt * (V  + (0.5 * dv ));
		njr::Vector3d dw     = dt * (AV + (0.5 * dav));

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
	njr::Vector3d vSurfaceNormal(a, b, c);
	njr::Vector3d p = pDOStatus->GetPosition();
	double r = cpDOModel->GetShapeAttributes().sphere.radius;

	double dSphere2Surface = fabs(d-(p%vSurfaceNormal)/vSurfaceNormal.length());

	if(dSphere2Surface >= r)
	{
		return 0.0;
	}
	else
	{
		return (r * r - dSphere2Surface * dSphere2Surface) * njr::dPI;
	}
};

};   // namespace vedo
