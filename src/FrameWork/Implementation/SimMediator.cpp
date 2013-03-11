#include <FrameWork/Interfaces/Constants.h>
#include <FrameWork/Interfaces/SimMediator.h>
#include <FrameWork/Interfaces/DOMap.h>
#include <FrameWork/Interfaces/DOWorld_WriteVTK.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <iterator>
#include <map>

using namespace std;

DataFieldVTKWriter* DataFieldVTKWriter::_instance = 0;

string GetRankString(int rank, int NP)
{
    unsigned dig_total
		= static_cast<unsigned>(log10(static_cast<double>(NP))) + 1;
	unsigned dig;
    if(rank == 0)
    {
    	dig = 1;
    }
    else
    {
	    dig = static_cast<unsigned>(log10(static_cast<double>(rank))) + 1;
    }
	string dig_string = "";
   	for(unsigned i=0; i<dig; i++)
   	{
		dig_string = char(48+rank%10) + dig_string;
        rank = rank/10;
    }
   	for(unsigned i=0; i<(dig_total-dig); i++)
    {
    	dig_string = '0' + dig_string;
   	}
    return dig_string;
}

void SimMediator::CalculateSystemEnergy()
{
	pConsultant->CalculateSystemEnergy();
	if (rank == 0)
	{
		const SystemParameter* csp
			= pConsultant->GetDOWorld()->GetSystemParameter();
		FileLogEnergy
			<< csp->GetTimeCurrent()         << ", "
			<< csp->GetDONumber()            << ", "
			<< csp->GetEnergyPotential()
			 + csp->GetEnergyTranslation()
			 + csp->GetEnergyRotation()      << ", "
			<< csp->GetEnergyPotential()     << ", "
			<< csp->GetEnergyTranslation()
			 + csp->GetEnergyRotation()      << ", "
			<< csp->GetEnergyTranslation()   << ", "
			<< csp->GetEnergyRotation()      << ", "
			<< csp->GetVelocityMin()         << ", "
			<< csp->GetVelocityMax()         << ", "
			<< csp->GetAngularVelocityMin()  << ", "
			<< csp->GetAngularVelocityMax()  << ", "
			<< csp->GetMomentumNorm()        << ", "
			<< csp->GetAngularMomentumNorm();

		for(unsigned u=0; u<2*uNumUserDefinedData; u++)
			FileLogEnergy << ", " << pConsultant->GetUserDefinedValue(u);
			
		FileLogEnergy << endl;
	};
};

SimMediator::SimMediator
	(Consultant* Consultant, const Assembler* Assembler)
	: pConsultant(Consultant), cpAssembler(Assembler), rank(0), NP(1)
{
	TimeInitiate();
	FileLog.open("time_0.csv", ios::out);
	FileLog
		<< "Rank, SimulatedTime, System, ImpactSolving, FieldForceAdding, "
		<< "ResponseUpdating, ContactDetection, NextStep, "
		<< "DOContainerSynchronization, Partitioning, Computing, "
		<< "Communication, Total"
		<< endl;
	FileLogEnergy.open("energy.csv", ios::out);
	FileLogEnergy
		<< "SystemTime, ElementNumber, "
		<< "SystemEnergy, PotentialEnergy, KineticEnergy, "
		<< "KineticEnergyTranslation, KineticEnergyRotation, MinimalVelocity, "
		<< "MaximalVelocity, MinimalAngularVelocity, MaximalAngularVelocity, "
		<< "NormMomentum, NormAngularMomentum";

	for(unsigned u=0; u<2*uNumUserDefinedData; u++)
		FileLogEnergy << ", User-defined Value " << u+1;

	FileLogEnergy << endl;

	CalculateSystemEnergy();
};

