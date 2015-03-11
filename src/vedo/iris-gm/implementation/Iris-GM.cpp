#include <mpi.h>
#include <vedo/njr/interfaces/Utility.h>
#include <vedo/framework/interfaces/Assembler.h>
#include <vedo/framework/interfaces/Consultant.h>
#include <vedo/framework/interfaces/DOModel.h>
#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/GeometricShape.h>
#include <vedo/framework/interfaces/SimMediator.h>
#include <vedo/iris-gm/interfaces/NBSParallelConsultant.h>
#include <vedo/constants/interfaces/Constants.h>
#include <vedo/ModuleList.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

vedo::Constants* vedo_cp = vedo::Constants::Instance();

void usage (vedo::vedo_int_t g)
{
	std::cout
		<< "Iris-GM " << vedo::sVersion << " " << vedo_cp->SysInfo() << std::endl
		<< std::endl
		<< "Usage: iris-gm <Mode> <IDO file> <Record> <UpIact>"      << std::endl
		<< "Mode  : analysis/redistribute"                           << std::endl
		<< "Record: integer"                                         << std::endl
		<< "UpIact: integer"                                         << std::endl;
	exit(0);
}

int main(int argc, char* argv[])
{
	if (argc < 5)
	{
		usage(1);
	}

	std::vector<std::string> arg;
	for (int i=0; i<argc; i++)
	{
		arg.push_back(argv[i]);
	}

	MPI_Init(&argc, &argv);

	time_t starttime;   // Starting time
	time_t endtime;     // Endind time
	time(&starttime);

	vedo::vedo_float_t timeSystem           = 0.0;   // Time of system preparing, starting and ending
	vedo::vedo_float_t timeImpactSolving    = 0.0;   // Time of impact solving
	vedo::vedo_float_t timeSyncDOContainer  = 0.0;   // Time of "SyncDOContainer"
	vedo::vedo_float_t timeFieldForceAdding = 0.0;   // Time of field force adding
	vedo::vedo_float_t timeResponseUpdating = 0.0;   // Time of response updating
	vedo::vedo_float_t timeContactDetection = 0.0;   // Time of contact detection
	vedo::vedo_float_t timeNextStep         = 0.0;   // Time of "NextStep"
	vedo::vedo_float_t timePartitioning     = 0.0;   // Time of problem partitioning

	std::string mode = argv[1];
	vedo::Constants* vedo_cp = vedo::Constants::Instance();
	vedo::vedo_uint_t RecordStep = vedo_cp->String2T<vedo::vedo_uint_t>(argv[3]);
	vedo::vedo_uint_t UpIact     = vedo_cp->String2T<vedo::vedo_uint_t>(argv[4]);

	vedo::DOWorld*       pDOWorld;
	vedo::Consultant*    pConsultant;
	vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
	vedo::Assembler*     pAssembler = vedo::CreateNewAssembler();

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime = endtime;

	std::string idofilename;
	if (njr::CheckSubName(arg[2], ".ido" ))
	{
		pDOWorld    = new vedo::DOWorld;
		idofilename = argv[2];
		pDOWorld->ReadIDO(idofilename, pIactRecordTab);
	}
	else
    {
		usage(2);
    }

	pConsultant = new vedo::NBSParallelConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);

	int rank;   // Rank   of processores
	int NP;     // Number of processores
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &NP);

	vedo::SimMediator sm(pConsultant, pAssembler, static_cast<vedo::vedo_uint_t>(rank), static_cast<vedo::vedo_uint_t>(NP));

	time(&endtime);
	timeSystem += (endtime - starttime);

	if (mode == "redistribute")
	{
		while (sm.ReDistribute());
	}
/*
	else if (mode == "show")
	{
		sm.ShowInteraction();
		std::string vtufile(idofilename);
		vtufile = vtufile.substr(0, vtufile.size() - 4) += ".vtu";
		sm.WriteInteractionForce(vtufile.c_str());
	}
*/
	else
	{
		while (sm.Run());
	}

	time(&starttime);

	//MPI_Comm comm;
	//MPI_Comm_dup(MPI_COMM_WORLD, &comm);
	//MPI_Comm_free(&comm);
	//njr::RunTime("Simulation End !!");

	timeSystem           += (sm.timeSystem);
	timeImpactSolving     = (sm.timeImpactSolving);
	timeSyncDOContainer   = (sm.timeSyncDOContainer);
	timeFieldForceAdding  = (sm.timeFieldForceAdding);
	timeResponseUpdating  = (sm.timeResponseUpdating);
	timeContactDetection  = (sm.timeContactDetection);
	timeNextStep          = (sm.timeNextStep);
	timePartitioning      = (sm.timePartitioning);

	time(&endtime);
	timeSystem           += (endtime - starttime);

	vedo::vedo_float_t timeComputing
		= timeSystem + timeImpactSolving + timeFieldForceAdding + timeResponseUpdating + timeContactDetection;
	vedo::vedo_float_t timeCommunication = timeSyncDOContainer + timeNextStep + timePartitioning;
	vedo::vedo_float_t timeTotal = timeComputing + timeCommunication;

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

	exit(0);
  	return true;
}
