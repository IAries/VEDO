#include <aries/utility/Constants.h>
#include <vedo/framework/SimMediator.h>
#include <vedo/framework/DOMap.h>
#include <vedo/framework/DOModel.h>
#include <vedo/framework/DOWorld_WriteVTK.h>
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

std::string GetRankString(_uint_t rank, _uint_t NP)
{
    _uint_t dig_total = static_cast<_uint_t>(log10(static_cast<_float_t>(NP))) + 1;
	_uint_t dig;
    if (rank == 0)
    {
    	dig = 1;
    }
    else
    {
	    dig = static_cast<_uint_t>(log10(static_cast<_float_t>(rank))) + 1;
    }
	std::string dig_string = "";
   	for (_uint_t i=0; i<dig; i++)
   	{
		dig_string = char(48+rank%10) + dig_string;
        rank = rank/10;
    }
   	for (_uint_t i=0; i<(dig_total-dig); i++)
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
		const SystemParameter* csp = pConsultant->GetDOWorld()->GetSystemParameter();
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

		aries::Constants* aries_cp = aries::Constants::Instance();
		_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
		for (_uint_t u=0; u<2*uNumUDDImpactStatus; u++)
		{
			FileLogSystemStatus << ", " << pConsultant->GetUserDefinedValue(u);
		}

		FileLogSystemStatus << std::endl;
	}
}

SimMediator::SimMediator(Consultant* Consultant, const Assembler* Assembler):
	pConsultant(Consultant), cpAssembler(Assembler), rank(0), NP(1), bFirstRun(true)
{
	TimeInitiate();
	FileLog.open("time_0.csv", std::ios::out);
	FileLog
		<< "Rank, Simulated time, System, Impact solving, Field force adding, "
		<< "Response updating, Contact detection, Next step, "
		<< "DOContainer synchronization, Partitioning, Computing, "
		<< "Communication, Total, "
		<< "Ratio of System, Ratio of Impact solving, "
		<< "Ratio of field force adding, Ratio of response updating, "
		<< "ratio of contact detection, ratio of next step, "
		<< "ratio of DOContainer synchronization, Ratio of partitioning, "
		<< "Ratio of computing, Ratio of communication"
		<< std::endl
		<< 0 << ", "
		<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent() << ", "
		<< "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0"
		<< std::endl;
	FileLogSystemStatus.open("SystemStatus.csv", std::ios::out);
	FileLogSystemStatus
		<< "Simulated time, number of elements, number of interactions, "
		<< "Number of contacts, "
		<< "System energy, Potential energy, Kinetic energy, "
		<< "Kinetic energy (translation), Kinetic energy (rotation), "
		<< "Minimal velocity, Maximal velocity, "
		<< "Minimal angular velocity, Maximal angular velocity, "
		<< "Norm of momentum, Norm of angular momentum";

	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
	for (_uint_t u=0; u<2*uNumUDDImpactStatus; u++)
	{
		FileLogSystemStatus << ", User-defined Value " << u+1;
	}

	FileLogSystemStatus << std::endl;

	CalculateSystemEnergy();
	Initiate();
//	pConsultant->SyncWorld(cDO);
//	pConsultant->RebuildIactRecordTab(cIact);
	_uint_t ulDOSizeOld = cDO.size();
	if (pConsultant->CleanUp(cDO, cIact))
	{
		pConsultant->Reset();
		_uint_t ulDOSizeNew = cDO.size();
		if (ulDOSizeOld != ulDOSizeNew)
		{
			CalculateSystemEnergy();
			Initiate();
		}
	}
}

