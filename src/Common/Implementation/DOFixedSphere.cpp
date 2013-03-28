#include <FrameWork/Interfaces/Constants.h>
#include <Common/Interfaces/DOFixedSphere.h>
#include <cmath>

namespace VEDO
{

DOFixedSphere::DOFixedSphere
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

double DOFixedSphere::CrossAreaToSurface
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
