#include <Common/Interfaces/DOQuasiPlate.h>
#include <cmath>

using namespace std;

DOQuasiPlate::DOQuasiPlate
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Width  = cpdoml->GetShapeAttributes().quasiplate.width;
	double Height = cpdoml->GetShapeAttributes().quasiplate.height;
	double Length = cpdoml->GetShapeAttributes().quasiplate.length;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = 0.0;
	vMassMomentInertia = NJRvector3d();
	pDOStatus->SetRange(max(max(Width, Height), Length));
};

void DOQuasiPlate::Response(double dt)
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
