#include <vedo/common/interfaces/DOFixedQuasiPlateWithCircularHole.h>
#include <cmath>

namespace vedo
{

DOFixedQuasiPlateWithCircularHole::DOFixedQuasiPlateWithCircularHole
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Width       = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.width;
	double Height      = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.height;
	double Length      = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.length;
	double HoleRadius  = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holeradius;
	double HoleXOffset = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holexoffset;
	double HoleYOffset = cpdoml->GetShapeAttributes().quasiplatewithcircularhole.holeyoffset;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = 0.0;
	vMassMomentInertia = njr::Vector3d();
};

};   // namespace vedo
