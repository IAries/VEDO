#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

_uint_t DiscreteObject::ulCounter = 0;

DiscreteObject::DiscreteObject(const DOStatus* cpdos, const DOModel* cpdoml)
{
	cpDOModel      = cpdoml;
	pDOStatus      = new DOStatus(*cpdos);
	dVolume        = cpdoml->GetVolume();
	dMass          = cpdoml->GetMass();
	dSudoMass      = cpdoml->GetSudoMass();
	vImpact.set(0.0, 0.0, 0.0);
	vAngularImpact.set(0.0, 0.0, 0.0);
	ulCounter++;
}

DiscreteObject::~DiscreteObject ()
{
	ulCounter--;
	delete pDOStatus;
}

void DiscreteObject::ModifyVelocity(const Vector3df& vdv)
{
	pDOStatus->SetVelocity ( (pDOStatus->GetVelocity()) + vdv);
}

void DiscreteObject::ModifyPosition(const Vector3df& vdp)
{
	pDOStatus->SetPosition ( (pDOStatus->GetPosition()) + vdp);
}

void DiscreteObject::ClearImpact()
{
	pDOStatus->SetImpact(vImpact);
	pDOStatus->SetAngularImpact(vAngularImpact);
	vImpact.set(0.0, 0.0, 0.0);
	vAngularImpact.set(0.0, 0.0, 0.0);
}

void DiscreteObject::AddImpact(const Vector3df& impact, const Vector3df& angularimpact)
{
	vImpact        = vImpact        + impact;
	vAngularImpact = vAngularImpact + angularimpact;
}

void DiscreteObject::AddImpact(const Vector3df& impact)
{
	vImpact        = vImpact        + impact;
}

void DiscreteObject::AddConstrainedImpact(_float_t dt)
{
	vImpact += dt * (cpDOModel->GetExternalForce());
}

void DiscreteObject::EnforcePeriodicBoundaryConditions(const Boundary& bc)
{
	Vector3df pos = pDOStatus->GetPosition();
	bc.EnforceBoundaryConditions(&pos);
	pDOStatus->SetPosition(pos);
}

}   // namespace vedo
