#include <vedo/common/interfaces/DOQuasiPlateWithCircularHole.h>
#include <cmath>

namespace vedo
{

DOQuasiPlateWithCircularHole::DOQuasiPlateWithCircularHole
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

void DOQuasiPlateWithCircularHole::Response(double dt)
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
};

};   // namespace vedo
