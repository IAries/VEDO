#include <aries/utility/Constants.h>
#include <vedo/common/DOFixedSphere.h>
#include <cmath>

namespace vedo
{

DOFixedSphere::DOFixedSphere(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
//	_float_t Radius = cpdoml->GetShapeAttributes().sphere.radius;
/*
	_float_t mmi   = 0.4 * dMass * Radius * Radius;
	vMassMomentInertia = aries::Vector3df(mmi, mmi, mmi);
*/
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = cpdoml->GetMassMomentInertia();
}

_float_t DOFixedSphere::CrossAreaToSurface(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const
{
	//Surface: ax+by+cz=d
	aries::Vector3df vSurfaceNormal(a, b, c);
	aries::Vector3df p               = pDOStatus->GetPosition();
	_float_t  r               = cpDOModel->GetShapeAttributes().sphere.radius;

	_float_t  dSphere2Surface = fabs(d-(p.dot(vSurfaceNormal))/vSurfaceNormal.length());

	if (dSphere2Surface >= r)
	{
		return 0.0;
	}
	else
	{
		return (r * r - dSphere2Surface * dSphere2Surface) * aries::fPI;
	}
}

}   // namespace vedo
