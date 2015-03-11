#include <vedo/common/interfaces/DOQuasiCylinder.h>

namespace vedo
{

DOQuasiCylinder::DOQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
	//vedo_float_t Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	//vedo_float_t Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = dMass;
	vMassMomentInertia  = cpdoml->GetMassMomentInertia();
}

void DOQuasiCylinder::Response(vedo_float_t dt)
{
	njr::Vector3d V  = pDOStatus->GetVelocity();
	njr::Vector3d AV = pDOStatus->GetAngularVelocity();
	njr::Vector3d P  = pDOStatus->GetPosition();
	njr::Vector3d Ox = pDOStatus->GetOrientationX();
	njr::Vector3d Oz = pDOStatus->GetOrientationZ();

	njr::Vector3d dv = 1.0 / dSudoMass * vImpact;
	njr::Vector3d dav
		(vAngularImpact.Dot(Ox)           / vMassMomentInertia.x(),
		 vAngularImpact.Dot(Oz.Cross(Ox)) / vMassMomentInertia.y(),
		 vAngularImpact.Dot(Oz)           / vMassMomentInertia.z() );

	njr::Vector3d dp = dt * (V  + (0.5 * dv ));
	njr::Vector3d dw = dt * (AV + (0.5 * dav));

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	pDOStatus->SetAngularVelocity(AV + dav);

	ClearImpact();
}

}   // namespace vedo
