#include <Common/Interfaces/DOConstrainedQuasiCylinder.h>

namespace VEDO
{

DOConstrainedQuasiCylinder::DOConstrainedQuasiCylinder
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Radius      = cpdoml->GetShapeAttributes().quasicylinder.radius;
	double Height      = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = NJR::Vector3d();
}

void DOConstrainedQuasiCylinder::Response(double dt)
{
	NJR::Vector3d V     = pDOStatus->GetVelocity();
	NJR::Vector3d AV    = pDOStatus->GetAngularVelocity();
	NJR::Vector3d P     = pDOStatus->GetPosition();
	NJR::Vector3d Ox    = pDOStatus->GetOrientationX();
	NJR::Vector3d Oz    = pDOStatus->GetOrientationZ();

	NJR::Vector3d dv    = 1.0 / dSudoMass * (vImpact.Dot(Oz) * (Oz.direction()));
	NJR::Vector3d dp    = dt * (V + (0.5 * dv));
	NJR::Vector3d dw    = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
};

};   // namespace VEDO