SimMediator::SimMediator
	(Consultant* Consultant,
	const Assembler* Assembler,
	const unsigned int r,
	const unsigned int np)
	: pConsultant(Consultant), cpAssembler(Assembler), rank(r), NP(np)
{
	pConsultant->SetRankNP(r, np);
	TimeInitiate();
	string name = "time_" + GetRankString(r, np);
	name += ".csv";
	FileLog.open(name.c_str(), ios::out);
	FileLog
		<< "Rank, SimulatedTime, System, ImpactSolving, FieldForceAdding, "
		<< "ResponseUpdating, ContactDetection, NextStep, "
		<< "DOContainerSynchronization, Partitioning, Computing, Communication, Total"
		<< endl;

	if (rank == 0)
	{
		FileLogEnergy.open("energy.csv", ios::out);
		FileLogEnergy
			<< "SystemTime, ElementNumber, "
			<< "SystemEnergy, PotentialEnergy, KineticEnergy, "
			<< "KineticEnergyTranslation, KineticEnergyRotation, MinimalVelocity, "
			<< "MaximalVelocity, MinimalAngularVelocity, MaximalAngularVelocity, "
			<< "NormMomentum, NormAngularMomentum, ";

		for(unsigned u=0; u<2*uNumUserDefinedData; u++)
			FileLogEnergy << ", User-defined Value " << u+1;

		FileLogEnergy << endl;
	};
	CalculateSystemEnergy();
};

SimMediator::~SimMediator()
{
	FileLog.close();
	if (rank == 0)
		FileLogEnergy.close();
};

void SimMediator::TimeInitiate()
{
	timeSystem           = 0.0;
	timeImpactSolving    = 0.0;
	timeFieldForceAdding = 0.0;
	timeResponseUpdating = 0.0;
	timeContactDetection = 0.0;
	timeNextStep         = 0.0;
	timeSyncDOContainer  = 0.0;
	timePartitioning     = 0.0;
	timeComputing        = 0.0;
	timeCommunication    = 0.0;
	timeTotal            = 0.0;
}

