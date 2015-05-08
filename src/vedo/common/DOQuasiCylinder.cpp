#include <vedo/common/DOQuasiCylinder.h>

namespace vedo
{

DOQuasiCylinder::DOQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
	//_float_t Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	//_float_t Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = dMass;
	vMassMomentInertia  = cpdoml->GetMassMomentInertia();
}

void DOQuasiCylinder::Response(_float_t dt)
{
	Vector3df V  = pDOStatus->GetVelocity();
	Vector3df AV = pDOStatus->GetAngularVelocity();
	Vector3df P  = pDOStatus->GetPosition();
	Vector3df Ox = pDOStatus->GetOrientationX();
	Vector3df Oz = pDOStatus->GetOrientationZ();

	Vector3df dv = 1.0 / dSudoMass * vImpact;
	Vector3df dav
		(vAngularImpact.dot(Ox)           / vMassMomentInertia.x(),
		 vAngularImpact.dot(Oz.cross(Ox)) / vMassMomentInertia.y(),
		 vAngularImpact.dot(Oz)           / vMassMomentInertia.z() );

	Vector3df dp = dt * (V  + (0.5 * dv ));
	Vector3df dw = dt * (AV + (0.5 * dav));

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	pDOStatus->SetAngularVelocity(AV + dav);

	ClearImpact();
}

}   // namespace vedo
