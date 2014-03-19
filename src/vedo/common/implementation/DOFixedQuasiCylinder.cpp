#include <vedo/common/interfaces/DOFixedQuasiCylinder.h>

namespace vedo
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
	vMassMomentInertia = njr::Vector3d();
};

};   // namespace vedo
