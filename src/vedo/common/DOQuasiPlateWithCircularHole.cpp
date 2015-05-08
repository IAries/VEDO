#include <vedo/common/DOQuasiPlateWithCircularHole.h>
#include <cmath>

namespace vedo
{

DOQuasiPlateWithCircularHole::DOQuasiPlateWithCircularHole(const DOStatus* cpdos, const DOModel* cpdoml):
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
	vMassMomentInertia       = Vector3df();
}

void DOQuasiPlateWithCircularHole::Response(_float_t dt)
{
	Vector3df V  = pDOStatus->GetVelocity();
	Vector3df AV = pDOStatus->GetAngularVelocity();
	Vector3df P  = pDOStatus->GetPosition();
	Vector3df Ox = pDOStatus->GetOrientationX();
	Vector3df Oz = pDOStatus->GetOrientationZ();

	Vector3df dp =  V * dt;
	Vector3df dw = AV * dt;

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	//pDOStatus->SetVelocity(V);
	//pDOStatus->SetAngularVelocity(AV);
	ClearImpact();
}

}   // namespace vedo
