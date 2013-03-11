#include <Common/Interfaces/DOSphere.h>
#include <cmath>

using namespace std;

DOSphere::DOSphere
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
//	double Radius = cpdoml->GetShapeAttributes().sphere.radius;
/*
	double mmi = 0.4 * dMass * Radius * Radius;
	vMassMomentInertia = NJRvector3d(mmi, mmi, mmi);
*/
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = cpdoml->GetMassMomentInertia();
	pDOStatus->SetRange(cpdoml->GetShapeAttributes().sphere.radius);
};

void DOSphere::Response(double dt)
{
  		NJRvector3d V      = pDOStatus->GetVelocity();
		NJRvector3d AV     = pDOStatus->GetAngularVelocity();
		NJRvector3d P      = pDOStatus->GetPosition();
		NJRvector3d Ox     = pDOStatus->GetOrientationX();
		NJRvector3d Oz     = pDOStatus->GetOrientationZ();
		double      Radius = cpDOModel->GetShapeAttributes().sphere.radius;

        NJRvector3d dv     = 1.0 / dSudoMass * vImpact;
//		NJRvector3d dav    = (1.0/(0.4*dSudoMass*Radius*Radius)) * vAngularImpact;
		NJRvector3d dav    = 2.5 / dSudoMass / Radius / Radius * vAngularImpact;
		NJRvector3d dp     = dt * (V  + (0.5 * dv ));
		NJRvector3d dw     = dt * (AV + (0.5 * dav));

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
	NJRvector3d vSurfaceNormal(a, b, c);
	NJRvector3d p = pDOStatus->GetPosition();
	double r = cpDOModel->GetShapeAttributes().sphere.radius;

	double dSphere2Surface = fabs(d-(p%vSurfaceNormal)/vSurfaceNormal.length());

	if(dSphere2Surface >= r)
	{
		return 0.0;
	}
	else
	{
		return (r * r - dSphere2Surface * dSphere2Surface) * 3.14159267;
	}
};