SimMediator::SimMediator(Consultant* Consultant, const Assembler* Assembler, const _uint_t r, const _uint_t np):
	pConsultant(Consultant), cpAssembler(Assembler), rank(r), NP(np), bFirstRun(true)
{
	pConsultant->SetRankNP(r, np);
	TimeInitiate();
	std::string name = "time_" + GetRankString(r, np);
	name += ".csv";
	FileLog.open(name.c_str(), std::ios::out);
	FileLog
		<< "Rank, Simulated time, System, Impact solving, Field force adding, "
		<< "Response updating, Contact detection, Next step, "
		<< "DOContainer synchronization, Partitioning, Computing, "
		<< "Communication, Total, "
		<< "Ratio of System, Ratio of Impact solving, "
		<< "Ratio of field force adding, Ratio of response updating, "
		<< "ratio of contact detection, ratio of next step, "
		<< "ratio of DOContainer synchronization, Ratio of partitioning, "
		<< "Ratio of computing, Ratio of communication"
		<< std::endl
		<< rank << ", "
		<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent() << ", "
		<< "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0"
		<< std::endl;

	if (rank == 0)
	{
		FileLogSystemStatus.open("SystemStatus.csv", std::ios::out);
        FileLogSystemStatus
            << "Simulated time, number of elements, number of interactions, "
            << "Number of contacts, "
            << "System energy, Potential energy, Kinetic energy, "
            << "Kinetic energy (translation), Kinetic energy (rotation), "
            << "Minimal velocity, Maximal velocity, "
            << "Minimal angular velocity, Maximal angular velocity, "
            << "Norm of momentum, Norm of angular momentum";

		aries::Constants* aries_cp = aries::Constants::Instance();
		_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
		for (_uint_t u=0; u<2*uNumUDDImpactStatus; u++)
		{
			FileLogSystemStatus << ", User-defined Value " << u+1;
		}

		FileLogSystemStatus << std::endl;
	}

	CalculateSystemEnergy();
	Initiate();
//	pConsultant->SyncWorld(cDO);
//	pConsultant->RebuildIactRecordTab(cIact);
	_uint_t ulDOSizeOld = cDO.size();
	if (pConsultant->CleanUp(cDO, cIact))
	{
		pConsultant->Reset();
		_uint_t ulDOSizeNew = cDO.size();
		if (ulDOSizeOld != ulDOSizeNew)
		{
			CalculateSystemEnergy();
			Initiate();
		}
	}
}

SimMediator::~SimMediator()
{
	FileLog.close();
	if (rank == 0)
	{
		FileLogSystemStatus.close();
	}
}

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

/*
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

	for(_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
		FileInteraction << ", User-defined value " << u+1;

	FileInteraction << std::endl;

	const Interaction*  iap = 0;
	const ContactInfo*  cip = 0;
	const ImpactStatus* isp = 0;
	Vector3df
		vImpactMaster, vImpactSlave, vAngularImpactMaster, vAngularImpactSlave;
	const _float_t* cdpudv;

	if (NP == 1)
	{
		for(_uint_t ul=0; ul<cIact.size(); ul++)
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

				for(_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
					FileInteraction << ", " << *(cdpudv+u);

				FileInteraction << std::endl;
			}
		}
	}
	else
	{
		for(_uint_t ul=0; ul<cIact.size(); ul++)
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

				for(_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
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
	Vector3df
		vPosition, vVelocity, vAngularVelocity,
		vOrientationX, vOrientationZ,
		vImpact, vAngularImpact;

	if (NP == 1)
	{
		for(_uint_t ul=0; ul<cDO.size(); ul++)
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
		for(_uint_t ul=0; ul<cDO.size(); ul++)
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
*/

