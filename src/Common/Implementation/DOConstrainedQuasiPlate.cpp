#include <Common/Interfaces/DOConstrainedQuasiPlate.h>
#include <cmath>

using namespace std;

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
	vMassMomentInertia = NJRvector3d();
	pDOStatus->SetRange(max(max(Width, Height), Length));
};

void DOConstrainedQuasiPlate::Response(double dt)
{
	NJRvector3d V   = pDOStatus->GetVelocity();
	NJRvector3d AV  = pDOStatus->GetAngularVelocity();
	NJRvector3d P   = pDOStatus->GetPosition();
	NJRvector3d Ox  = pDOStatus->GetOrientationX();
	NJRvector3d Oz  = pDOStatus->GetOrientationZ();

	NJRvector3d dv  = 1.0 / dSudoMass * (vImpact.Dot(Oz) * (Oz.direction()));
	NJRvector3d dp  = dt * (V + (0.5 * dv));
	NJRvector3d dw  = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
}
