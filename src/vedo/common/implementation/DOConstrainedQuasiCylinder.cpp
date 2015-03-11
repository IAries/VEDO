#include <vedo/common/interfaces/DOConstrainedQuasiCylinder.h>

namespace vedo
{

DOConstrainedQuasiCylinder::DOConstrainedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml):
	DiscreteObject(cpdos, cpdoml)
{
	//vedo_float_t Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	//vedo_float_t Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = cpdoml->GetSudoMass();
	vMassMomentInertia  = njr::Vector3d();
}

void DOConstrainedQuasiCylinder::Response(vedo_float_t dt)
{
	njr::Vector3d V  = pDOStatus->GetVelocity();
	njr::Vector3d AV = pDOStatus->GetAngularVelocity();
	njr::Vector3d P  = pDOStatus->GetPosition();
	njr::Vector3d Ox = pDOStatus->GetOrientationX();
	njr::Vector3d Oz = pDOStatus->GetOrientationZ();

	njr::Vector3d dv = 1.0 / dSudoMass * (vImpact.Dot(Oz) * (Oz.direction()));
	njr::Vector3d dp = dt * (V + (0.5 * dv));
	njr::Vector3d dw = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
}

}   // namespace vedo
