#include <aries/utility/Constants.h>
#include <vedo/common/DOSphere.h>
#include <cmath>

namespace vedo
{

DOSphere::DOSphere(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
//	_float_t Radius = cpdoml->GetShapeAttributes().sphere.radius;
/*
	_float_t mmi = 0.4 * dMass * Radius * Radius;
	vMassMomentInertia = aries::Vector3df(mmi, mmi, mmi);
*/
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = cpdoml->GetMassMomentInertia();
}

void DOSphere::Response(_float_t dt)
{
	aries::Vector3df V      = pDOStatus->GetVelocity();
	aries::Vector3df AV     = pDOStatus->GetAngularVelocity();
	aries::Vector3df P      = pDOStatus->GetPosition();
	aries::Vector3df Ox     = pDOStatus->GetOrientationX();
	aries::Vector3df Oz     = pDOStatus->GetOrientationZ();
	_float_t  Radius = cpDOModel->GetShapeAttributes().sphere.radius;

	aries::Vector3df dv     = 1.0 / dSudoMass * vImpact;
/*
	aries::Vector3df
		dav
			(vAngularImpact.x() / vMassMomentInertia.x(),
			 vAngularImpact.y() / vMassMomentInertia.y(),
			 vAngularImpact.z() / vMassMomentInertia.z() );
	aries::Vector3df dav    = (1.0/(0.4*dSudoMass*Radius*Radius)) * vAngularImpact;
*/
	aries::Vector3df dav    = 2.5 / dSudoMass / Radius / Radius * vAngularImpact;
	aries::Vector3df dp     = dt * (V  + (0.5 * dv ));
	aries::Vector3df dw     = dt * (AV + (0.5 * dav));

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	pDOStatus->SetAngularVelocity(AV + dav);
	ClearImpact();
}

_float_t DOSphere::CrossAreaToSurface(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const
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
		return (r * r - dSphere2Surface * dSphere2Surface) * aries::math::_PI;
	}
}

}   // namespace vedo
