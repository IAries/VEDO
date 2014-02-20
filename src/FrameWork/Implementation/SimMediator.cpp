#include <Framework/Interfaces/Constants.h>
#include <Framework/Interfaces/SimMediator.h>
#include <Framework/Interfaces/DOMap.h>
#include <Framework/Interfaces/DOWorld_WriteVTK.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iostream>
#include <iterator>
#include <map>

namespace vedo
{

DataFieldVTKWriter* DataFieldVTKWriter::_instance = 0;

std::string GetRankString(int rank, int NP)
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
	std::string dig_string = "";
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
		FileLogSystemStatus
			<< csp->GetTimeCurrent()                            << ", "
			<< csp->GetDONumber()                               << ", "
            << pConsultant->GetIactRecordTab()->GetTabSize()    << ", "
            << pConsultant->GetIactRecordTab()->ContactNumber() << ", "
			<< csp->GetEnergyPotential()
			 + csp->GetEnergyTranslation()
			 + csp->GetEnergyRotation()                         << ", "
			<< csp->GetEnergyPotential()                        << ", "
			<< csp->GetEnergyTranslation()
			 + csp->GetEnergyRotation()                         << ", "
			<< csp->GetEnergyTranslation()                      << ", "
			<< csp->GetEnergyRotation()                         << ", "
			<< csp->GetVelocityMin()                            << ", "
			<< csp->GetVelocityMax()                            << ", "
			<< csp->GetAngularVelocityMin()                     << ", "
			<< csp->GetAngularVelocityMax()                     << ", "
			<< csp->GetMomentumNorm()                           << ", "
			<< csp->GetAngularMomentumNorm();

		for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
			FileLogSystemStatus << ", " << pConsultant->GetUserDefinedValue(u);

		FileLogSystemStatus << std::endl;
	};
};

SimMediator::SimMediator
	(Consultant* Consultant, const Assembler* Assembler)
	: pConsultant(Consultant), cpAssembler(Assembler), rank(0), NP(1), bFirstRun(true)
{
	TimeInitiate();
	FileLog.open("time_0.csv", std::ios::out);
	FileLog
		<< "Rank, SimulatedTime, System, ImpactSolving, FieldForceAdding, "
		<< "ResponseUpdating, ContactDetection, NextStep, "
		<< "DOContainerSynchronization, Partitioning, Computing, "
		<< "Communication, Total"
		<< std::endl;
	FileLogSystemStatus.open("SystemStatus.csv", std::ios::out);
	FileLogSystemStatus
		<< "SystemTime, ElementNumber, InteractionNumber, ContactNumber, "
		<< "SystemEnergy, PotentialEnergy, KineticEnergy, "
		<< "KineticEnergyTranslation, KineticEnergyRotation, MinimalVelocity, "
		<< "MaximalVelocity, MinimalAngularVelocity, MaximalAngularVelocity, "
		<< "NormMomentum, NormAngularMomentum";

	for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
		FileLogSystemStatus << ", User-defined Value " << u+1;

	FileLogSystemStatus << std::endl;

	CalculateSystemEnergy();
	Initiate();
//	pConsultant->SyncWorld(cDO);
//	pConsultant->RebuildIactRecordTab(cIact);
	unsigned long ulDOSizeOld = cDO.size();
	if (pConsultant->CleanUp(cDO, cIact))
	{
		pConsultant->Reset();
		unsigned long ulDOSizeNew = cDO.size();
		if(ulDOSizeOld != ulDOSizeNew)
		{
			CalculateSystemEnergy();
			Initiate();
		}
	}
};

