#include <vedo/njr/interfaces/Constants.h>
#include <vedo/common/interfaces/DOSphere.h>
#include <cmath>

namespace vedo
{

DOSphere::DOSphere(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
//	vedo_float_t Radius = cpdoml->GetShapeAttributes().sphere.radius;
/*
	vedo_float_t mmi = 0.4 * dMass * Radius * Radius;
	vMassMomentInertia = njr::Vector3d(mmi, mmi, mmi);
*/
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = cpdoml->GetMassMomentInertia();
}

void DOSphere::Response(vedo_float_t dt)
{
	njr::Vector3d V      = pDOStatus->GetVelocity();
	njr::Vector3d AV     = pDOStatus->GetAngularVelocity();
	njr::Vector3d P      = pDOStatus->GetPosition();
	njr::Vector3d Ox     = pDOStatus->GetOrientationX();
	njr::Vector3d Oz     = pDOStatus->GetOrientationZ();
	vedo_float_t  Radius = cpDOModel->GetShapeAttributes().sphere.radius;

	njr::Vector3d dv     = 1.0 / dSudoMass * vImpact;
/*
	njr::Vector3d
		dav
			(vAngularImpact.x() / vMassMomentInertia.x(),
			 vAngularImpact.y() / vMassMomentInertia.y(),
			 vAngularImpact.z() / vMassMomentInertia.z() );
	njr::Vector3d dav    = (1.0/(0.4*dSudoMass*Radius*Radius)) * vAngularImpact;
*/
	njr::Vector3d dav    = 2.5 / dSudoMass / Radius / Radius * vAngularImpact;
	njr::Vector3d dp     = dt * (V  + (0.5 * dv ));
	njr::Vector3d dw     = dt * (AV + (0.5 * dav));

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	pDOStatus->SetAngularVelocity(AV + dav);
	ClearImpact();
}

vedo_float_t DOSphere::CrossAreaToSurface(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const
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
