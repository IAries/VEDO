#include <aries/utility/Constants.h>
#include <aries/utility/BinaryIO.h>
#include <vedo/framework/Assembler.h>
#include <vedo/framework/Consultant.h>
#include <vedo/framework/DataType.h>
#include <vedo/framework/DOModel.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/GeometricShape.h>
#include <vedo/framework/IactRecordTab.h>
#include <vedo/framework/SimMediator.h>
#include <vedo/common/LeapConsultant.h>
#include <vedo/common/NBSConsultant.h>
#include <vedo/common/NearConsultant.h>
#include <vedo/common/SafeConsultant.h>
#include <vedo/ModuleList.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

aries::Constants* aries_cp = aries::Constants::Instance();
vedo::_float_t SafetyFactor = aries_cp->SafetyFactor();

void usage (vedo::_uint_t g)
{
	std::cout
		<< "Anne " << aries_cp->Version() << " " << aries_cp->Information() << std::endl
		<< std::endl
		<< "Usage: anne <Mode> <IDO file> <Type> <Record> <UpIact>" << std::endl
		<< "Mode                : analysis/redistribute"            << std::endl
		<< "Type                : (I)  safe/near"                   << std::endl
		<< "                      (II) nbs/leap"                    << std::endl
		<< "Record              : integer"                          << std::endl
		<< "UpIact (for Type II): integer"                          << std::endl;
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

	std::string mode = arg[1];
	std::string type = arg[3];
	std::string s = arg[4];
	vedo::_uint_t RecordStep = aries::String2T<vedo::_uint_t>(arg[4]);
	vedo::_uint_t UpIact = 1;
	if (arg.size() >= 6)
	{
		UpIact = aries::String2T<vedo::_uint_t>(arg[5]);
	}

	vedo::DOWorld*       pDOWorld;
	vedo::Consultant*    pConsultant;
	vedo::IactRecordTab* pIactRecordTab = new vedo::IactRecordTab();
	vedo::Assembler*     pAssembler = vedo::CreateNewAssembler();

	std::string idofilename;
	if (aries::CheckSubName(arg[2], ".ido"))
	{
		pDOWorld    = new vedo::DOWorld;
		idofilename = arg[2];
		pDOWorld->ReadIDO(idofilename, pIactRecordTab);
	}
	else
	{
		usage(2);
	}

	if (type == "safe")
	{
		pConsultant = new vedo::SafeConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep);
	}
    else if (type == "near")
    {
		pConsultant = new vedo::NearConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep);
	}
	else if (type == "nbs")
	{
		if (argc < 6)
		{
			usage(3);
		}
	  	pConsultant = new vedo::NBSConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
	}
	else if (type == "leap")
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

	if (mode == "analysis")
	{
		while (sm.Run());
	}
	else if (mode == "redistribute")
	{
		while (sm.ReDistribute());
	}
/*
	else if (!strcmp(mode, "show"))
	{
		sm.ShowInteraction();
		std::string vtufile(idofilename);
		vtufile = vtufile.substr(0, vtufile.size() - 4) += ".vtu";
		sm.WriteInteractionForce(vtufile.c_str());
	}
*/
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
	timeSystem           += (endtime - starttime);

	vedo::_float_t timeComputing
		= timeSystem + timeImpactSolving + timeFieldForceAdding + timeResponseUpdating + timeContactDetection + timeNextStep;

	vedo::_float_t timeCommunication = timeSyncDOContainer;

	vedo::_float_t timeTotal = timeComputing + timeCommunication;

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

	delete pIactRecordTab;
	delete pDOWorld;
	delete pConsultant;

	exit(0);
	return true;
}
