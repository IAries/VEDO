#include <vedo/common/interfaces/DOFixedQuasiPlate.h>
#include <cmath>

namespace vedo
{

DOFixedQuasiPlate::DOFixedQuasiPlate(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
	//vedo_float_t Width  = cpdoml->GetShapeAttributes().quasiplate.width;
	//vedo_float_t Height = cpdoml->GetShapeAttributes().quasiplate.height;
	//vedo_float_t Length = cpdoml->GetShapeAttributes().quasiplate.length;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = 0.0;
	vMassMomentInertia  = njr::Vector3d();
}

}   // namespace vedo