void SimMediator::ShowInteraction()
{
	ofstream FileInteraction;
	const DOWorld*         cpDOWorld = pConsultant->GetDOWorld();
	const SystemParameter* csp       = cpDOWorld->GetSystemParameter();

	//cIact.CheckContactStatus();
	//cIact.CalculateImpact(csp->GetTimeInterval());

	const Interaction*  iap = 0;
	const ContactInfo*  cip = 0;
	const ImpactStatus* isp = 0;
	NJRvector3d
		vImpactMaster, vImpactSlave, vAngularImpactMaster, vAngularImpactSlave;

	if (NP == 1)
	{
		FileInteraction.open("interaction.csv", ios::out);
	}
	else
	{
		char sFileName[256];
		sprintf(sFileName, "interaction_%d.csv\0", rank);
		FileInteraction.open(sFileName, ios::out);
	}

	FileInteraction
		<< "MasterElementID, SlaveElementID, "
		<< "ImpactXToMaster, ImpactYToMaster, ImpactZToMaster, "
		<< "ImpactXToSlave, ImpactYToSlave, ImpactZToSlave, "
		<< "AngularImpactXToMaster, AngularImpactYToMaster, AngularImpactZToMaster, "
		<< "AngularImpactXToSlave, AngularImpactYToSlave, AngularImpactZToSlave, "
		<< "Overlap, "
		<< "ImpactDirectionX, ImpactDirectionY, ImpactDirectionZ, "
		<< "ImpactPointX, ImpactPointY, ImpactPointZ, "
		<< "Bond, Contacted, RememberedNormalStiffness, "
		<< "RememberedShearForceX, RememberedShearForceY, RememberedShearForceZ";

	for(unsigned u=0; u<4*uNumUserDefinedData; u++)
		FileInteraction << ", User-defined value " << u+1;

	FileInteraction << endl;

	const double* cdpudv;
	if (NP == 1)
	{
		for(unsigned long ul=0; ul<cIact.size(); ul++)
		{
			cIact.InteractionDetectContact(ul);
			iap = cIact.GetInteraction(ul);
			cip = iap->GetContactInfo();

			if (cip->bActive)
			{
				vImpactMaster        = iap->GetMaster()->GetImpact();
				vImpactSlave         = iap->GetSlave()->GetImpact();
				vAngularImpactMaster = iap->GetMaster()->GetAngularImpact();
				vAngularImpactSlave  = iap->GetSlave()->GetAngularImpact();
				cIact.CalculateImpact(csp->GetTimeInterval(), ul);
				vImpactMaster        = iap->GetMaster()->GetImpact()        - vImpactMaster;
				vImpactSlave         = iap->GetSlave()->GetImpact()         - vImpactSlave;
				vAngularImpactMaster = iap->GetMaster()->GetAngularImpact() - vAngularImpactMaster;
				vAngularImpactSlave  = iap->GetSlave()->GetAngularImpact()  - vAngularImpactSlave;
				FileInteraction
					<< pConsultant->GetIactMaster(ul) << ", "
					<< pConsultant->GetIactSlave(ul)  << ", "
					<< vImpactMaster.x()              << ", "
					<< vImpactMaster.y()              << ", "
					<< vImpactMaster.z()              << ", "
					<< vImpactSlave.x()               << ", "
					<< vImpactSlave.y()               << ", "
					<< vImpactSlave.z()               << ", "
					<< vAngularImpactMaster.x()       << ", "
					<< vAngularImpactMaster.y()       << ", "
					<< vAngularImpactMaster.z()       << ", "
					<< vAngularImpactSlave.x()        << ", "
					<< vAngularImpactSlave.y()        << ", "
					<< vAngularImpactSlave.z()        << ", "
					<< cip->dImpactDepth              << ", "
					<< cip->vImpactDirection.x()      << ", "
					<< cip->vImpactDirection.y()      << ", "
					<< cip->vImpactDirection.z()      << ", "
					<< cip->vImpactPoint.x()          << ", "
					<< cip->vImpactPoint.y()          << ", "
					<< cip->vImpactPoint.z()          << ", ";

				isp = iap->GetSolver()->GetImpactStatus();
				cdpudv = isp->RetrieveAllUserDefinedValue();

				if (isp->Bond())
				{
					FileInteraction << 1 << ", ";
				}
				else
				{
					FileInteraction << 0 << ", ";
				}

				if (isp->Contact())
				{
					FileInteraction << 1 << ", ";
				}
				else
				{
					FileInteraction << 0 << ", ";
				}

				FileInteraction
					<< isp->Kn()             << ", "
					<< isp->ShearForce().x() << ", "
					<< isp->ShearForce().y() << ", "
					<< isp->ShearForce().z();

				for(unsigned u=0; u<4*uNumUserDefinedData; u++)
					FileInteraction << ", " << *(cdpudv+u);

				FileInteraction << endl;
			}
		}
	}
	else
	{
		for(unsigned long ul=0; ul<cIact.size(); ul++)
		{
			cIact.InteractionDetectContact(ul);
			iap = cIact.GetInteraction(ul);
			cip = iap->GetContactInfo();

			if (cip->bActive)
			{
				vImpactMaster        = iap->GetMaster()->GetImpact();
				vImpactSlave         = iap->GetSlave()->GetImpact();
				vAngularImpactMaster = iap->GetMaster()->GetAngularImpact();
				vAngularImpactSlave  = iap->GetSlave()->GetAngularImpact();
				cIact.CalculateImpact(csp->GetTimeInterval(), ul);
				vImpactMaster        = iap->GetMaster()->GetImpact()        - vImpactMaster;
				vImpactSlave         = iap->GetSlave()->GetImpact()         - vImpactSlave;
				vAngularImpactMaster = iap->GetMaster()->GetAngularImpact() - vAngularImpactMaster;
				vAngularImpactSlave  = iap->GetSlave()->GetAngularImpact()  - vAngularImpactSlave;
				FileInteraction
					<< pConsultant->GetDO(pConsultant->GetIactMaster(ul)) << ", "
					<< pConsultant->GetDO(pConsultant->GetIactSlave(ul))  << ", "
					<< vImpactMaster.x()         << ", "
					<< vImpactMaster.y()         << ", "
					<< vImpactMaster.z()         << ", "
					<< vImpactSlave.x()          << ", "
					<< vImpactSlave.y()          << ", "
					<< vImpactSlave.z()          << ", "
					<< vAngularImpactMaster.x()  << ", "
					<< vAngularImpactMaster.y()  << ", "
					<< vAngularImpactMaster.z()  << ", "
					<< vAngularImpactSlave.x()   << ", "
					<< vAngularImpactSlave.y()   << ", "
					<< vAngularImpactSlave.z()   << ", "
					<< cip->dImpactDepth         << ", "
					<< cip->vImpactDirection.x() << ", "
					<< cip->vImpactDirection.y() << ", "
					<< cip->vImpactDirection.z() << ", "
					<< cip->vImpactPoint.x()     << ", "
					<< cip->vImpactPoint.y()     << ", "
					<< cip->vImpactPoint.z()     << ", ";

				isp = iap->GetSolver()->GetImpactStatus();
				cdpudv = isp->RetrieveAllUserDefinedValue();

				if (isp->Bond())
				{
					FileInteraction << 1 << ", ";
				}
				else
				{
					FileInteraction << 0 << ", ";
				}

				if (isp->Contact())
				{
					FileInteraction << 1 << ", ";
				}
				else
				{
					FileInteraction << 0 << ", ";
				}

				FileInteraction
					<< isp->Kn()             << ", "
					<< isp->ShearForce().x() << ", "
					<< isp->ShearForce().y() << ", "
					<< isp->ShearForce().z();

				for(unsigned u=0; u<4*uNumUserDefinedData; u++)
					FileInteraction << ", " << *(cdpudv+u);

				FileInteraction << endl;
			}
		}
	}

	FileInteraction.close();

	pConsultant->SyncDOContainer(cDO);
	cDO.AddFieldImpact(csp->GetFieldForce() * csp->GetTimeInterval());
	cDO.AddConstrainedImpact(csp->GetTimeInterval());

	if (NP == 1)
	{
		FileInteraction.open("interaction_element.csv", ios::out);
	}
	else
	{
		char sFileName[256];
		sprintf(sFileName, "interaction_element_%d.csv\0", rank);
		FileInteraction.open(sFileName, ios::out);
	}

	FileInteraction
		<< "ElementID, ElementName, "
		<< "PositionX, PositionY, PositionZ, "
		<< "VelocityX, VelocityY, VelocityZ, "
		<< "AngularVelocityX, AngularVelocityY, AngularVelocityZ, "
		<< "OrientationXX, OrientationXY, OrientationXZ, "
		<< "OrientationZX, OrientationZY, OrientationZZ, "
		<< "ImpactX, ImpactY, ImpactZ, "
		<< "AngularImpactX, AngularImpactY, AngularImpactZ"
		<< endl;

	const DiscreteObject* dop = 0;
	const DOStatus*       dos = 0;
	NJRvector3d
		vPosition, vVelocity, vAngularVelocity,
		vOrientationX, vOrientationZ,
		vImpact, vAngularImpact;

	if (NP == 1)
	{
		for(unsigned long ul=0; ul<cDO.size(); ul++)
		{
			dos              = cDO.GetDOStatus(ul);
			vPosition        = dos->GetPosition();
			vVelocity        = dos->GetVelocity();
			vAngularVelocity = dos->GetAngularVelocity();
			vOrientationX    = dos->GetOrientationX();
			vOrientationZ    = dos->GetOrientationZ();
			dop              = cDO.GetDiscreteObject(ul);
			vImpact          = dop->GetImpact();
			vAngularImpact   = dop->GetAngularImpact();

			FileInteraction
				<< ul                   << ", "
				<< dos->GetDOName()     << ", "
				<< vPosition.x()        << ", "
				<< vPosition.y()        << ", "
				<< vPosition.z()        << ", "
				<< vVelocity.x()        << ", "
				<< vVelocity.y()        << ", "
				<< vVelocity.z()        << ", "
				<< vAngularVelocity.x() << ", "
				<< vAngularVelocity.y() << ", "
				<< vAngularVelocity.z() << ", "
				<< vOrientationX.x()    << ", "
				<< vOrientationX.y()    << ", "
				<< vOrientationX.z()    << ", "
				<< vOrientationZ.x()    << ", "
				<< vOrientationZ.y()    << ", "
				<< vOrientationZ.z()    << ", "
				<< vImpact.x()          << ", "
				<< vImpact.y()          << ", "
				<< vImpact.z()          << ", "
				<< vAngularImpact.x()   << ", "
				<< vAngularImpact.y()   << ", "
				<< vAngularImpact.z()   << endl;
		}
	}
	else
	{
		for(unsigned long ul=0; ul<cDO.size(); ul++)
		{
			dos              = cDO.GetDOStatus(ul);
			vPosition        = dos->GetPosition();
			vVelocity        = dos->GetVelocity();
			vAngularVelocity = dos->GetAngularVelocity();
			vOrientationX    = dos->GetOrientationX();
			vOrientationZ    = dos->GetOrientationZ();
			dop              = cDO.GetDiscreteObject(ul);
			vImpact          = dop->GetImpact();
			vAngularImpact   = dop->GetAngularImpact();

			FileInteraction
				<< pConsultant->GetDO(ul) << ", "
				<< dos->GetDOName()       << ", "
				<< vPosition.x()          << ", "
				<< vPosition.y()          << ", "
				<< vPosition.z()          << ", "
				<< vVelocity.x()          << ", "
				<< vVelocity.y()          << ", "
				<< vVelocity.z()          << ", "
				<< vAngularVelocity.x()   << ", "
				<< vAngularVelocity.y()   << ", "
				<< vAngularVelocity.z()   << ", "
				<< vOrientationX.x()      << ", "
				<< vOrientationX.y()      << ", "
				<< vOrientationX.z()      << ", "
				<< vOrientationZ.x()      << ", "
				<< vOrientationZ.y()      << ", "
				<< vOrientationZ.z()      << ", "
				<< vImpact.x()            << ", "
				<< vImpact.y()            << ", "
				<< vImpact.z()            << ", "
				<< vAngularImpact.x()     << ", "
				<< vAngularImpact.y()     << ", "
				<< vAngularImpact.z()     << endl;
		}
	}

	FileInteraction.close();
}

