#include <vedo/common/DOFixedQuasiCylinder.h>

namespace vedo
{

DOFixedQuasiCylinder::DOFixedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
	//_float_t Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	//_float_t Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = 0.0;
	vMassMomentInertia  = aries::Vector3df();
}

}   // namespace vedo
