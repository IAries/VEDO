#include <Common/Interfaces/DOQuasiCylinder.h>

using namespace std;

DOQuasiCylinder::DOQuasiCylinder
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Radius      = cpdoml->GetShapeAttributes().quasicylinder.radius;
	double Height      = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = 0.0;
	vMassMomentInertia = NJRvector3d();
	pDOStatus->SetRange(max(Radius, Height));
};

void DOQuasiCylinder::Response(double dt)
{
	NJRvector3d V  = pDOStatus->GetVelocity();
	NJRvector3d AV = pDOStatus->GetAngularVelocity();
	NJRvector3d P  = pDOStatus->GetPosition();
	NJRvector3d Ox = pDOStatus->GetOrientationX();
	NJRvector3d Oz = pDOStatus->GetOrientationZ();

	NJRvector3d dp =  V * dt;
	NJRvector3d dw = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	//pDOStatus->SetVelocity(V);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
};