void SimMediator::WriteInteractionForce
	(const char* filename,
	 const vector<pair<NJRvector3d, NJRvector3d> >* vvExternalImpact)
{
	const SystemParameter* csp = pConsultant->GetDOWorld()->GetSystemParameter();
	const double           dt  = csp->GetTimeInterval();

	const DiscreteObject* dop = 0;

	// Calculate the force due to interaction
	//cIact.CalculateImpact(csp->GetTimeInterval());   // It has been done in the function "ShowImpact"

	vector<double> iactForce_vec, extForce_vec, fieldForce_vec, totalForce_vec;

	for(unsigned long ul=0; ul<cDO.size(); ul++)
	{
		dop = cDO.GetDiscreteObject(ul);

		if (dop->GetDOModel()->GetBehavior() != "mobile")
			continue;

		NJRvector3d iactForce  = dop->GetImpact() * (1.0/dt);
		NJRvector3d fieldForce = csp->GetFieldForce();
		NJRvector3d extForce   = vvExternalImpact ? (*vvExternalImpact)[pConsultant->GetDO(ul)].first * (1./dt) : ZERO;
		NJRvector3d totalForce = iactForce + fieldForce + extForce;

		iactForce_vec.push_back(iactForce.x());
		iactForce_vec.push_back(iactForce.y());
		iactForce_vec.push_back(iactForce.z());
		fieldForce_vec.push_back(fieldForce.x());
		fieldForce_vec.push_back(fieldForce.y());
		fieldForce_vec.push_back(fieldForce.z());
		totalForce_vec.push_back(totalForce.x());
		totalForce_vec.push_back(totalForce.y());
		totalForce_vec.push_back(totalForce.z());
		extForce_vec.push_back(extForce.x());
		extForce_vec.push_back(extForce.y());
		extForce_vec.push_back(extForce.z());
	}

/*
	if (rank == 0)
	{
		DataFieldVTKWriter* wrapArr = DataFieldVTKWriter::Instance();
		wrapArr->clearAll();
		wrapArr->addArray("InteractionForce", 3, iactForce_vec);
		wrapArr->addArray("FieldForce", 3, fieldForce_vec);
		wrapArr->addArray("ExternalForce", 3, extForce_vec);
		wrapArr->addArray("TotalForce", 3, totalForce_vec);
		pConsultant->GetDOWorld()->WriteVTK<DataFieldVTKWriter>(filename);
	}
*/
};

