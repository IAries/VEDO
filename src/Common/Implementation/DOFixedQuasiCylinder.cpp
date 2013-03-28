#include <Common/Interfaces/DOFixedQuasiCylinder.h>

namespace VEDO
{

DOFixedQuasiCylinder::DOFixedQuasiCylinder
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	double Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = 0.0;
	vMassMomentInertia = NJR::Vector3d();
};

};   // namespace VEDO