void SimMediator::WriteInteractionForce
	(const char* filename, const std::vector<std::pair<Vector3df, Vector3df> >* vvExternalImpact)
{
	const SystemParameter* csp = pConsultant->GetDOWorld()->GetSystemParameter();
	const _float_t     dt  = csp->GetTimeInterval();

	const DiscreteObject* dop = 0;

	// Calculate the force due to interaction
	//cIact.CalculateImpact(csp->GetTimeInterval());   // It has been done in the function "ShowImpact"

	std::vector<_float_t> iactForce_vec, extForce_vec, fieldForce_vec, totalForce_vec;

	for (_uint_t ul=0; ul<cDO.size(); ul++)
	{
		dop = cDO.GetDiscreteObject(ul);

		if (dop->GetDOModel()->GetBehavior() != mobile)
		{
			continue;
		}

		Vector3df iactForce  = dop->GetImpact() * (1.0/dt);
		Vector3df fieldForce = csp->GetFieldAcceleration();
		Vector3df extForce   = vvExternalImpact ? (*vvExternalImpact)[pConsultant->GetDO(ul)].first * (1./dt) : Vector3df();
		Vector3df totalForce = iactForce + fieldForce + extForce;

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

    #ifdef _STD_CPP_11
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
    #endif   // _STD_CPP_11
}

void SimMediator::SetDOStatus(const _uint_t& odo, const DOStatus& dos)
{
	if (odo < cDO.size())
	{
		cDO.SetDOStatus(odo, dos);
	}
	else
	{
		std::cerr
			<< "Error!! Code: void SimMediator::SetDOStatus(const _uint_t&, const DOStatus&)" << std::endl
			<< "        Note: Element's ID wrong"                                             << std::endl;
		exit(-1);
	}
}

void SimMediator::Initiate()
{
	cDO.Clear();
	cIact.Clear();

	_uint_t i, ulMaster, ulSlave;

	const DOModel*   cpdoml;
	const DOStatus*  cpdos;
	const IactModel* cpiactml;
	const DOWorld*   cpDOWorld = pConsultant->GetDOWorld();
	const Boundary*  pbc       = &(cpDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions());

	_uint_t DONum   = pConsultant->GetDONum();
	_uint_t IactNum = pConsultant->GetIactNum();

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
			cpiactml = cpDOWorld->GetIactModel(cDO[ulMaster]->GetDOModel()->GetDOGroup(), cDO[ulSlave]->GetDOModel()->GetDOGroup());
			Interaction* pInteraction = cpAssembler->CreateInteraction(cDO[ulMaster], cDO[ulSlave], cpiactml);

			if (pInteraction != 0)   // Aries added this judgement
			{
				pInteraction->SetPeriodicBoundaryConditions(pbc);
				pInteraction->DetectContact();

				const ImpactStatus* status = pConsultant->RetrieveImpactStatus(ulMaster, ulSlave);

				if (status != 0)
				{
					pInteraction->SetImpactStatus(status);
				}

				cIact.Add(pInteraction);
			}
		}
	}
	else
	{
		for (i=0; i<IactNum; ++i)
		{
			ulMaster = pConsultant->GetIactMaster(i);
			ulSlave  = pConsultant->GetIactSlave(i);
			cpiactml = cpDOWorld->GetIactModel(cDO[ulMaster]->GetDOModel()->GetDOGroup(), cDO[ulSlave]->GetDOModel()->GetDOGroup());
			Interaction* pInteraction = cpAssembler->CreateInteraction(cDO[ulMaster], cDO[ulSlave], cpiactml);

			if (pInteraction != 0)   // Aries added this judgement
			{
				const ImpactStatus* status = pConsultant->RetrieveImpactStatus(ulMaster, ulSlave);
				if (status != 0)
				{
					pInteraction->SetImpactStatus(status);
				}

				cIact.Add(pInteraction);
			}
		}
	}

	// Aries' Debug
	//cIact.Dump("Interaction.txt");
}

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

	if (pConsultant->ISRecord() || pConsultant->ISReset())
	{
		if (rank == 0)
		{
			std::cout << "Simulated time: " << csp->GetTimeCurrent() << " / " << csp->GetTimeStop() << std::endl;
		}
		CalculateSystemEnergy();
	}

	// Attention check if it has to rebuilder
	if (pConsultant->ISReset())
	{
		Initiate();
	}

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (pConsultant->ISRecord())
	{
		timeComputing     = timeSystem + timeImpactSolving + timeFieldForceAdding + timeResponseUpdating + timeContactDetection;
		timeCommunication = timeSyncDOContainer + timePartitioning;
		if (NP == 1)
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
			<< timeTotal                                                         << ", ";
        if (timeTotal != 0.0)
        {
            FileLog
			<< timeSystem/timeTotal                                              << ", "
			<< timeImpactSolving/timeTotal                                       << ", "
			<< timeFieldForceAdding/timeTotal                                    << ", "
			<< timeResponseUpdating/timeTotal                                    << ", "
			<< timeContactDetection/timeTotal                                    << ", "
			<< timeNextStep/timeTotal                                            << ", "
			<< timeSyncDOContainer/timeTotal                                     << ", "
			<< timePartitioning/timeTotal                                        << ", "
			<< timeComputing/timeTotal                                           << ", "
			<< timeCommunication/timeTotal                                       << std::endl;
        }
        else
        {
            FileLog
			<< "0, 0, 0, 0, 0, 0, 0, 0, 0, 0"                                    << std::endl;
        }
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		 (pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop()   )   );
}

