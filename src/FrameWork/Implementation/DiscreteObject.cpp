#include <FrameWork/Interfaces/DiscreteObject.h>

namespace vedo
{

unsigned long DiscreteObject::ulCounter = 0;

DiscreteObject::DiscreteObject
	(const DOStatus* cpdos, const DOModel* cpdoml)
{
	cpDOModel      = cpdoml;
	pDOStatus      = new DOStatus(*cpdos);
	dVolume        = cpdoml->GetVolume();
	dMass          = cpdoml->GetMass();
	dSudoMass      = cpdoml->GetSudoMass();
	vImpact        = njrdxf::ZERO;
	vAngularImpact = njrdxf::ZERO;
	ulCounter++;
};

DiscreteObject::~DiscreteObject ()
{
    ulCounter--;
	delete pDOStatus;
};

void DiscreteObject::ModifyVelocity(const njr::Vector3d& vdv)
{
	pDOStatus->SetVelocity ( (pDOStatus->GetVelocity()) + vdv);
};

void DiscreteObject::ModifyPosition(const njr::Vector3d& vdp)
{
	pDOStatus->SetPosition ( (pDOStatus->GetPosition()) + vdp);
};

void DiscreteObject::ClearImpact()
{
	vImpact        = njrdxf::ZERO;
	vAngularImpact = njrdxf::ZERO;
};

void DiscreteObject::AddImpact
	(const njr::Vector3d& impact, const njr::Vector3d& angularimpact)
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
		njr::Vector3d pos = pDOStatus->GetPosition();
		bc.EnforceBoundaryConditions(&pos);
		pDOStatus->SetPosition(pos);
	}
};

};   // namespace vedo
