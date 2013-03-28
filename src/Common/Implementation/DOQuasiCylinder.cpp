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
	vMassMomentInertia = NJR::Vector3d();
};

void DOQuasiCylinder::Response(double dt)
{
	NJR::Vector3d V  = pDOStatus->GetVelocity();
	NJR::Vector3d AV = pDOStatus->GetAngularVelocity();
	NJR::Vector3d P  = pDOStatus->GetPosition();
	NJR::Vector3d Ox = pDOStatus->GetOrientationX();
	NJR::Vector3d Oz = pDOStatus->GetOrientationZ();

	NJR::Vector3d dp =  V * dt;
	NJR::Vector3d dw = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	//pDOStatus->SetVelocity(V);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
};

};   // namespace VEDO
