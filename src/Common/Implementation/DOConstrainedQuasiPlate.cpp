#include <Common/Interfaces/DOConstrainedQuasiPlate.h>
#include <cmath>

namespace VEDO
{

DOConstrainedQuasiPlate::DOConstrainedQuasiPlate
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Width  = cpdoml->GetShapeAttributes().quasiplate.width;
	double Height = cpdoml->GetShapeAttributes().quasiplate.height;
	double Length = cpdoml->GetShapeAttributes().quasiplate.length;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = NJR::Vector3d();
};

void DOConstrainedQuasiPlate::Response(double dt)
{
	NJR::Vector3d V   = pDOStatus->GetVelocity();
	NJR::Vector3d AV  = pDOStatus->GetAngularVelocity();
	NJR::Vector3d P   = pDOStatus->GetPosition();
	NJR::Vector3d Ox  = pDOStatus->GetOrientationX();
	NJR::Vector3d Oz  = pDOStatus->GetOrientationZ();

	NJR::Vector3d dv  = 1.0 / dSudoMass * (vImpact.Dot(Oz) * (Oz.direction()));
	NJR::Vector3d dp  = dt * (V + (0.5 * dv));
	NJR::Vector3d dw  = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
};

};   // namespace VEDO
