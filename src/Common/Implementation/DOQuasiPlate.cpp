#include <Common/Interfaces/DOQuasiPlate.h>
#include <cmath>

namespace VEDO
{

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
	vMassMomentInertia = NJR::NJRvector3d();
};

void DOQuasiPlate::Response(double dt)
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
