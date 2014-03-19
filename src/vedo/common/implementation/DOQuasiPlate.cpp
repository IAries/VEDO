#include <vedo/common/interfaces/DOQuasiPlate.h>
#include <cmath>

namespace vedo
{

DOQuasiPlate::DOQuasiPlate
	(const DOStatus* cpdos, const DOModel* cpdoml)
: DiscreteObject(cpdos, cpdoml)
{
	double Width       = cpdoml->GetShapeAttributes().quasiplate.width;
	double Height      = cpdoml->GetShapeAttributes().quasiplate.height;
	double Length      = cpdoml->GetShapeAttributes().quasiplate.length;
	dVolume            = cpdoml->GetVolume();
	dMass              = cpdoml->GetMass();
	dSudoMass          = dMass;
	vMassMomentInertia = cpdoml->GetMassMomentInertia();
};

void DOQuasiPlate::Response(double dt)
{
	njr::Vector3d V  = pDOStatus->GetVelocity();
	njr::Vector3d AV = pDOStatus->GetAngularVelocity();
	njr::Vector3d P  = pDOStatus->GetPosition();
	njr::Vector3d Ox = pDOStatus->GetOrientationX();
	njr::Vector3d Oz = pDOStatus->GetOrientationZ();

	njr::Vector3d dv = 1.0 / dSudoMass * vImpact;
	njr::Vector3d
		dav
			(vAngularImpact.Dot(Ox)           / vMassMomentInertia.x(),
			 vAngularImpact.Dot(Oz.Cross(Ox)) / vMassMomentInertia.y(),
			 vAngularImpact.Dot(Oz)           / vMassMomentInertia.z() );

	njr::Vector3d dp = dt * (V  + (0.5 * dv ));
	njr::Vector3d dw = dt * (AV + (0.5 * dav));

	pDOStatus->SetPosition(P + dp);
	pDOStatus->SetOrientation(Ox.RotateAround(dw), Oz.RotateAround(dw));
	pDOStatus->SetVelocity(V + dv);
	pDOStatus->SetAngularVelocity(AV + dav);

	ClearImpact();
};

};   // namespace vedo
