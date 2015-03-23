#include <vedo/common/DOFixedQuasiPlateWithCircularHole.h>
#include <cmath>

namespace vedo
{

DOFixedQuasiPlateWithCircularHole::DOFixedQuasiPlateWithCircularHole(const DOStatus* cpdos, const DOModel* cpdoml):
	DiscreteObject(cpdos, cpdoml)
{
	//_float_t Width       = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.width;
	//_float_t Height      = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.height;
	//_float_t Length      = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.length;
	//_float_t HoleRadius  = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holeradius;
	//_float_t HoleXOffset = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holexoffset;
	//_float_t HoleYOffset = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holeyoffset;
	dVolume                  = cpdoml->GetVolume();
	dMass                    = cpdoml->GetMass();
	dSudoMass                = 0.0;
	vMassMomentInertia       = aries::Vector3df();
}

}   // namespace vedo
