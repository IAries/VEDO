#include <Common/Interfaces/DOConstrainedQuasiCylinder.h>

using namespace std;

DOConstrainedQuasiCylinder::DOConstrainedQuasiCylinder
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Radius      = cpdoml->GetShapeAttributes().quasicylinder.radius;
	double Height      = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = cpdoml->GetSudoMass();
	vMassMomentInertia = NJRvector3d();
	pDOStatus->SetRange(max(Radius, Height));
}

void DOConstrainedQuasiCylinder::Response(double dt)
{
	NJRvector3d V     = pDOStatus->GetVelocity();
	NJRvector3d AV    = pDOStatus->GetAngularVelocity();
	NJRvector3d P     = pDOStatus->GetPosition();
	NJRvector3d Ox    = pDOStatus->GetOrientationX();
	NJRvector3d Oz    = pDOStatus->GetOrientationZ();

	NJRvector3d dv    = 1.0 / dSudoMass * (vImpact.Dot(Oz) * (Oz.direction()));
	NJRvector3d dp    = dt * (V + (0.5 * dv));
	NJRvector3d dw    = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
}