void SimMediator::Initiate()
{
	cDO.Clear();
	cIact.Clear();

	unsigned long i, ulMaster, ulSlave;

	const DOModel*   cpdoml;
	const DOStatus*  cpdos;
	const IactModel* cpiactml;
	const DOWorld*   cpDOWorld = pConsultant->GetDOWorld();

	const Boundary*  pbc
		= &(cpDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions());

	unsigned long DONum   = pConsultant->GetDONum();
	unsigned long IactNum = pConsultant->GetIactNum();

	for (i=0; i<DONum; ++i)
	{
		cpdos  = cpDOWorld->GetDOStatus(pConsultant->GetDO(i));
		cpdoml = cpDOWorld->GetDOModel(cpdos->GetDOName());
		cDO.Add(cpAssembler->CreateDiscreteObject(cpdoml, cpdos));
	}

	if (pbc->Active())
	{
		for (i=0; i<IactNum; ++i)
		{
			ulMaster = pConsultant->GetIactMaster(i);
			ulSlave  = pConsultant->GetIactSlave(i);

			cpiactml
				= cpDOWorld
					->GetIactModel
						(cDO[ulMaster]->GetDOModel()->GetDOGroup(),
						cDO[ulSlave]->GetDOModel()->GetDOGroup()   );

			Interaction* pInteraction
				= cpAssembler
					->CreateInteraction(cDO[ulMaster], cDO[ulSlave], cpiactml);

			if (pInteraction != 0)   // Aries added this judgement
			{
				pInteraction->SetPeriodicBoundaryConditions(pbc);
				pInteraction->DetectContact();

				const ImpactStatus* status
					= pConsultant->RetrieveImpactStatus(ulMaster, ulSlave);

				if (status != 0)
				{
					pInteraction->SetImpactStatus(status);
				}

				cIact.Add(pInteraction);
			};
		}
	}
	else
	{
		for (i=0; i<IactNum; ++i)
		{
			ulMaster = pConsultant->GetIactMaster(i);
			ulSlave  = pConsultant->GetIactSlave(i);

			cpiactml
				= cpDOWorld
					->GetIactModel
						(cDO[ulMaster]->GetDOModel()->GetDOGroup(),
						cDO[ulSlave]->GetDOModel()->GetDOGroup()   );

			Interaction* pInteraction
				= cpAssembler
					->CreateInteraction(cDO[ulMaster], cDO[ulSlave], cpiactml);

			if (pInteraction != 0)   // Aries added this judgement
			{
				const ImpactStatus* status
					= pConsultant->RetrieveImpactStatus(ulMaster, ulSlave);

				if (status != 0)
				{
					pInteraction->SetImpactStatus(status);
				}

				cIact.Add(pInteraction);
			};
		}
	}
	// Aries' Debug
	//cIact.Dump("Interaction.txt");
};

