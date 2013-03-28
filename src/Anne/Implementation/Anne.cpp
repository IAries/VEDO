#include <NJR/Interfaces/Utility.h>
#include <FrameWork/Interfaces/Assembler.h>
#include <FrameWork/Interfaces/Consultant.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <FrameWork/Interfaces/GeometricShape.h>
#include <FrameWork/Interfaces/SimMediator.h>

#include <Common/Interfaces/CDSphere2QuasiCylinder.h>
#include <Common/Interfaces/CDSphere2QuasiCylinderAT.h>
#include <Common/Interfaces/CDSphere2QuasiPlate.h>
#include <Common/Interfaces/CDSphere2Sphere.h>
#include <Common/Interfaces/CDSphere2SphereAT.h>

#include <Common/Interfaces/DOConstrainedQuasiCylinder.h>
#include <Common/Interfaces/DOConstrainedQuasiPlate.h>
#include <Common/Interfaces/DOFixedQuasiCylinder.h>
#include <Common/Interfaces/DOFixedQuasiPlate.h>
#include <Common/Interfaces/DOFixedSphere.h>
#include <Common/Interfaces/DOQuasiCylinder.h>
#include <Common/Interfaces/DOQuasiPlate.h>
#include <Common/Interfaces/DOSphere.h>

//#include <Common/Interfaces/GSComplexShape.h>
//#include <Common/Interfaces/GSCylinder.h>
//#include <Common/Interfaces/GSEllipsoid.h>
//#include <Common/Interfaces/GSRectangle.h>
//#include <Common/Interfaces/GSSphere.h>

#include <Common/Interfaces/ISwBSDBF.h>
#include <Common/Interfaces/ISwBtSDBF.h>

#include <Common/Interfaces/LeapConsultant.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <Common/Interfaces/NearConsultant.h>
#include <Common/Interfaces/SafeConsultant.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

void usage (int g)
{
	std::cout
		<< "Anne X3 build 328" << std::endl
		<< std::endl
		<< '\t' << "Usage:" << std::endl
		<< '\t' << "Anne <Mode> <IDO file> <Type I > <Record>" << std::endl
		<< '\t' << "Anne <Mode> <IDO file> <Type II> <Record> <UpIact>" << std::endl
		<< std::endl
		<< '\t' << "Mode   : analysis/show/redistribute" << std::endl
		<< '\t' << "Type I : safe/near" << std::endl
		<< '\t' << "Type II: nbs/leap" << std::endl
		<< '\t' << "Record : integer" << std::endl
		<< '\t' << "UpIact : integer" << std::endl << std::endl
		<< '\t' << "Error Condition: " << g << std::endl;
	exit(0);
}

#include <NJR/Interfaces/TimeHistory.h>

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

	char         mode[256];
	char*        idofilename;
	char         type[256];
	unsigned int RecordStep;
	unsigned int UpIact;

	sscanf(argv[1], "%s", mode);
	sscanf(argv[3], "%s", type);
	sscanf(argv[4], "%d", &RecordStep);
	if (argc >= 6)
		sscanf(argv[5], "%d", &UpIact);

	VEDO::DOWorld*       pDOWorld;
	VEDO::Consultant*    pConsultant;
	VEDO::IactRecordTab* pIactRecordTab = new VEDO::IactRecordTab();
	VEDO::Assembler*     pAssembler = new VEDO::Assembler;

	pAssembler->AddDO(new VEDO::DOcf<VEDO::DOConstrainedQuasiCylinder>(VEDO::QuasiCylinder, "constrained"));
	pAssembler->AddDO(new VEDO::DOcf<VEDO::DOConstrainedQuasiPlate>(VEDO::QuasiPlate, "constrained"));
	pAssembler->AddDO(new VEDO::DOcf<VEDO::DOFixedQuasiCylinder>(VEDO::QuasiCylinder, "fixed"));
	pAssembler->AddDO(new VEDO::DOcf<VEDO::DOFixedQuasiPlate>(VEDO::QuasiPlate, "fixed"));
	pAssembler->AddDO(new VEDO::DOcf<VEDO::DOFixedSphere>(VEDO::Sphere, "fixed"));
	pAssembler->AddDO(new VEDO::DOcf<VEDO::DOQuasiCylinder>(VEDO::QuasiCylinder, "mobile"));
	pAssembler->AddDO(new VEDO::DOcf<VEDO::DOQuasiPlate>(VEDO::QuasiPlate, "mobile"));
	pAssembler->AddDO(new VEDO::DOcf<VEDO::DOSphere>(VEDO::Sphere, "mobile"));

	pAssembler->AddIS(new VEDO::IScf<VEDO::ISwBSDBF>("ISwBSDBF"));
	pAssembler->AddIS(new VEDO::IScf<VEDO::ISwBtSDBF>("ISwBtSDBF"));

    pAssembler->AddCD(new VEDO::CDcf<VEDO::CDSphere_Sphere>          (VEDO::Sphere, VEDO::Sphere       , "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
    pAssembler->AddCD(new VEDO::CDcf<VEDO::CDSphere_QuasiCylinder>   (VEDO::Sphere, VEDO::QuasiCylinder, "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
	pAssembler->AddCD(new VEDO::CDcf<VEDO::CDSphere_QuasiPlate>      (VEDO::Sphere, VEDO::QuasiPlate   , "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
    //pAssembler->AddCD(new VEDO::CDcf<VEDO::CDSphere_SphereAT>        (VEDO::Sphere, VEDO::Sphere       , "List- <CT>ISwLSDAT"));
    //pAssembler->AddCD(new VEDO::CDcf<VEDO::CDSphere_QuasiCylinderAT> (VEDO::Sphere, VEDO::QuasiCylinder, "List- <CT>ISwLSDAT"));

	if ( !strcmp (strlen(argv[2]) - 4 + argv[2], ".ido") )
	{
		pDOWorld    = new VEDO::DOWorld;
		idofilename = argv[2];
		pDOWorld->ReadIDO(idofilename, pIactRecordTab);
	}
	else
	{
		usage(2);
	}

	if (!strcmp(type, "safe"))
	{
		pConsultant = new VEDO::SafeConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep);
	}
    else if (!strcmp(type, "near"))
    {
		pConsultant = new VEDO::NearConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep);
	}
	else if (!strcmp(type, "nbs"))
	{
		if (argc < 6)
		{
			usage(3);
		}
	  	pConsultant = new VEDO::NBSConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
	}
	else if (!strcmp(type, "leap"))
	{
	    if (argc < 6)
	    {
			usage(3);
		}
		pConsultant = new VEDO::LeapConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
	}
	else
	{
		usage(5);
	}

	//NJR::RunTime("Simulation Start !!");

	VEDO::SimMediator sm(pConsultant, pAssembler);
	sm.Initiate();

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

	//NJR::RunTime("Simulation End !!");

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
