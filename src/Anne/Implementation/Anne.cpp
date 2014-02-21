#include <NJR/Interfaces/Utility.h>
#include <Framework/Interfaces/Assembler.h>
#include <Framework/Interfaces/Consultant.h>
#include <Framework/Interfaces/DOWorld.h>
#include <Framework/Interfaces/GeometricShape.h>
#include <Framework/Interfaces/SimMediator.h>
#include <Common/Interfaces/ModuleList.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

void usage (int g)
{
	std::cout
		<< "Anne " << vedo::sVersion << std::endl
		<< std::endl
		<< '\t' << "Usage:" << std::endl
		<< '\t' << "anne <Mode> <IDO file> <Type I > <Record>" << std::endl
		<< '\t' << "anne <Mode> <IDO file> <Type II> <Record> <UpIact>" << std::endl
		<< std::endl
		<< '\t' << "Mode   : analysis/show/redistribute" << std::endl
		<< '\t' << "Type I : safe/near" << std::endl
		<< '\t' << "Type II: nbs/leap" << std::endl
		<< '\t' << "Record : integer" << std::endl
		<< '\t' << "UpIact : integer" << std::endl << std::endl
		<< '\t' << "Error Condition: " << g << std::endl;
	exit(0);
}

int main (int argc, char* argv[])
{
	if (argc < 5)
		usage(1);

	time_t starttime;   // Starting time
	time_t endtime;     // Endind time
	time(&starttime);

	double timeSystem           = 0.0;   // Time of system preparing, starting and ending
	double timeImpactSolving    = 0.0;   // Time of impact solving
	double timeSyncDOContainer  = 0.0;   // Time of "SyncDOContainer"
	double timeFieldForceAdding = 0.0;   // Time of field force adding
	double timeResponseUpdating = 0.0;   // Time of response updating
	double timeContactDetection = 0.0;   // Time of contact detection
	double timeNextStep         = 0.0;   // Time of "NextStep"

	char     mode[256];
	char*    idofilename;
	char     type[256];
	unsigned RecordStep;
	unsigned UpIact;

	sscanf(argv[1], "%s", mode);
	sscanf(argv[3], "%s", type);
	sscanf(argv[4], "%d", &RecordStep);
	if (argc >= 6)
		sscanf(argv[5], "%d", &UpIact);

	vedo::DOWorld*       pDOWorld;
	vedo::Consultant*    pConsultant;
	vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
	vedo::Assembler*     pAssembler = new vedo::Assembler;

	pAssembler->AddDO(new vedo::DOcf<vedo::DOConstrainedQuasiCylinder>(vedo::QuasiCylinder, "constrained"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOConstrainedQuasiPlate>(vedo::QuasiPlate, "constrained"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOFixedQuasiCylinder>(vedo::QuasiCylinder, "fixed"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOFixedQuasiPlate>(vedo::QuasiPlate, "fixed"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOFixedQuasiPlateWithCircularHole>(vedo::QuasiPlateWithCircularHole, "fixed"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOFixedSphere>(vedo::Sphere, "fixed"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOQuasiCylinder>(vedo::QuasiCylinder, "mobile"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOQuasiPlate>(vedo::QuasiPlate, "mobile"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOQuasiPlateWithCircularHole>(vedo::QuasiPlateWithCircularHole, "mobile"));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOSphere>(vedo::Sphere, "mobile"));

	pAssembler->AddIS(new vedo::IScf<vedo::ISwBSDBF>("ISwBSDBF"));
	pAssembler->AddIS(new vedo::IScf<vedo::ISwBtSDBF>("ISwBtSDBF"));

    pAssembler->AddCD(new vedo::CDcf<vedo::CDSphere_Sphere>                     (vedo::Sphere, vedo::Sphere                    , "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
    pAssembler->AddCD(new vedo::CDcf<vedo::CDSphere_QuasiCylinder>              (vedo::Sphere, vedo::QuasiCylinder             , "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
	pAssembler->AddCD(new vedo::CDcf<vedo::CDSphere_QuasiPlate>                 (vedo::Sphere, vedo::QuasiPlate                , "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
	pAssembler->AddCD(new vedo::CDcf<vedo::CDSphere_QuasiPlateWithCircularHole> (vedo::Sphere, vedo::QuasiPlateWithCircularHole, "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));

	if ( !strcmp (strlen(argv[2]) - 4 + argv[2], ".ido") )
	{
		pDOWorld    = new vedo::DOWorld;
		idofilename = argv[2];
		pDOWorld->ReadIDO(idofilename, pIactRecordTab);
	}
	else
	{
		usage(2);
	}

	if (!strcmp(type, "safe"))
	{
		pConsultant = new vedo::SafeConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep);
	}
    else if (!strcmp(type, "near"))
    {
		pConsultant = new vedo::NearConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep);
	}
	else if (!strcmp(type, "nbs"))
	{
		if (argc < 6)
		{
			usage(3);
		}
	  	pConsultant = new vedo::NBSConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
	}
	else if (!strcmp(type, "leap"))
	{
	    if (argc < 6)
	    {
			usage(3);
		}
		pConsultant = new vedo::LeapConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
	}
	else
	{
		usage(5);
	}

	//njr::RunTime("Simulation Start !!");

	vedo::SimMediator sm(pConsultant, pAssembler);

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (!strcmp(mode, "analysis"))
	{
		while (sm.Run());
	}
	else if (!strcmp(mode, "redistribute"))
	{
		while (sm.ReDistribute());
	}
	else if (!strcmp(mode, "show"))
	{
		sm.ShowInteraction();
		std::string vtufile(idofilename);
		vtufile = vtufile.substr(0, vtufile.size() - 4) += ".vtu";
		sm.WriteInteractionForce(vtufile.c_str());
	}
	else
	{
		usage(1);
	}

	time(&starttime);

	//njr::RunTime("Simulation End !!");

	timeSystem           += (sm.timeSystem);
	timeImpactSolving     = (sm.timeImpactSolving);
	timeSyncDOContainer   = (sm.timeSyncDOContainer);
	timeFieldForceAdding  = (sm.timeFieldForceAdding);
	timeResponseUpdating  = (sm.timeResponseUpdating);
	timeContactDetection  = (sm.timeContactDetection);
	timeNextStep          = (sm.timeNextStep);

	time(&endtime);
	timeSystem += (endtime - starttime);

	double timeComputing
		= timeSystem
		+ timeImpactSolving
		+ timeFieldForceAdding
		+ timeResponseUpdating
		+ timeContactDetection
		+ timeNextStep;

	double timeCommunication
		= timeSyncDOContainer;

	double timeTotal = timeComputing + timeCommunication;

	std::ofstream FileLog("time.txt", std::ios::out);
	FileLog
		<< "=============================================================" << std::endl
		<< " Simulated Time: "
		<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()
		<< " seconds" << std::endl
		<< "-------------------------------------------------------------" << std::endl
		<< " Item                                       Time(s) Rate(%)  " << std::endl
		<< "-------------------------------------------------------------" << std::endl
		<< " System                                    "                   << '\t'
		<< timeSystem                                                      << "\t\t"
		<< timeSystem                    /timeTotal*100.0                  << std::endl
		<< " Impact Solving                            "                   << '\t'
	   	<< timeImpactSolving                                               << "\t\t"
		<< timeImpactSolving             /timeTotal*100.0                  << std::endl
		<< " Field Force Adding                        "                   << '\t'
		<< timeFieldForceAdding                                            << "\t\t"
		<< timeFieldForceAdding          /timeTotal*100.0                  << std::endl
		<< " Response Updating                         "                   << '\t'
		<< timeResponseUpdating                                            << "\t\t"
		<< timeResponseUpdating          /timeTotal*100.0                  << std::endl
		<< " Contact Detection                         "                   << '\t'
		<< timeContactDetection                                            << "\t\t"
		<< timeContactDetection          /timeTotal*100.0                  << std::endl
		<< " NextStep                                  "                   << '\t'
		<< timeNextStep                                                    << "\t\t"
		<< timeNextStep                  /timeTotal*100.0                  << std::endl
		<< " DOContainer Synchronization               "                   << '\t'
	   	<< timeSyncDOContainer                                             << "\t\t"
		<< timeSyncDOContainer           /timeTotal*100.0                  << std::endl
		<< "-------------------------------------------------------------" << std::endl
		<< " Computing                                 "                   << '\t'
		<< timeComputing                                                   << "\t\t"
		<< timeComputing                 /timeTotal*100.0                  << std::endl
		<< " Communication                             "                   << '\t'
		<< timeCommunication                                               << "\t\t"
		<< timeCommunication             /timeTotal*100.0                  << std::endl
		<< "-------------------------------------------------------------" << std::endl
		<< " Total                                     "                   << '\t'
		<< timeTotal                                                       << "\t\t"
		<< 100                                                             << std::endl
		<< "=============================================================" << std::endl;
	FileLog.close();

	delete pDOWorld;
	delete pIactRecordTab;
	delete pConsultant;

	return true;
};