bool SimMediator::Run()
{
	time(&starttime);

	const DOWorld*         cpDOWorld = pConsultant->GetDOWorld();
	const SystemParameter* csp       = cpDOWorld->GetSystemParameter();

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime = endtime;

	cIact.CalculateImpact(csp->GetTimeInterval());

	time(&endtime);
	timeImpactSolving += (endtime - starttime);
	starttime = endtime;

	// Attention synchronize external impact
	pConsultant->SyncDOContainer(cDO);

	time(&endtime);
	timeSyncDOContainer += (endtime - starttime);
	starttime = endtime;

	cDO.AddFieldImpact(csp->GetFieldForce() * csp->GetTimeInterval());
	cDO.AddConstrainedImpact(csp->GetTimeInterval());

	time(&endtime);
	timeFieldForceAdding += (endtime - starttime);
	starttime = endtime;

	cDO.Response(csp->GetTimeInterval());
	cDO.EnforcePeriodicBoundaryConditions(csp->GetPeriodicBoundaryConditions());

	time(&endtime);
	timeResponseUpdating += (endtime - starttime);
	starttime = endtime;

	cIact.CheckContactStatus();

	time(&endtime);
	timeContactDetection += (endtime - starttime);
	starttime = endtime;

	pConsultant->ResetTimePartitioning();
	bool bContinue = pConsultant->NextStep(cDO, cIact);

	time(&endtime);
	timePartitioning += (pConsultant->timePartitioning);
	timeNextStep += (endtime - starttime - (pConsultant->timePartitioning));
	starttime = endtime;

	// Attention check if it has to rebuilder
	if (pConsultant->ISReset())
	{
		CalculateSystemEnergy();
		Initiate();
	};

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (pConsultant->ISRecord())
	{
		timeComputing
			= timeSystem
			+ timeImpactSolving
			+ timeFieldForceAdding
			+ timeResponseUpdating
			+ timeContactDetection;
		timeCommunication
			= timeSyncDOContainer
			+ timePartitioning;
		if(NP == 1)
		{
			timeComputing += timeNextStep;
		}
		else
		{
			timeCommunication += timeNextStep;
		}
		timeTotal = timeComputing + timeCommunication;
		FileLog
			<< rank                                                              << ", "
			<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent() << ", "
			<< timeSystem                                                        << ", "
			<< timeImpactSolving                                                 << ", "
			<< timeFieldForceAdding                                              << ", "
			<< timeResponseUpdating                                              << ", "
			<< timeContactDetection                                              << ", "
			<< timeNextStep                                                      << ", "
			<< timeSyncDOContainer                                               << ", "
			<< timePartitioning                                                  << ", "
			<< timeComputing                                                     << ", "
			<< timeCommunication                                                 << ", "
			<< timeTotal                                                         << endl;
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido");
		pConsultant->GetIactRecordTab()->WriteIRT("terminate.irt");
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		(pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop())      );
};

