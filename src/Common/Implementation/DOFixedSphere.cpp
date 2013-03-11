#include <Common/Interfaces/DOFixedSphere.h>
#include <cmath>

DOFixedSphere::DOFixedSphere
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
//	double Radius = cpdoml->GetShapeAttributes().sphere.radius;
/*
	DiscreteObject::dVolume = 3.141592654 * Radius*Radius*Radius * 4.0 / 3.0;
	DiscreteObject::dMass
		= DiscreteObject::cpDOModel->GetDensity() * DiscreteObject::dVolume;
	double mmi = 0.4 * dMass * Radius * Radius;
	vMassMomentInertia = NJRvector3d(mmi, mmi, mmi);
*/
	dVolume            = cpDOModel->GetVolume();
	dMass              = cpDOModel->GetMass();
	dSudoMass          = cpDOModel->GetSudoMass();
	vMassMomentInertia = cpDOModel->GetMassMomentInertia();
	pDOStatus->SetRange(cpdoml->GetShapeAttributes().sphere.radius);
};

double DOFixedSphere::CrossAreaToSurface
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
