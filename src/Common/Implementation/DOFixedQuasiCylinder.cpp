#include <Common/Interfaces/DOFixedQuasiCylinder.h>

using namespace std;

DOFixedQuasiCylinder::DOFixedQuasiCylinder
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	double Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = 0.0;
	vMassMomentInertia = NJRvector3d();
	pDOStatus->SetRange(max(Radius, Height));
};