bool SimMediator::Run
	(const vector<pair<NJRvector3d, NJRvector3d> >& vvExternalImpact)
{
	time(&starttime);

	const DOWorld*         cpDOWorld = pConsultant->GetDOWorld();
	const SystemParameter* csp       = cpDOWorld->GetSystemParameter();

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime = endtime;

	cIact.CalculateImpact(csp->GetTimeInterval());

	time(&endtime);
	timeImpactSolving += (endtime - starttime);
	starttime = endtime;

	// Attention synchronize external impact
	pConsultant->SyncDOContainer(cDO);

	time(&endtime);
	timeSyncDOContainer += (endtime - starttime);
	starttime = endtime;

	cDO.AddFieldImpact(csp->GetFieldForce() * csp->GetTimeInterval());
	for(unsigned long i=0; i<cDO.size(); i++)
	{
		cDO.AddImpact
			(i,
			 vvExternalImpact[pConsultant->GetDO(i)].first,
			 vvExternalImpact[pConsultant->GetDO(i)].second);
	};

	time(&endtime);
	timeFieldForceAdding += (endtime - starttime);
	starttime = endtime;

	cDO.Response(csp->GetTimeInterval());
	cDO.EnforcePeriodicBoundaryConditions(csp->GetPeriodicBoundaryConditions());   // PBC by Liao 2009/5/27

	time(&endtime);
	timeResponseUpdating += (endtime - starttime);
	starttime = endtime;

	cIact.CheckContactStatus();

	time(&endtime);
	timeContactDetection += (endtime - starttime);
	starttime = endtime;

	pConsultant->ResetTimePartitioning();
	bool bContinue = pConsultant->NextStep(cDO, cIact);

	time(&endtime);
	timePartitioning += (pConsultant->timePartitioning);
	timeNextStep += (endtime - starttime - (pConsultant->timePartitioning));
	starttime = endtime;

	// Attention check if it has to rebuilder
	if (pConsultant->ISReset())
	{
		CalculateSystemEnergy();
		Initiate();
	};

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (pConsultant->ISRecord())
	{
		timeComputing
			= timeSystem
			+ timeImpactSolving
			+ timeFieldForceAdding
			+ timeResponseUpdating
			+ timeContactDetection;
		timeCommunication
			= timeSyncDOContainer
			+ timePartitioning;
		if(NP == 1)
		{
			timeComputing += timeNextStep;
		}
		else
		{
			timeCommunication += timeNextStep;
		}
		timeTotal = timeComputing + timeCommunication;
		FileLog
			<< rank                                                              << ", "
			<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent() << ", "
			<< timeSystem                                                        << ", "
			<< timeImpactSolving                                                 << ", "
			<< timeFieldForceAdding                                              << ", "
			<< timeResponseUpdating                                              << ", "
			<< timeContactDetection                                              << ", "
			<< timeNextStep                                                      << ", "
			<< timeSyncDOContainer                                               << ", "
			<< timePartitioning                                                  << ", "
			<< timeComputing                                                     << ", "
			<< timeCommunication                                                 << ", "
			<< timeTotal                                                         << endl;
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido");
		pConsultant->GetIactRecordTab()->WriteIRT("terminate.irt");
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		(pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop())      );
};

