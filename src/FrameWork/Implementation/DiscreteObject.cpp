#include <FrameWork/Interfaces/DiscreteObject.h>

unsigned long DiscreteObject::ulCounter = 0;

DiscreteObject::DiscreteObject
	(const DOStatus* cpdos, const DOModel* cpdoml)
{
	cpDOModel      = cpdoml;
	pDOStatus      = new DOStatus(*cpdos);
	dVolume        = cpdoml->GetVolume();
	dMass          = cpdoml->GetMass();
	dSudoMass      = cpdoml->GetSudoMass();
	vImpact        = ZERO;
	vAngularImpact = ZERO;
	ulCounter++;
};

DiscreteObject::~DiscreteObject ()
{
    ulCounter--;
	delete pDOStatus;
};

void DiscreteObject::ModifyVelocity(const NJRvector3d& vdv)
{
	pDOStatus->SetVelocity ( (pDOStatus->GetVelocity()) + vdv);
};

void DiscreteObject::ModifyPosition(const NJRvector3d& vdp)
{
	pDOStatus->SetPosition ( (pDOStatus->GetPosition()) + vdp);
};

void DiscreteObject::ClearImpact()
{
	vImpact        = ZERO;
	vAngularImpact = ZERO;
};

void DiscreteObject::AddImpact
	(const NJRvector3d& impact, const NJRvector3d& angularimpact)
{
	vImpact        = vImpact        + impact;
	vAngularImpact = vAngularImpact + angularimpact;
};

void DiscreteObject::AddConstrainedImpact(double dt)
{
	vImpact += dt * (cpDOModel->GetExternalForce());
};

void DiscreteObject::EnforcePeriodicBoundaryConditions
		(const Boundary& bc)
{
	if(cpDOModel->GetBehavior() == "mobile")
	{
		NJRvector3d pos = pDOStatus->GetPosition();
		bc.EnforceBoundaryConditions(&pos);
		pDOStatus->SetPosition(pos);
	}
};