bool SimMediator::Run(const std::map<_uint_t, DOStatus>& AssignedDOStatus, const std::vector<std::pair<Vector3df, Vector3df> >& vvExternalImpact)
{
	time(&starttime);

	const DOWorld*         cpDOWorld = pConsultant->GetDOWorld();
	const SystemParameter* csp       = cpDOWorld->GetSystemParameter();

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime   = endtime;

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
	for (_uint_t i=0; i<cDO.size(); i++)
	{
		cDO.AddImpact(i, vvExternalImpact[pConsultant->GetDO(i)].first, vvExternalImpact[pConsultant->GetDO(i)].second);
	}

	time(&endtime);
	timeFieldForceAdding += (endtime - starttime);
	starttime             = endtime;

	cDO.Response(csp->GetTimeInterval());
	std::map<_uint_t, DOStatus>::const_iterator musPos;
	for (_uint_t i=0; i<cDO.size(); i++)
	{
		musPos = AssignedDOStatus.find(pConsultant->GetDO(i));
		if (musPos != AssignedDOStatus.end())
		{
			cDO.SetDOStatus(i, musPos->second);
		}
	}
	cDO.EnforcePeriodicBoundaryConditions(csp->GetPeriodicBoundaryConditions());

	time(&endtime);
	timeResponseUpdating += (endtime - starttime);
	starttime             = endtime;

	cIact.CheckContactStatus();

	time(&endtime);
	timeContactDetection += (endtime - starttime);
	starttime             = endtime;

	pConsultant->ResetTimePartitioning();
	bool bContinue = pConsultant->NextStep(cDO, cIact);

	time(&endtime);
	timePartitioning += (pConsultant->timePartitioning);
	timeNextStep     += (endtime - starttime - (pConsultant->timePartitioning));
	starttime         = endtime;

	if (pConsultant->ISRecord() || pConsultant->ISReset())
	{
		if (rank == 0)
		{
			std::cout << "Simulated time: " << csp->GetTimeCurrent() << " / " << csp->GetTimeStop() << std::endl;
		}
		CalculateSystemEnergy();
	}

	// Attention check if it has to rebuilder
	if (pConsultant->ISReset())
	{
		Initiate();
	}

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (pConsultant->ISRecord())
	{
		timeComputing     = timeSystem + timeImpactSolving + timeFieldForceAdding + timeResponseUpdating + timeContactDetection;
		timeCommunication = timeSyncDOContainer + timePartitioning;
		if (NP == 1)
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
			<< timeTotal                                                         << ", ";
        if (timeTotal != 0.0)
        {
            FileLog
			<< timeSystem/timeTotal                                              << ", "
			<< timeImpactSolving/timeTotal                                       << ", "
			<< timeFieldForceAdding/timeTotal                                    << ", "
			<< timeResponseUpdating/timeTotal                                    << ", "
			<< timeContactDetection/timeTotal                                    << ", "
			<< timeNextStep/timeTotal                                            << ", "
			<< timeSyncDOContainer/timeTotal                                     << ", "
			<< timePartitioning/timeTotal                                        << ", "
			<< timeComputing/timeTotal                                           << ", "
			<< timeCommunication/timeTotal                                       << std::endl;
        }
        else
        {
            FileLog
			<< "0, 0, 0, 0, 0, 0, 0, 0, 0, 0"                                    << std::endl;
        }
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		 (pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop()   )   );
}

