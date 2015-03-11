#include <vedo/common/interfaces/DOQuasiPlateWithCircularHole.h>
#include <cmath>

namespace vedo
{

DOQuasiPlateWithCircularHole::DOQuasiPlateWithCircularHole(const DOStatus* cpdos, const DOModel* cpdoml):
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

void DOQuasiPlateWithCircularHole::Response(vedo_float_t dt)
{
	njr::Vector3d V  = pDOStatus->GetVelocity();
	njr::Vector3d AV = pDOStatus->GetAngularVelocity();
	njr::Vector3d P  = pDOStatus->GetPosition();
	njr::Vector3d Ox = pDOStatus->GetOrientationX();
	njr::Vector3d Oz = pDOStatus->GetOrientationZ();

	njr::Vector3d dp =  V * dt;
	njr::Vector3d dw = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	//pDOStatus->SetVelocity(V);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
}

}   // namespace vedo
