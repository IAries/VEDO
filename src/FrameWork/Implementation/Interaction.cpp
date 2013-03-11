#include <FrameWork/Interfaces/Interaction.h>

Interaction::Interaction
	(DiscreteObject* doslave,
	DiscreteObject* domaster,
	ContactDetector* cd,
	ImpactSolver* is)
{
	pdoMaster = domaster;
	pdoSlave  = doslave;
	pcd       = cd;
	pis       = is;
	pcd->Detect(pdoSlave, pdoMaster);
};

Interaction::~Interaction()
{
	if (pcd != 0)
	{
		delete pcd;
	}

	if (pis != 0)
	{
		delete pis;
	}
};

void Interaction::ControlError()
{
	if (pcd->GetContactInfo()->bUnBalance == true)
	{
		pis->InitialStep(pcd, pdoSlave, pdoMaster);
	}
};

void Interaction::SolveImpact(const double dt)
{
	//if (pcd->GetContactInfo()->bActive == true)
	pis->NextStep(pcd, pdoSlave, pdoMaster, dt);
};

void Interaction::DetectContact()
{
	pcd->Detect(pdoSlave, pdoMaster);
};
