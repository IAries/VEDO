#include <Common/Interfaces/DOQuasiCylinder.h>

namespace VEDO
{

DOQuasiCylinder::DOQuasiCylinder
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Radius      = cpdoml->GetShapeAttributes().quasicylinder.radius;
	double Height      = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = 0.0;
	vMassMomentInertia = NJR::NJRvector3d();
};

void DOQuasiCylinder::Response(double dt)
{
	NJR::NJRvector3d V  = pDOStatus->GetVelocity();
	NJR::NJRvector3d AV = pDOStatus->GetAngularVelocity();
	NJR::NJRvector3d P  = pDOStatus->GetPosition();
	NJR::NJRvector3d Ox = pDOStatus->GetOrientationX();
	NJR::NJRvector3d Oz = pDOStatus->GetOrientationZ();

	NJR::NJRvector3d dp =  V * dt;
	NJR::NJRvector3d dw = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	//pDOStatus->SetVelocity(V);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
};

};   // namespace VEDO
