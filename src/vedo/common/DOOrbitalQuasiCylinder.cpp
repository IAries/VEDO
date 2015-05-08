#include <vedo/common/DOOrbitalQuasiCylinder.h>

namespace vedo
{

DOOrbitalQuasiCylinder::DOOrbitalQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
	//_float_t Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	//_float_t Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = 0.0;
	vMassMomentInertia  = Vector3df();
}

void DOOrbitalQuasiCylinder::Response(_float_t dt)
{
	Vector3df V  = pDOStatus->GetVelocity();
	Vector3df AV = pDOStatus->GetAngularVelocity();
	Vector3df P  = pDOStatus->GetPosition();
	Vector3df Ox = pDOStatus->GetOrientationX();
	Vector3df Oz = pDOStatus->GetOrientationZ();

	Vector3df dp =  V * dt;
	Vector3df dw = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	//pDOStatus->SetVelocity(V);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
}

}   // namespace vedo
