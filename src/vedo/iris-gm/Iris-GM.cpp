#include <mpi.h>
#include <vedo/framework/Assembler.h>
#include <vedo/framework/Consultant.h>
#include <vedo/framework/DOModel.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/GeometricShape.h>
#include <vedo/framework/SimMediator.h>
#include <vedo/iris-gm/NBSParallelConsultant.h>
#include <vedo/ModuleList.h>
#include <aries/utility/BinaryIO.h>
#include <aries/utility/Constants.h>
#include <aries/utility/DataType.h>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

void usage (vedo::_uint_t g)
{
	//aries::Constants* aries_cp = aries::Constants::Instance();
	std::cout
		//<< "Iris-GM " << aries_cp->Version() << " " << aries_cp->Information() << std::endl
		<< "Iris-GM "                                           << std::endl
		<< std::endl
		<< "Usage: iris-gm <Mode> <IDO file> <Record> <UpIact>" << std::endl
		<< "Mode  : analysis/redistribute"                      << std::endl
		<< "Record: integer"                                    << std::endl
		<< "UpIact: integer"                                    << std::endl;
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

	vedo::_float_t timeSystem           = 0.0;   // Time of system preparing, starting and ending
	vedo::_float_t timeImpactSolving    = 0.0;   // Time of impact solving
	vedo::_float_t timeSyncDOContainer  = 0.0;   // Time of "SyncDOContainer"
	vedo::_float_t timeFieldForceAdding = 0.0;   // Time of field force adding
	vedo::_float_t timeResponseUpdating = 0.0;   // Time of response updating
	vedo::_float_t timeContactDetection = 0.0;   // Time of contact detection
	vedo::_float_t timeNextStep         = 0.0;   // Time of "NextStep"
	vedo::_float_t timePartitioning     = 0.0;   // Time of problem partitioning

	std::string mode = argv[1];
	vedo::_uint_t RecordStep = aries::String2T<vedo::_uint_t>(argv[3]);
	vedo::_uint_t UpIact     = aries::String2T<vedo::_uint_t>(argv[4]);

	vedo::DOWorld*       pDOWorld;
	vedo::Consultant*    pConsultant;
	vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
	vedo::Assembler*     pAssembler = vedo::CreateNewAssembler();

	time(&endtime);
	timeSystem += (endtime - starttime);
	starttime = endtime;

	std::string idofilename;
	if (aries::CheckSubName(arg[2], ".ido" ))
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

	vedo::SimMediator sm(pConsultant, pAssembler, static_cast<vedo::_uint_t>(rank), static_cast<vedo::_uint_t>(NP));

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

	vedo::_float_t timeComputing
		= timeSystem + timeImpactSolving + timeFieldForceAdding + timeResponseUpdating + timeContactDetection;
	vedo::_float_t timeCommunication = timeSyncDOContainer + timeNextStep + timePartitioning;
	vedo::_float_t timeTotal = timeComputing + timeCommunication;

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
