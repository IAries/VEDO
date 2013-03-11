#include <Common/Interfaces/DOFixedQuasiPlate.h>
#include <cmath>

using namespace std;

DOFixedQuasiPlate::DOFixedQuasiPlate
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
