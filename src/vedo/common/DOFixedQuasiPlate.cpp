#include <vedo/common/DOFixedQuasiPlate.h>
#include <cmath>

namespace vedo
{

DOFixedQuasiPlate::DOFixedQuasiPlate(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
	//_float_t Width  = cpdoml->GetShapeAttributes().quasiplate.width;
	//_float_t Height = cpdoml->GetShapeAttributes().quasiplate.height;
	//_float_t Length = cpdoml->GetShapeAttributes().quasiplate.length;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = 0.0;
	vMassMomentInertia  = aries::Vector3df();
}

}   // namespace vedo
