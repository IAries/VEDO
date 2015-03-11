#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

vedo::vedo_uint_t DiscreteObject::ulCounter = 0;

DiscreteObject::DiscreteObject(const DOStatus* cpdos, const DOModel* cpdoml)
{
	cpDOModel      = cpdoml;
	pDOStatus      = new DOStatus(*cpdos);
	dVolume        = cpdoml->GetVolume();
	dMass          = cpdoml->GetMass();
	dSudoMass      = cpdoml->GetSudoMass();
	vImpact        = njr::ZERO;
	vAngularImpact = njr::ZERO;
	ulCounter++;
}

DiscreteObject::~DiscreteObject ()
{
	ulCounter--;
	delete pDOStatus;
}

void DiscreteObject::ModifyVelocity(const njr::Vector3d& vdv)
{
	pDOStatus->SetVelocity ( (pDOStatus->GetVelocity()) + vdv);
}

void DiscreteObject::ModifyPosition(const njr::Vector3d& vdp)
{
	pDOStatus->SetPosition ( (pDOStatus->GetPosition()) + vdp);
}

void DiscreteObject::ClearImpact()
{
	pDOStatus->SetImpact(vImpact);
	pDOStatus->SetAngularImpact(vAngularImpact);
	vImpact        = njr::ZERO;
	vAngularImpact = njr::ZERO;
}

void DiscreteObject::AddImpact(const njr::Vector3d& impact, const njr::Vector3d& angularimpact)
{
	vImpact        = vImpact        + impact;
	vAngularImpact = vAngularImpact + angularimpact;
}

void DiscreteObject::AddConstrainedImpact(vedo_float_t dt)
{
	vImpact += dt * (cpDOModel->GetExternalForce());
}

void DiscreteObject::EnforcePeriodicBoundaryConditions(const Boundary& bc)
{
	njr::Vector3d pos = pDOStatus->GetPosition();
	bc.EnforceBoundaryConditions(&pos);
	pDOStatus->SetPosition(pos);
}

}   // namespace vedo
