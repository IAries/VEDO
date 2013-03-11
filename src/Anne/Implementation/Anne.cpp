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
#include <Common/Interfaces/ISwH.h>
#include <Common/Interfaces/ISwHWS.h>
#include <Common/Interfaces/ISwTest1.h>
#include <Common/Interfaces/ISwTest2.h>

#include <Common/Interfaces/LeapConsultant.h>
#include <Common/Interfaces/LeapConsultant2.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <Common/Interfaces/NearConsultant.h>
#include <Common/Interfaces/SafeConsultant.h>

#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

void usage (int g)
{
	cout
    	<< "ANNE 2011 Build 222" << endl
		<< endl
		<< "Usage:" << endl
		<< "Anne <Mode> <IDO file> <Type I > <Record>" << endl
		<< "Anne <Mode> <IDO file> <Type II> <Record> <UpIact>" << endl << endl
		<< "Mode   : analysis/show/redistribute" << endl
		<< "Type I : safe/near" << endl
		<< "Type II: nbs/leap" << endl
		<< "Record : integer" << endl
		<< "UpIact : integer" << endl << endl
		<< "Error Condition: "
		<< g
		<< endl;
	exit(0);
}

int main (int argc, char* argv[])
{
	if (argc < 5)
    {
		usage(1);
	}

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
    {
		sscanf(argv[5], "%d", &UpIact);
	}

	DOWorld*       pDOWorld;
	Consultant*    pConsultant;
	IactRecordTab* pIactRecordTab;
	Assembler*     pAssembler = new Assembler;

	pAssembler->AddDO(new DOcf<DOConstrainedQuasiCylinder>(QuasiCylinder, "constrained"));
	pAssembler->AddDO(new DOcf<DOConstrainedQuasiPlate>(QuasiPlate, "constrained"));
	pAssembler->AddDO(new DOcf<DOFixedQuasiCylinder>(QuasiCylinder, "fixed"));
	pAssembler->AddDO(new DOcf<DOFixedQuasiPlate>(QuasiPlate, "fixed"));
	pAssembler->AddDO(new DOcf<DOFixedSphere>(Sphere, "fixed"));
	pAssembler->AddDO(new DOcf<DOQuasiCylinder>(QuasiCylinder, "mobile"));
	pAssembler->AddDO(new DOcf<DOQuasiPlate>(QuasiPlate, "mobile"));
	pAssembler->AddDO(new DOcf<DOSphere>(Sphere, "mobile"));

	pAssembler->AddIS(new IScf<ISwBSDBF>("ISwBSDBF"));
	pAssembler->AddIS(new IScf<ISwBtSDBF>("ISwBtSDBF"));
	pAssembler->AddIS(new IScf<ISwH>("ISwH"));
	pAssembler->AddIS(new IScf<ISwHWS>("ISwHWS"));
	pAssembler->AddIS(new IScf<ISwTest1>("ISwTest1"));
	pAssembler->AddIS(new IScf<ISwTest2>("ISwTest2"));

    pAssembler->AddCD(new CDcf<CDSphere_Sphere>          (Sphere, Sphere       , "List- <CT>ISwBSDBF <CT>ISwBtSDBF <CT>ISwH <CT>ISwHWS <CT>ISwTest1 <CT>ISwTest2"));
    pAssembler->AddCD(new CDcf<CDSphere_QuasiCylinder>   (Sphere, QuasiCylinder, "List- <CT>ISwBSDBF <CT>ISwBtSDBF <CT>ISwH <CT>ISwHWS <CT>ISwTest1 <CT>ISwTest2"));
	pAssembler->AddCD(new CDcf<CDSphere_QuasiPlate>      (Sphere, QuasiPlate   , "List- <CT>ISwBSDBF <CT>ISwBtSDBF <CT>ISwH <CT>ISwHWS <CT>ISwTest1 <CT>ISwTest2"));
    //pAssembler->AddCD(new CDcf<CDSphere_SphereAT>        (Sphere, Sphere       , "List- <CT>ISwLSDAT"));
    //pAssembler->AddCD(new CDcf<CDSphere_QuasiCylinderAT> (Sphere, QuasiCylinder, "List- <CT>ISwLSDAT"));

	if ( !strcmp (strlen(argv[2]) - 4 + argv[2], ".ido") )
	{
		pDOWorld    = new DOWorld;
		idofilename = argv[2];
		pDOWorld->ReadIDO(idofilename);
		string irtfile(idofilename);
		irtfile = irtfile.substr(0, irtfile.size() - 4) += ".irt";
		pIactRecordTab = new IactRecordTab(irtfile.c_str());
	}
	else
	{
		usage(2);
	}

	if (!strcmp(type, "safe"))
	{
		pConsultant = new SafeConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep);
	}
    else if (!strcmp(type, "near"))
    {
		pConsultant = new NearConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep);
	}
	else if (!strcmp(type, "nbs"))
	{
		if (argc < 6)
		{
			usage(3);
		}
	  	pConsultant = new NBSConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
	}
	else if (!strcmp(type, "leap"))
	{
	    if (argc < 6)
	    {
			usage(3);
		}
//		pConsultant = new LeapConsultant(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
		pConsultant = new LeapConsultant2(pDOWorld, pIactRecordTab, idofilename, RecordStep, UpIact);
	}
	else
	{
		usage(5);
	}

	//NJR::RunTime("Simulation Start !!");

	SimMediator sm(pConsultant, pAssembler);
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
		string vtufile(idofilename);
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

	ofstream FileLog("time.txt", ios::out);
	FileLog
		<< "=============================================================" << endl
		<< " Simulated Time: "
		<< pConsultant->GetDOWorld()->GetSystemParameter()->GetTimeCurrent()
		<< " seconds" << endl
		<< "-------------------------------------------------------------" << endl
		<< " Item                                       Time(s) Rate(%)  " << endl
		<< "-------------------------------------------------------------" << endl
		<< " System                                    "                   << '\t'
		<< timeSystem                                                      << "\t\t"
		<< timeSystem                    /timeTotal*100.0                  << endl
		<< " Impact Solving                            "                   << '\t'
	   	<< timeImpactSolving                                               << "\t\t"
		<< timeImpactSolving             /timeTotal*100.0                  << endl
		<< " Field Force Adding                        "                   << '\t'
		<< timeFieldForceAdding                                            << "\t\t"
		<< timeFieldForceAdding          /timeTotal*100.0                  << endl
		<< " Response Updating                         "                   << '\t'
		<< timeResponseUpdating                                            << "\t\t"
		<< timeResponseUpdating          /timeTotal*100.0                  << endl
		<< " Contact Detection                         "                   << '\t'
		<< timeContactDetection                                            << "\t\t"
		<< timeContactDetection          /timeTotal*100.0                  << endl
		<< " NextStep                                  "                   << '\t'
		<< timeNextStep                                                    << "\t\t"
		<< timeNextStep                  /timeTotal*100.0                  << endl
		<< " DOContainer Synchronization               "                   << '\t'
	   	<< timeSyncDOContainer                                             << "\t\t"
		<< timeSyncDOContainer           /timeTotal*100.0                  << endl
		<< "-------------------------------------------------------------" << endl
		<< " Computing                                 "                   << '\t'
		<< timeComputing                                                   << "\t\t"
		<< timeComputing                 /timeTotal*100.0                  << endl
		<< " Communication                             "                   << '\t'
		<< timeCommunication                                               << "\t\t"
		<< timeCommunication             /timeTotal*100.0                  << endl
		<< "-------------------------------------------------------------" << endl
		<< " Total                                     "                   << '\t'
		<< timeTotal                                                       << "\t\t"
		<< 100                                                             << endl
		<< "=============================================================" << endl;
	FileLog.close();

	delete pDOWorld;
	delete pIactRecordTab;
	delete pConsultant;

	return true;
};
