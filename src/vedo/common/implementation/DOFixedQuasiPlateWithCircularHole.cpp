#include <vedo/common/interfaces/DOFixedQuasiPlateWithCircularHole.h>
#include <cmath>

namespace vedo
{

DOFixedQuasiPlateWithCircularHole::DOFixedQuasiPlateWithCircularHole(const DOStatus* cpdos, const DOModel* cpdoml):
	DiscreteObject(cpdos, cpdoml)
{
	//vedo_float_t Width       = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.width;
	//vedo_float_t Height      = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.height;
	//vedo_float_t Length      = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.length;
	//vedo_float_t HoleRadius  = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holeradius;
	//vedo_float_t HoleXOffset = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holexoffset;
	//vedo_float_t HoleYOffset = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holeyoffset;
	dVolume                  = cpdoml->GetVolume();
	dMass                    = cpdoml->GetMass();
	dSudoMass                = 0.0;
	vMassMomentInertia       = njr::Vector3d();
}

}   // namespace vedo
