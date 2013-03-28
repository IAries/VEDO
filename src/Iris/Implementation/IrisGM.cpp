#include <mpi.h>
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

//#include <Iris/Interfaces/DConsultant.h>
//#include <Iris/Interfaces/NBSGPConsultant.h>
#include <Iris/Interfaces/NBSParallelConsultant.h>
//#include <Iris/Interfaces/ParallelConsultant.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

void usage (int g)
{
    std::cout
    	<< "IrisGM X3 build 328" << std::endl
		<< std::endl
		<< "\tUsage:" << std::endl
		<< "\tIris <Mode> <IDO file> <Record> <UpIact>" << std::endl
		<< std::endl
		<< "\tMode  : geometric/show/redistribute" << std::endl
		<< "\tRecord: integer" << std::endl
		<< "\tUpIact: integer" << std::endl
		<< std::endl
		<< "\tError Condition: " << g << std::endl;
	exit(0);
}

int main (int argc, char* argv[])
{
	if (argc < 5)
		usage(1);

	MPI_Init(&argc, &argv);

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
	double timePartitioning     = 0.0;   // Time of problem partitioning

	char         mode[256];
	char*        idofilename;
	unsigned int RecordStep;
	unsigned int UpIact;

	//Iris <Mode> <IDO file> <Record> <UpIact>"
	sscanf(argv[1], "%s", mode);
	sscanf(argv[3], "%d", &RecordStep);
	sscanf(argv[4], "%d", &UpIact);

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

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime = endtime;

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

	pConsultant = new VEDO::NBSParallelConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
	//NJR::RunTime("Simulation Start !!");

	int rank;   // Rank   of processores
	int NP;     // Number of processores
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &NP);

	VEDO::SimMediator sm(pConsultant, pAssembler, static_cast<unsigned int>(rank), static_cast<unsigned int>(NP));
	sm.Initiate();

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (!strcmp(mode, "redistribute"))
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
		while (sm.Run());
	}

	time(&starttime);

	//MPI_Comm comm;
	//MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	//MPI_Comm_free(&comm);
	//NJR::RunTime("Simulation End !!");

	timeSystem           += (sm.timeSystem);
	timeImpactSolving     = (sm.timeImpactSolving);
	timeSyncDOContainer   = (sm.timeSyncDOContainer);
	timeFieldForceAdding  = (sm.timeFieldForceAdding);
	timeResponseUpdating  = (sm.timeResponseUpdating);
	timeContactDetection  = (sm.timeContactDetection);
	timeNextStep          = (sm.timeNextStep);
	timePartitioning      = (sm.timePartitioning);

	time(&endtime);
	timeSystem += (endtime - starttime);

	double timeComputing
		= timeSystem
		+ timeImpactSolving
		+ timeFieldForceAdding
		+ timeResponseUpdating
		+ timeContactDetection;

	double timeCommunication
		= timeSyncDOContainer
		+ timeNextStep
		+ timePartitioning;

	double timeTotal = timeComputing + timeCommunication;

	if(rank == 0)
	{
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
	}

	delete pDOWorld;
	delete pIactRecordTab;
	delete pConsultant;

	MPI_Finalize();

  	return true;
};