bool SimMediator::Run(const std::vector<std::pair<Vector3df, Vector3df> >& vvExternalImpact)
{
	time(&starttime);

	const DOWorld*         cpDOWorld = pConsultant->GetDOWorld();
	const SystemParameter* csp       = cpDOWorld->GetSystemParameter();

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime   = endtime;

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
	for (_uint_t i=0; i<cDO.size(); i++)
	{
		cDO.AddImpact(i, vvExternalImpact[pConsultant->GetDO(i)].first, vvExternalImpact[pConsultant->GetDO(i)].second);
	}

	time(&endtime);
	timeFieldForceAdding += (endtime - starttime);
	starttime             = endtime;

	cDO.Response(csp->GetTimeInterval());
	cDO.EnforcePeriodicBoundaryConditions(csp->GetPeriodicBoundaryConditions());

	time(&endtime);
	timeResponseUpdating += (endtime - starttime);
	starttime             = endtime;

	cIact.CheckContactStatus();

	time(&endtime);
	timeContactDetection += (endtime - starttime);
	starttime             = endtime;

	pConsultant->ResetTimePartitioning();
	bool bContinue = pConsultant->NextStep(cDO, cIact);

	time(&endtime);
	timePartitioning += (pConsultant->timePartitioning);
	timeNextStep     += (endtime - starttime - (pConsultant->timePartitioning));
	starttime         = endtime;

	if (pConsultant->ISRecord() || pConsultant->ISReset())
	{
		if (rank == 0)
		{
			std::cout << "Simulated time: " << csp->GetTimeCurrent() << " / " << csp->GetTimeStop() << std::endl;
		}
		CalculateSystemEnergy();
	}

	// Attention check if it has to rebuilder
	if (pConsultant->ISReset())
	{
		Initiate();
	}

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (pConsultant->ISRecord())
	{
		timeComputing     = timeSystem + timeImpactSolving + timeFieldForceAdding + timeResponseUpdating + timeContactDetection;
		timeCommunication = timeSyncDOContainer + timePartitioning;
		if (NP == 1)
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
			<< timeTotal                                                         << ", ";
        if (timeTotal != 0.0)
        {
            FileLog
			<< timeSystem/timeTotal                                              << ", "
			<< timeImpactSolving/timeTotal                                       << ", "
			<< timeFieldForceAdding/timeTotal                                    << ", "
			<< timeResponseUpdating/timeTotal                                    << ", "
			<< timeContactDetection/timeTotal                                    << ", "
			<< timeNextStep/timeTotal                                            << ", "
			<< timeSyncDOContainer/timeTotal                                     << ", "
			<< timePartitioning/timeTotal                                        << ", "
			<< timeComputing/timeTotal                                           << ", "
			<< timeCommunication/timeTotal                                       << std::endl;
        }
        else
        {
            FileLog
			<< "0, 0, 0, 0, 0, 0, 0, 0, 0, 0"                                    << std::endl;
        }
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		 (pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop()   )   );
}

