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
	vMassMomentInertia = NJR::NJRvector3d();
};

void DOConstrainedQuasiPlate::Response(double dt)
{
	NJR::NJRvector3d V   = pDOStatus->GetVelocity();
	NJR::NJRvector3d AV  = pDOStatus->GetAngularVelocity();
	NJR::NJRvector3d P   = pDOStatus->GetPosition();
	NJR::NJRvector3d Ox  = pDOStatus->GetOrientationX();
	NJR::NJRvector3d Oz  = pDOStatus->GetOrientationZ();

	NJR::NJRvector3d dv  = 1.0 / dSudoMass * (vImpact.Dot(Oz) * (Oz.direction()));
	NJR::NJRvector3d dp  = dt * (V + (0.5 * dv));
	NJR::NJRvector3d dw  = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
};

};   // namespace VEDO
