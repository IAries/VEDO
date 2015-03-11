#include <vedo/njr/interfaces/Constants.h>
#include <vedo/common/interfaces/DOFixedSphere.h>
#include <cmath>

namespace vedo
{

DOFixedSphere::DOFixedSphere(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
//	vedo_float_t Radius = cpdoml->GetShapeAttributes().sphere.radius;
/*
	vedo_float_t mmi   = 0.4 * dMass * Radius * Radius;
	vMassMomentInertia = njr::Vector3d(mmi, mmi, mmi);
*/
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = cpdoml->GetMassMomentInertia();
}

vedo_float_t DOFixedSphere::CrossAreaToSurface(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const
{
	//Surface: ax+by+cz=d
	njr::Vector3d vSurfaceNormal(a, b, c);
	njr::Vector3d p               = pDOStatus->GetPosition();
	vedo_float_t  r               = cpDOModel->GetShapeAttributes().sphere.radius;

	vedo_float_t  dSphere2Surface = fabs(d-(p%vSurfaceNormal)/vSurfaceNormal.length());

	if (dSphere2Surface >= r)
	{
		return 0.0;
	}
	else
	{
		return (r * r - dSphere2Surface * dSphere2Surface) * njr::dPI;
	}
}

}   // namespace vedo