SimMediator::SimMediator
	(Consultant* Consultant,
	const Assembler* Assembler,
	const unsigned int r,
	const unsigned int np)
	: pConsultant(Consultant), cpAssembler(Assembler), rank(r), NP(np), bFirstRun(true)
{
	pConsultant->SetRankNP(r, np);
	TimeInitiate();
	std::string name = "time_" + GetRankString(r, np);
	name += ".csv";
	FileLog.open(name.c_str(), std::ios::out);
	FileLog
		<< "Rank, SimulatedTime, System, ImpactSolving, FieldForceAdding, "
		<< "ResponseUpdating, ContactDetection, NextStep, "
		<< "DOContainerSynchronization, Partitioning, Computing, Communication, Total"
		<< std::endl;

	if (rank == 0)
	{
		FileLogSystemStatus.open("SystemStatus.csv", std::ios::out);
		FileLogSystemStatus
			<< "SystemTime, ElementNumber, InteractionNumber, ContactNumber, "
			<< "SystemEnergy, PotentialEnergy, KineticEnergy, "
			<< "KineticEnergyTranslation, KineticEnergyRotation, MinimalVelocity, "
			<< "MaximalVelocity, MinimalAngularVelocity, MaximalAngularVelocity, "
			<< "NormMomentum, NormAngularMomentum, ";

		for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
			FileLogSystemStatus << ", User-defined Value " << u+1;

		FileLogSystemStatus << std::endl;
	};

	CalculateSystemEnergy();
	Initiate();
//	pConsultant->SyncWorld(cDO);
//	pConsultant->RebuildIactRecordTab(cIact);
	unsigned long ulDOSizeOld = cDO.size();
	if (pConsultant->CleanUp(cDO, cIact))
	{
		pConsultant->Reset();
		unsigned long ulDOSizeNew = cDO.size();
		if(ulDOSizeOld != ulDOSizeNew)
		{
			CalculateSystemEnergy();
			Initiate();
		}
	}
};

SimMediator::~SimMediator()
{
	FileLog.close();
	if (rank == 0)
		FileLogSystemStatus.close();
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
	std::ofstream          FileInteraction;
	const DOWorld*         cpDOWorld = pConsultant->GetDOWorld();
	const SystemParameter* csp       = cpDOWorld->GetSystemParameter();

	if (NP == 1)
	{
		FileInteraction.open("interaction.csv", std::ios::out);
	}
	else
	{
		char sFileName[256];
		sprintf(sFileName, "interaction_%d.csv\0", rank);
		FileInteraction.open(sFileName, std::ios::out);
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
		<< "Bond, Contacted, RememberedNormalStiffness, RememberedInitialVelocity, "
		<< "RememberedShearForceX, RememberedShearForceY, RememberedShearForceZ";

	for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
		FileInteraction << ", User-defined value " << u+1;

	FileInteraction << std::endl;

	const Interaction*  iap = 0;
	const ContactInfo*  cip = 0;
	const ImpactStatus* isp = 0;
	njr::Vector3d
		vImpactMaster, vImpactSlave, vAngularImpactMaster, vAngularImpactSlave;
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
				// Retrieve existed impace force & angular impact force in DiscreteObjects
				vImpactMaster        = iap->GetMaster()->GetImpact();
				vImpactSlave         = iap->GetSlave()->GetImpact();
				vAngularImpactMaster = iap->GetMaster()->GetAngularImpact();
				vAngularImpactSlave  = iap->GetSlave()->GetAngularImpact();

				cIact.CalculateImpact(csp->GetTimeInterval(), ul);

				// Calculate impace force & angular impact caused by this interaction
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

				isp
					= pConsultant
						->RetrieveImpactStatus
							(pConsultant->GetIactMaster(ul),
							 pConsultant->GetIactSlave(ul)  );
//				isp = iap->GetSolver()->GetImpactStatus();
				if(!isp)
					continue;

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
					<< isp->Kn()              << ", "
					<< isp->InitialVelocity() << ", "
					<< isp->ShearForce().x()  << ", "
					<< isp->ShearForce().y()  << ", "
					<< isp->ShearForce().z();

				for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
					FileInteraction << ", " << *(cdpudv+u);

				FileInteraction << std::endl;
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
				// Retrieve existed impace force & angular impact force in DiscreteObjects
				vImpactMaster        = iap->GetMaster()->GetImpact();
				vImpactSlave         = iap->GetSlave()->GetImpact();
				vAngularImpactMaster = iap->GetMaster()->GetAngularImpact();
				vAngularImpactSlave  = iap->GetSlave()->GetAngularImpact();

				cIact.CalculateImpact(csp->GetTimeInterval(), ul);

				// Calculate impace force & angular impact caused by this interaction
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

				isp
					= pConsultant
						->RetrieveImpactStatus
							(pConsultant->GetIactMaster(ul),
							 pConsultant->GetIactSlave(ul)  );
//				isp = iap->GetSolver()->GetImpactStatus();
				if(!isp)
					continue;

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
					<< isp->Kn()              << ", "
					<< isp->InitialVelocity() << ", "
					<< isp->ShearForce().x()  << ", "
					<< isp->ShearForce().y()  << ", "
					<< isp->ShearForce().z();

				for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
					FileInteraction << ", " << *(cdpudv+u);

				FileInteraction << std::endl;
			}
		}
	}

	FileInteraction.close();

	pConsultant->SyncDOContainer(cDO);
	cDO.AddFieldImpact(csp->GetFieldAcceleration() * csp->GetTimeInterval());
	cDO.AddConstrainedImpact(csp->GetTimeInterval());

	if (NP == 1)
	{
		FileInteraction.open("interaction_element.csv", std::ios::out);
	}
	else
	{
		char sFileName[256];
		sprintf(sFileName, "interaction_element_%d.csv\0", rank);
		FileInteraction.open(sFileName, std::ios::out);
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
		<< std::endl;

	const DiscreteObject* dop = 0;
	const DOStatus*       dos = 0;
	njr::Vector3d
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
				<< vAngularImpact.z()   << std::endl;
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
				<< vAngularImpact.z()     << std::endl;
		}
	}

	FileInteraction.close();
}

