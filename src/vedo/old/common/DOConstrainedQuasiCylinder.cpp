#include <vedo/common/DOConstrainedQuasiCylinder.h>

namespace vedo
{

DOConstrainedQuasiCylinder::DOConstrainedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml):
	DiscreteObject(cpdos, cpdoml)
{
	//_float_t Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	//_float_t Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = cpdoml->GetSudoMass();
	vMassMomentInertia  = Vector3df();
}

void DOConstrainedQuasiCylinder::Response(_float_t dt)
{
	Vector3df V  = pDOStatus->GetVelocity();
	Vector3df AV = pDOStatus->GetAngularVelocity();
	Vector3df P  = pDOStatus->GetPosition();
	Vector3df Ox = pDOStatus->GetOrientationX();
	Vector3df Oz = pDOStatus->GetOrientationZ();

	Vector3df dv = 1.0 / dSudoMass * (vImpact.dot(Oz) * (Oz.direction()));
	Vector3df dp = dt * (V + (0.5 * dv));
	Vector3df dw = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
}

}   // namespace vedo