bool SimMediator::ReDistribute()
{
	time(&starttime);

	const DOWorld*         cpDOWorld = pConsultant->GetDOWorld();
	const SystemParameter* csp       = cpDOWorld->GetSystemParameter();

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime = endtime;

	cIact.CalculateImpact(csp->GetTimeInterval());

	time(&endtime);
	timeImpactSolving += (endtime - starttime);
	starttime = endtime;

	// Attention synchronize external impact
	pConsultant->SyncDOContainer(cDO);

	time(&endtime);
	timeSyncDOContainer += (endtime - starttime);
	starttime = endtime;

/*
	cDO.AddFieldImpact(csp->GetFieldForce() * csp->GetTimeInterval());

	time(&endtime);
	timeFieldForceAdding += (endtime - starttime);
	starttime = endtime;
*/

	cDO.Response(csp->GetTimeInterval());
	cDO.EnforcePeriodicBoundaryConditions(csp->GetPeriodicBoundaryConditions());

	// Freeze all elements
	for (unsigned long ul=0; ul<cDO.size(); ul++)
	{
		cDO[ul]->SetVelocity(NJRvector3d(ZERO));
		cDO[ul]->SetAngularVelocity(NJRvector3d(ZERO));
	}

	// Check the number of contact pairs
	if (pConsultant->ISReset())
	{
		if (rank == 0)
		{
			ofstream FileContactNumber("contact_number.txt", ios::app);
			unsigned long ulContactPairNumber = pConsultant->ContactNumber();
			FileContactNumber
				<< "Time = "
				<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()
				<< "; Number of contacts = "
				<< ulContactPairNumber
				<< endl;
			FileContactNumber.close();
			if (ulContactPairNumber == 0)
			{
//				pConsultant->GetDOWorld()->WriteXML("terminate.xml");
				pConsultant->GetDOWorld()->WriteIDO("terminate.ido");
				pConsultant->GetIactRecordTab()->WriteIRT("terminate.irt");
				return false;
			}
		}
		// Clean all interactions
		for (unsigned long ul=0; ul<cIact.size(); ul++)
		{
			cIact.CleanSolverStatus(ul);
		}
	}

	// Clean all interactions
/*
	for (unsigned long ul=0; ul<cIact.size(); ul++)
	{
		cIact.CleanSolverStatus(ul);
	}
*/

	time(&endtime);
	timeResponseUpdating += (endtime - starttime);
	starttime = endtime;

	cIact.CheckContactStatus();

	time(&endtime);
	timeContactDetection += (endtime - starttime);
	starttime = endtime;

	pConsultant->ResetTimePartitioning();
	bool bContinue = pConsultant->NextStep(cDO, cIact);

	time(&endtime);
	timePartitioning += (pConsultant->timePartitioning);
	timeNextStep += (endtime - starttime - (pConsultant->timePartitioning));
	starttime = endtime;

	// Attention check if it has to rebuilder
	if (pConsultant->ISReset())
	{
		Initiate();
	};

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (pConsultant->ISRecord())
	{
		timeComputing
			= timeSystem
			+ timeImpactSolving
			+ timeFieldForceAdding
			+ timeResponseUpdating
			+ timeContactDetection;
		timeCommunication
			= timeSyncDOContainer
			+ timePartitioning;
		if(NP == 1)
		{
			timeComputing += timeNextStep;
		}
		else
		{
			timeCommunication += timeNextStep;
		}
		timeTotal = timeComputing + timeCommunication;
		FileLog
			<< rank                                                              << ", "
			<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent() << ", "
			<< timeSystem                                                        << ", "
			<< timeImpactSolving                                                 << ", "
			<< timeFieldForceAdding                                              << ", "
			<< timeResponseUpdating                                              << ", "
			<< timeContactDetection                                              << ", "
			<< timeNextStep                                                      << ", "
			<< timeSyncDOContainer                                               << ", "
			<< timePartitioning                                                  << ", "
			<< timeComputing                                                     << ", "
			<< timeCommunication                                                 << ", "
			<< timeTotal                                                         << endl;
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido");
		pConsultant->GetIactRecordTab()->WriteIRT("terminate.irt");
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		(pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop())      );
};

