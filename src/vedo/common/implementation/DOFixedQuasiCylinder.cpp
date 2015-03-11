#include <vedo/common/interfaces/DOFixedQuasiCylinder.h>

namespace vedo
{

DOFixedQuasiCylinder::DOFixedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml): DiscreteObject(cpdos, cpdoml)
{
	//vedo_float_t Radius = cpdoml->GetShapeAttributes().quasicylinder.radius;
	//vedo_float_t Height = cpdoml->GetShapeAttributes().quasicylinder.height;
	dVolume             = cpdoml->GetVolume();
	dMass               = cpdoml->GetMass();
	dSudoMass           = 0.0;
	vMassMomentInertia  = njr::Vector3d();
}

}   // namespace vedo