bool SimMediator::Run(const std::map<_uint_t, DOStatus>& AssignedDOStatus)
{
	time(&starttime);

	const DOWorld*         cpDOWorld = pConsultant->GetDOWorld();
	const SystemParameter* csp       = cpDOWorld->GetSystemParameter();

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime   = endtime;

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
	starttime             = endtime;

	cDO.Response(csp->GetTimeInterval());
	std::map<_uint_t, DOStatus>::const_iterator musPos;
	for (_uint_t i=0; i<cDO.size(); i++)
	{
		musPos = AssignedDOStatus.find(pConsultant->GetDO(i));
		if (musPos != AssignedDOStatus.end())
		{
			cDO.SetDOStatus(i, musPos->second);
		}
	}
	cDO.EnforcePeriodicBoundaryConditions(csp->GetPeriodicBoundaryConditions());

	time(&endtime);
	timeResponseUpdating += (endtime - starttime);
	starttime             = endtime;

	cIact.CheckContactStatus();

	time(&endtime);
	timeContactDetection += (endtime - starttime);
	starttime             = endtime;

	pConsultant->ResetTimePartitioning();
	bool bContinue = pConsultant->NextStep(cDO, cIact);

	time(&endtime);
	timePartitioning += (pConsultant->timePartitioning);
	timeNextStep     += (endtime - starttime - (pConsultant->timePartitioning));
	starttime         = endtime;

	if (pConsultant->ISRecord() || pConsultant->ISReset())
	{
		if (rank == 0)
		{
			std::cout << "Simulated time: " << csp->GetTimeCurrent() << " / " << csp->GetTimeStop() << std::endl;
		}
		CalculateSystemEnergy();
	}

	// Attention check if it has to rebuilder
	if (pConsultant->ISReset())
	{
		Initiate();
	}

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (pConsultant->ISRecord())
	{
		timeComputing     = timeSystem + timeImpactSolving + timeFieldForceAdding + timeResponseUpdating + timeContactDetection;
		timeCommunication = timeSyncDOContainer + timePartitioning;
		if (NP == 1)
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
			<< timeTotal                                                         << ", ";
        if (timeTotal != 0.0)
        {
            FileLog
			<< timeSystem/timeTotal                                              << ", "
			<< timeImpactSolving/timeTotal                                       << ", "
			<< timeFieldForceAdding/timeTotal                                    << ", "
			<< timeResponseUpdating/timeTotal                                    << ", "
			<< timeContactDetection/timeTotal                                    << ", "
			<< timeNextStep/timeTotal                                            << ", "
			<< timeSyncDOContainer/timeTotal                                     << ", "
			<< timePartitioning/timeTotal                                        << ", "
			<< timeComputing/timeTotal                                           << ", "
			<< timeCommunication/timeTotal                                       << std::endl;
        }
        else
        {
            FileLog
			<< "0, 0, 0, 0, 0, 0, 0, 0, 0, 0"                                    << std::endl;
        }
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		 (pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop()   )   );
}

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
	for (_uint_t ul=0; ul<cDO.size(); ul++)
	{
		cDO[ul]->SetVelocity(Vector3df());
		cDO[ul]->SetAngularVelocity(Vector3df());
	}

	// Check the number of contact pairs
	if (pConsultant->ISReset())
	{
		if (rank == 0)
		{
			std::ofstream FileContactNumber("contact_number.txt", std::ios::app);
			_uint_t ulContactPairNumber = pConsultant->ContactNumber();
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

		if (pConsultant->ISRecord() || pConsultant->ISReset())
		{
			if (rank == 0)
			{
				std::cout << "Simulated time: " << csp->GetTimeCurrent() << " / " << csp->GetTimeStop() << std::endl;
			}
			CalculateSystemEnergy();
		}

		// Clean all interactions
		for (_uint_t ul=0; ul<cIact.size(); ul++)
		{
			cIact.CleanSolverStatus(ul);
		}

		bFirstRun = false;
	}

	// Clean all interactions
/*
	for(_uint_t ul=0; ul<cIact.size(); ul++)
	{
		cIact.CleanSolverStatus(ul);
	}
*/

	time(&endtime);
	timeResponseUpdating += (endtime - starttime);
	starttime             = endtime;

	cIact.CheckContactStatus();

	time(&endtime);
	timeContactDetection += (endtime - starttime);
	starttime             = endtime;

	pConsultant->ResetTimePartitioning();
	bool bContinue = pConsultant->NextStep(cDO, cIact);

	time(&endtime);
	timePartitioning += (pConsultant->timePartitioning);
	timeNextStep     += (endtime - starttime - (pConsultant->timePartitioning));
	starttime         = endtime;

	// Attention check if it has to rebuilder
	if (pConsultant->ISReset())
	{
		Initiate();
	}

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (pConsultant->ISRecord())
	{
		timeComputing     = timeSystem + timeImpactSolving + timeFieldForceAdding + timeResponseUpdating + timeContactDetection;
		timeCommunication = timeSyncDOContainer + timePartitioning;
		if (NP == 1)
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
			<< timeTotal                                                         << ", ";
        if (timeTotal != 0.0)
        {
            FileLog
			<< timeSystem/timeTotal                                              << ", "
			<< timeImpactSolving/timeTotal                                       << ", "
			<< timeFieldForceAdding/timeTotal                                    << ", "
			<< timeResponseUpdating/timeTotal                                    << ", "
			<< timeContactDetection/timeTotal                                    << ", "
			<< timeNextStep/timeTotal                                            << ", "
			<< timeSyncDOContainer/timeTotal                                     << ", "
			<< timePartitioning/timeTotal                                        << ", "
			<< timeComputing/timeTotal                                           << ", "
			<< timeCommunication/timeTotal                                       << std::endl;
        }
        else
        {
            FileLog
			<< "0, 0, 0, 0, 0, 0, 0, 0, 0, 0"                                    << std::endl;
        }
	}

	if (!bContinue)
	{
//		pConsultant->GetDOWorld()->WriteXML("terminate.xml");
		pConsultant->GetDOWorld()->WriteIDO("terminate.ido", pConsultant->GetIactRecordTab());
		return false;
	}

	return
		((pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()) <
		 (pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeStop()   )   );
}

}   // namespace vedo
