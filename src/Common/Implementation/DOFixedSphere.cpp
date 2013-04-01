#include <FrameWork/Interfaces/Constants.h>
#include <Common/Interfaces/DOFixedSphere.h>
#include <cmath>

namespace vedo
{

DOFixedSphere::DOFixedSphere
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

double DOFixedSphere::CrossAreaToSurface
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