void SimMediator::WriteInteractionForce
	(const char* filename,
	 const std::vector<std::pair<njr::Vector3d, njr::Vector3d> >* vvExternalImpact)
{
	const SystemParameter* csp = pConsultant->GetDOWorld()->GetSystemParameter();
	const double           dt  = csp->GetTimeInterval();

	const DiscreteObject* dop = 0;

	// Calculate the force due to interaction
	//cIact.CalculateImpact(csp->GetTimeInterval());   // It has been done in the function "ShowImpact"

	std::vector<double> iactForce_vec, extForce_vec, fieldForce_vec, totalForce_vec;

	for(unsigned long ul=0; ul<cDO.size(); ul++)
	{
		dop = cDO.GetDiscreteObject(ul);

		if (dop->GetDOModel()->GetBehavior() != "mobile")
			continue;

		njr::Vector3d iactForce  = dop->GetImpact() * (1.0/dt);
		njr::Vector3d fieldForce = csp->GetFieldAcceleration();
		njr::Vector3d extForce   = vvExternalImpact ? (*vvExternalImpact)[pConsultant->GetDO(ul)].first * (1./dt) : njr::ZERO;
		njr::Vector3d totalForce = iactForce + fieldForce + extForce;

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
		wrapArr->addArray("FieldAcceleration", 3, fieldForce_vec);
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

	cDO.AddFieldImpact(csp->GetFieldAcceleration() * csp->GetTimeInterval());
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
			<< timeTotal                                                         << std::endl;
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		(pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop())      );
};

bool SimMediator::Run
	(const std::vector<std::pair<njr::Vector3d, njr::Vector3d> >& vvExternalImpact)
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

	cDO.AddFieldImpact(csp->GetFieldAcceleration() * csp->GetTimeInterval());
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
			<< timeTotal                                                         << std::endl;
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
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
		cDO[ul]->SetVelocity(njr::ZERO);
		cDO[ul]->SetAngularVelocity(njr::ZERO);
	}

	// Check the number of contact pairs
	if (pConsultant->ISReset())
	{
		if (rank == 0)
		{
			std::ofstream FileContactNumber("contact_number.txt", std::ios::app);
			unsigned long ulContactPairNumber = pConsultant->ContactNumber();
			FileContactNumber
				<< "Time = "
				<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()
				<< "; Number of contacts = "
				<< ulContactPairNumber
				<< std::endl;
			FileContactNumber.close();
			if (ulContactPairNumber == 0)
			{
				if (!bFirstRun)
				{
					pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
//					pConsultant->GetDOWorld()->WriteXML("terminate.xml", pConsultant->GetIactRecordTab());
					return false;
				}
			}
		}
		// Clean all interactions
		for (unsigned long ul=0; ul<cIact.size(); ul++)
		{
			cIact.CleanSolverStatus(ul);
		}

		bFirstRun = false;
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
			<< timeTotal                                                         << std::endl;
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		(pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop())      );
};

};   // namespace vedo
