#include <mpi.h>
#include <Iris/Interfaces/MPIAssistant.h>
#include <Iris/Interfaces/NBSGPConsultant.h>
#include <NJRLib/Interfaces/Utility.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <iterator>

#include <parmetis.h>
/******************************************************************************
 * The two undef prevents confliction min and max in std namespace conflicts
 * with the macros in <mpi.h>
 ******************************************************************************/
#undef min
#undef max

using namespace std;

// The EnsureLength function
static void EnsureLength
	(unsigned int base,
	unsigned long target,
	unsigned long& length,
	double*& array)
{
	if ((base*target) > length)
	{
		while ((base*target) > length)
        {
        	length *= 2;
        }
		delete[] array;
		array = new double[length];
		// Monitor the status of "Impact Buffer"
		cerr
			<< "Resizing Impact Buffer = "
			<< length
			<< " (NBSGPConsulant::EnsureLength)\n";
	}

/*
	if ((base*target) != 0)
	{
		if ((base*target) > length)
		{
			length = base*target;
			// Monitor the status of "Impact Buffer"
			cerr
				<< "Resizing Impact Buffer = "
				<< length
				<< " (NBSGPConsulant::EnsureLength)\n";
			delete[] array;
			array = new double[length];
		}
		else if ((length%base) != 0)
		{
			length += (base - (length%base));
			// Monitor the status of "Impact Buffer"
			cerr
				<< "Resizing Impact Buffer = "
				<< length
				<< " (NBSGPConsulant::EnsureLength)\n";
			delete[] array;
			array = new double[length];
		}
	}
*/
}

unsigned long NBSGPConsultant::GetDONum() const
{
	return (unsigned long)DOTab.size();
};

unsigned long NBSGPConsultant::GetIactNum() const
{
	return (unsigned long)IactPairTab.size();
};

unsigned long NBSGPConsultant::GetDO(unsigned long i) const
{
	return DOTab[i];
};

unsigned long NBSGPConsultant::GetIactMaster(unsigned long i) const
{
	return G2LTab[IactPairTab[i].first];
};

unsigned long NBSGPConsultant::GetIactSlave(unsigned long i) const
{
	return G2LTab[IactPairTab[i].second];
};

void NBSGPConsultant::WriteDOSubWorld(DOContainer &vDO) const
{
	list<DOModel*> listModel;
	transform
		(pDOWorld->GetDOModel().begin(),
		pDOWorld->GetDOModel().end(),
		back_inserter(listModel),
		NJR::Copy_obj()                 );

	list<IactModel*> listIModel;
	transform
		(pDOWorld->GetIactModel().begin(),
		pDOWorld->GetIactModel().end(),
		back_inserter(listIModel),
		NJR::Copy_obj()                   );

	vector<DOStatus*> vecStatus;

	for (unsigned int i=0; i<vDO.size(); ++i)
	{
		vecStatus.push_back(new DOStatus( *(vDO[i]->GetDOStatus()) ));
	}

	const SystemParameter* oSP = pDOWorld->GetSystemParameter();

	SystemParameter* ps
		= new SystemParameter
			(oSP->GetTitle(),
			oSP->GetNote(),
			oSP->GetTimeStart(),
			oSP->GetTimeStop(),
			oSP->GetTimeInterval(),
			oSP->GetTimeCurrent(),
			vDO.size(),
			oSP->GetFieldForce(),
			oSP->GetZoneOfInterest(),
			oSP->GetPeriodicBoundaryConditions());

	DOWorld w(ps,listModel, listIModel, vecStatus);

	char ltoa[256];
	string file = sfilename.c_str();
	file.append("_");

	sprintf(ltoa, "SUB_%d.xml\0", rank);
	file.append(ltoa);

	w.WriteXML(file.c_str());
}

NBSGPConsultant::NBSGPConsultant
	(DOWorld* DOWorld,
	IactRecordTab* pIactRecordTab,
	char filename[],
	unsigned long ulwrite,
	unsigned long ulupdate)
: Consultant(DOWorld, pIactRecordTab, filename, ulwrite),
	culUpIact(ulupdate),
	G2LTab
		(pDOWorld->GetSystemParameter()->GetDONumber(),
		pDOWorld->GetSystemParameter()->GetDONumber()  )
{
	int u;
	MPI_Comm_size(MPI_COMM_WORLD, &u);
	NP = u;
	MPI_Comm_rank(MPI_COMM_WORLD, &u);
	rank = u;

	MAX_CUT         = 6;
	MAX_ALLOWED_UNB = 0.025;

	overlapTab.assign(NP, vector<unsigned long>());

/******************************************************************************
 * Aries' Comment (2006/04/05)
 *
 *    Maybe we can find some rule between "the number of elements" and
 * "initialImpactCnt", then we can determine the initial buffer for some special
 * case.
 ******************************************************************************/
	//int initialImpactCnt = ImpactBufferSize;

	for (unsigned int i=0; i<NP; ++i)
    {
		sendBufVec.push_back
			(pair<unsigned long, double*>
				(ImpactBufferSize, new double [ImpactBufferSize]) );

		recvBufVec.push_back
			(pair<unsigned long, double*>
				(ImpactBufferSize, new double [ImpactBufferSize]) );
	}

	int initialSyncCnt = 1.2 * (pDOWorld->GetSystemParameter()->GetDONumber());

/******************************************************************************
 * Aries' Comment (2006/02/25)
 *
 *    For meeeage passing with MPI, we have to expend the size of buffer
 * parallel environmnent, this is useless.
 ******************************************************************************/
//	if (rank != MASTER)
//    {
		syncSendBufVec.push_back
			(make_pair
				(initialSyncCnt,
					make_pair
						(new unsigned long[initialSyncCnt],
						new double[initialSyncCnt * 15])   ));
//	}
//	else
//	{
		for (unsigned int i=0; i<NP; ++i)
        {
			syncRecvBufVec.push_back
				(make_pair
					(initialSyncCnt,
						make_pair
							(new unsigned long[initialSyncCnt],
							new double[initialSyncCnt * 15])   ));
        }
//	}

	//timeReset              = 0.0;
	//timeGP                 = 0;
	TIMES_MAX_CUT_EXCEEDED = 0;
	Reset();
}


NBSGPConsultant::~NBSGPConsultant()
{
	for (unsigned int i=0; i<NP; ++i)
    {
		delete sendBufVec[i].second;
		delete recvBufVec[i].second;
	}
	for (unsigned int i=0;i<syncSendBufVec.size();++i)
    {
		delete syncSendBufVec[i].second.first;
		delete syncSendBufVec[i].second.second;
	}
	for (unsigned int i=0;i<syncRecvBufVec.size();++i)
    {
		delete syncRecvBufVec[i].second.first;
		delete syncRecvBufVec[i].second.second;
	}
	// for run time statistics
	//cerr << '[' << rank << "] timeReset             : " << timeReset/3600.0       << endl;
	//cerr << '[' << rank << "] timeGP                : " << timeGP   /  60.0       << endl;
	cerr << '[' << rank << "] TIMES_MAX_CUT_EXCEEDED: " << TIMES_MAX_CUT_EXCEEDED << endl;
}

void NBSGPConsultant::ConstructDOandOverLapTab()
{
	DOTab.clear();
	unsigned long DONum = pDOWorld->GetSystemParameter()->GetDONumber();
	G2LTab.reserve(DONum);
	G2LTab.assign(DONum, DONum);
	for (unsigned i=0; i<NP; ++i)
    {
		overlapTab[i].clear();
    }

	// Contrust the DO Table and G2LTab
	for (unsigned long i=0; i<IactPairTab.size(); ++i)
    {
		G2LTab[IactPairTab[i].first ] = 0;
		G2LTab[IactPairTab[i].second] = 0;
	}

	for(unsigned long i=0; i<DONum; i++)
    {
		if(G2LTab[i] == DONum)
        {
			continue;
        }
		DOTab.push_back(i);
		G2LTab[i] = DOTab.size() - 1;
	}

	char* occur    = new char[DONum   ];
	char* evpoccur = new char[DONum*NP];

	// Contrust the overlapTable
	for (unsigned long i=0; i<DONum; ++i)
    {
		occur[i]
			= (G2LTab[i]==DONum) ? static_cast<char>(0) : static_cast<char>(1);
    }

    MPI_Allgather
		(occur, DONum, MPI_CHAR, evpoccur, DONum, MPI_CHAR, MPI_COMM_WORLD);

	for (unsigned int i=0; i<NP; ++i)
    {
		if (i == rank)
        {
			continue;
        }
		for (unsigned long j=0; j<DONum; ++j)
        {
			if ( ( occur[j]==static_cast<char>(1) )
				&& ( evpoccur[i*DONum+j]==static_cast<char>(1) ) )
            {
				overlapTab[i].push_back(j);
            }
		}
	}

	// Pick the orphan
	if (rank == MASTER)
    {
		for(unsigned long i=0; i<DONum; i++)
        {
			int sum = 0;
			for (unsigned j=0; j<NP; ++j)
            {
				sum += static_cast<int>(evpoccur[j*DONum+i]);
            }
			if (sum == 0)
            {
				DOTab.push_back(i);
				G2LTab[i] = DOTab.size()-1;
			}
		}
	}
	delete[] occur;
	delete[] evpoccur;
}

void NBSGPConsultant::SyncDOContainer(DOContainer & vDO)
{
	for(unsigned int i=0; i<overlapTab.size(); i++)
    {
		EnsureLength
			(6,
			overlapTab[i].size(),
			sendBufVec[i].first,
			sendBufVec[i].second);

		EnsureLength
			(6,
			overlapTab[i].size(),
			recvBufVec[i].first,
			recvBufVec[i].second);

		ImpactBufferSize
			= max
				(ImpactBufferSize, max
					(sendBufVec[i].first, recvBufVec[i].first) );
	}

	// Send impact
	MPI_Request*  reqArray     = new MPI_Request[NP-1];
	MPI_Status*   statusArray  = new MPI_Status [NP-1];
    int           idxReq       = 0;
	unsigned long idx          = 0;
	double*       impactSedBuf = 0;
	for(unsigned int i=0; i<overlapTab.size(); i++)
    {
		if(i == rank)
        {
			continue;
		}
		impactSedBuf = sendBufVec[i].second;
		for(unsigned long j=0; j<overlapTab[i].size(); j++)
		{
			idx = G2LTab[ overlapTab[i][j] ];
			impactSedBuf[6*j]   = vDO[idx]->GetImpact().x();
			impactSedBuf[6*j+1] = vDO[idx]->GetImpact().y();
			impactSedBuf[6*j+2] = vDO[idx]->GetImpact().z();
			impactSedBuf[6*j+3] = vDO[idx]->GetAngularImpact().x();
			impactSedBuf[6*j+4] = vDO[idx]->GetAngularImpact().y();
			impactSedBuf[6*j+5] = vDO[idx]->GetAngularImpact().z();
		}
		//assistant.transferImpactSend(impactSedBuf, 6*overlapTab[i].size(), i);
		MPI_Isend(  impactSedBuf  , 6*overlapTab[i].size(), MPI_DOUBLE, i, 1,
					MPI_COMM_WORLD, &reqArray[idxReq]                        );
		idxReq++;
	}

/******************************************************************************
 * Aries' Comment (2006/04/05)
 *
 *    If "Wait All" at here, it may causes deadlock in some parallel
 * environment, such as IBM POE.
 ******************************************************************************/
	//MPI_Waitall(NP-1, reqArray, statusArray);

	// Receive impact
	MPI_Status*  statusArray2 = new MPI_Status [NP-1];
	             idxReq       = 0;
	double*      impactRecBuf = 0;
	for(unsigned int i=0; i<overlapTab.size(); i++)
    {
		if(i == rank)
        {
			continue;
        }
		impactRecBuf = recvBufVec[i].second;
		//assistant.transferImpactRecv(impactRecBuf,  6*overlapTab[i].size(), i);
		MPI_Recv(   impactRecBuf  , 6*overlapTab[i].size(), MPI_DOUBLE, i, 1,
					MPI_COMM_WORLD, &statusArray2[idxReq]                    );
		idxReq++;
	}
/******************************************************************************
 * Aries' Comment (2006/04/05)
 *
 *    If we don't "Wait All" for "MPI_ISend", it may causes memory leaks.
 ******************************************************************************/
	MPI_Waitall(NP-1, reqArray,  statusArray);
	delete[] reqArray;
	delete[] statusArray;
	delete[] statusArray2;

	// Add impact
	for (unsigned int i=0; i<overlapTab.size(); ++i)
    {
        if(i == rank)
        {
			continue;
        }
		impactRecBuf = recvBufVec[i].second;
		for(unsigned long j=0; j<overlapTab[i].size(); j++)
        {
			idx = G2LTab[ overlapTab[i][j] ];
			vDO[idx]
				->AddImpact
					(NJRvector3d
						(impactRecBuf[6*j],
						impactRecBuf[6*j+1],
						impactRecBuf[6*j+2]),
					NJRvector3d
						(impactRecBuf[6*j+3],
						impactRecBuf[6*j+4],
						impactRecBuf[6*j+5]) );
		}
	}
}

void NBSGPConsultant::SyncWorld(DOContainer & vDO)
{
	//To ensure the size is enough
	/*
	if (this->rank == MASTER)
	{
		for (int i=0; i<NP; ++i)
		{
			EnsureLength
				(15,
				pDOWorld->GetSystemParameter()->GetDONumber(),
				this->syncRecvBufVec[i].first,
				syncRecvBufVec[i].second);
		}
	}
	else
	{
		EnsureLength
			(15,
			DOTab.size(),
			syncSendBufVec[0].first,
			syncSendBufVec[0].second);
	}
	*/

/******************************************************************************
 * Aries' Comment (2006/04/19)
 *
 *    Wokerla uses file translation to exchange the information between
 * processoes. On some plateform, this may causes information error due to the
 * I/O framework of operating system. At here, we exchange the information by
 * "message passing".
 ******************************************************************************/
	// Ask How much elements each processor has
	unsigned long* recCnt = new unsigned long [NP];
	unsigned long       s = vDO.size();
	MPI_Allgather(  &s    , 1, MPI_UNSIGNED_LONG,
					recCnt, 1, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);

	// Update the information of elements from local processor
	const DOStatus* SourceDOStatus = 0;
	      DOStatus* TargetDOStatus = 0;
	for (unsigned long i=0; i<s; ++i)
    {
		SourceDOStatus =    vDO[i]->GetDOStatus();
		TargetDOStatus = (pDOWorld->GetDOStatus())[ DOTab[i] ];
		TargetDOStatus->SetPosition(SourceDOStatus->GetPosition());
		TargetDOStatus->SetVelocity(SourceDOStatus->GetVelocity());
		TargetDOStatus
			->SetOrientation
				(SourceDOStatus->GetOrientationX(),
				SourceDOStatus->GetOrientationZ());
		TargetDOStatus
			->SetAngularVelocity(SourceDOStatus->GetAngularVelocity());
	}

	// Get the information of elements from the other processors
	// Send the information to the other processors
	MPI_Request* reqArray    = new MPI_Request[2*NP-2];
	MPI_Status*  statusArray = new MPI_Status [2*NP-2];
	unsigned long  sedCnt    = DOTab.size();
	unsigned long* sedID     = syncSendBufVec[0].second.first;
	double*        sedStatus = syncSendBufVec[0].second.second;
    int            idxReq    = 0;

	for (unsigned long i=0; i<NP; i++)
	{
		if(i == rank)
        {
			continue;
		}
/*
		for (unsigned long j=0; j<sedCnt; ++j)
		{
			sedID[j] = DOTab[j];
			const DOStatus* dos = vDO[j]->GetDOStatus();
			sedStatus[15*j]     = dos->GetPosition().x();
			sedStatus[15*j+1]   = dos->GetPosition().y();
			sedStatus[15*j+2]   = dos->GetPosition().z();
			sedStatus[15*j+3]   = dos->GetVelocity().x();
			sedStatus[15*j+4]   = dos->GetVelocity().y();
			sedStatus[15*j+5]   = dos->GetVelocity().z();
			sedStatus[15*j+6]   = dos->GetOrientationX().x();
			sedStatus[15*j+7]   = dos->GetOrientationX().y();
			sedStatus[15*j+8]   = dos->GetOrientationX().z();
			sedStatus[15*j+9]   = dos->GetOrientationZ().x();
			sedStatus[15*j+10]  = dos->GetOrientationZ().y();
			sedStatus[15*j+11]  = dos->GetOrientationZ().z();
			sedStatus[15*j+12]  = dos->GetAngularVelocity().x();
			sedStatus[15*j+13]  = dos->GetAngularVelocity().y();
			sedStatus[15*j+14]  = dos->GetAngularVelocity().z();
		}
*/
		for (unsigned long j=0, j0=0; j<sedCnt; ++j)
		{
			sedID[j] = DOTab[j];
			const DOStatus* dos = vDO[j]->GetDOStatus();
			sedStatus[j0++] = dos->GetPosition().x();
			sedStatus[j0++] = dos->GetPosition().y();
			sedStatus[j0++] = dos->GetPosition().z();
			sedStatus[j0++] = dos->GetVelocity().x();
			sedStatus[j0++] = dos->GetVelocity().y();
			sedStatus[j0++] = dos->GetVelocity().z();
			sedStatus[j0++] = dos->GetOrientationX().x();
			sedStatus[j0++] = dos->GetOrientationX().y();
			sedStatus[j0++] = dos->GetOrientationX().z();
			sedStatus[j0++] = dos->GetOrientationZ().x();
			sedStatus[j0++] = dos->GetOrientationZ().y();
			sedStatus[j0++] = dos->GetOrientationZ().z();
			sedStatus[j0++] = dos->GetAngularVelocity().x();
			sedStatus[j0++] = dos->GetAngularVelocity().y();
			sedStatus[j0++] = dos->GetAngularVelocity().z();
		}
		MPI_Isend(  sedID, sedCnt , MPI_UNSIGNED_LONG, i, 1,
					MPI_COMM_WORLD, &reqArray[idxReq]       );
		idxReq++;
		MPI_Isend(  sedStatus, 15*sedCnt, MPI_DOUBLE , i, 2,
					MPI_COMM_WORLD, &reqArray[idxReq]       );
		idxReq++;
	}

	// Receive the information from the other processors
    unsigned long* recID        = 0;
	double*        recStatus    = 0;
	               idxReq       = 0;
	MPI_Status*    statusArray2 = new MPI_Status [2*NP-2];
	for(unsigned int i=0; i<NP; ++i)
    {
		if (i == rank)
        {
			continue;
        }
		recID     = syncRecvBufVec[i].second.first;
		recStatus = syncRecvBufVec[i].second.second;
		MPI_Recv(   recID, recCnt[i]       , MPI_UNSIGNED_LONG, i, 1,
					MPI_COMM_WORLD         , &statusArray2[idxReq]   );
		idxReq++;
		MPI_Recv(   recStatus, 15*recCnt[i], MPI_DOUBLE       , i, 2,
					MPI_COMM_WORLD         , &statusArray2[idxReq]   );
		idxReq++;
	}

	// Waitall to ensure every "MPI_Isend" has completed.
	MPI_Waitall(2*NP-2, reqArray, statusArray);

	// Update the information of elements from the other processores
	unsigned long ridx = 0;
	for(unsigned int i=0; i<NP; ++i)
    {
		if (i == rank)
        {
			continue;
        }
		recID     = syncRecvBufVec[i].second.first;
		recStatus = syncRecvBufVec[i].second.second;
		for (unsigned long j=0; j<recCnt[i]; ++j)
        {
            ridx = recID[j];
			if ( (G2LTab[ridx] >= 0) && (G2LTab[ridx] < DOTab.size()) )
            {
				continue;
			}
			(pDOWorld->GetDOStatus())[ridx]
				->SetPosition
					(NJRvector3d
						(recStatus[15*j],
						recStatus[15*j+1],
						recStatus[15*j+2]));

			(pDOWorld->GetDOStatus())[ridx]
				->SetVelocity
					(NJRvector3d
						(recStatus[15*j+3],
						recStatus[15*j+4],
						recStatus[15*j+5]) );

			(pDOWorld->GetDOStatus())[ridx]
				->SetOrientation
					(NJRvector3d
						(recStatus[15*j+6],
						recStatus[15*j+7],
						recStatus[15*j+8]),
					NJRvector3d
						(recStatus[15*j+9],
						recStatus[15*j+10],
						recStatus[15*j+11]) );

			(pDOWorld->GetDOStatus())[ridx]
				->SetAngularVelocity
					(NJRvector3d
						(recStatus[15*j+12],
						recStatus[15*j+13],
						recStatus[15*j+14]) );
		}
	}
	delete[] recCnt;
	delete[] reqArray;
	delete[] statusArray;
	delete[] statusArray2;

/******************************************************************************
 * Aries' Comment (2006/04/19)
 *
 *    These codes are implemented by wokerla, which use file translation to
 *
 ******************************************************************************/
/*
	if (rank == MASTER)
    {
		this->MasterSyncWorld(vDO, cIact);
    }
	else
    {
		this->SlaveSyncWorld(vDO, cIact);
    }
*/
}

bool NBSGPConsultant::NextStep(DOContainer& vDO, IactContainer& cIact)
{
/*
    if (ulRoundCount == 1)
	{
		this->WriteDOSubWorld(vDO);
	}
*/
	bool rebuild = false;
	ulRoundCount++;
	pDOWorld->NextStep();
	bool HasMobileElement = true;
/*
	if (ISRecord())
	{
		this->WriteDOSubWorld(vDO);
	}
*/

	if (ISReset())
	{
		SyncWorld(vDO);
		RebuildIactRecordTab(cIact);
		rebuild = true;
		CleanUp(vDO, cIact);   // Check "ZOI"
		HasMobileElement = NBSGPConsultant::Reset();
	}

	if (ISRecord())
	{
		if (!rebuild)
		{
			SyncWorld(vDO);
			RebuildIactRecordTab(cIact);
			rebuild = true;
		}

		if (rank == MASTER)
        {
			RecordIDO();
        }
	}

	const SystemParameter* csp = pDOWorld->GetSystemParameter();

	if ( csp->GetTimeCurrent() >= csp->GetTimeStop() )
    {
		if (!rebuild)
        {
			SyncWorld(vDO);
			RebuildIactRecordTab(cIact);
			rebuild = true;
		}

		if (rank == MASTER)
        {
//			pDOWorld->WriteXML("terminate.xml");
			pDOWorld->WriteIDO("terminate.ido");
			pIRTbl  ->WriteIRT("terminate.irt");
		}
		//this->WriteDOSubWorld(vDO);
	}

	return HasMobileElement;
}

bool NBSGPConsultant::Reset()
{
	vcDO.clear();
	IactPairTab.clear();

	const SystemParameter* csp         = pDOWorld->GetSystemParameter();
	unsigned long          numberDO    = csp->GetDONumber();
	NJRvector3d            vFieldForce = csp->GetFieldForce();
	double                 dt          = culUpIact * csp->GetTimeInterval();

	vector<DOMap> vDOMap;
/*
	vector<DOMap> FTab;
	vector<DOMap> MTab;
	vector<DOMap> WorkTab;
	double xmin = numeric_limits<double>::max();
	double xmax = numeric_limits<double>::min();

	double ymin = numeric_limits<double>::max();
	double ymax = numeric_limits<double>::min();

	double zmin = numeric_limits<double>::max();
	double zmax = numeric_limits<double>::min();
*/

	vector<double> vecxloc;
	vector<double> vecyloc;
	vector<double> veczloc;
	vector<double> vecvloc;
	vector<double> vecrloc;

	double safeD;   // The safe distance of each element
	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	for (unsigned long i=0; i<numberDO; ++i)
	{
		vcDO.push_back(i);
		cpdos  = pDOWorld->GetDOStatus(i);
		cpdoml = pDOWorld->GetDOModel(cpdos->GetDOName());
/******************************************************************************
 * Aries' Comment (2006/03/31)
 *
 *    The Safety distance contains the influence of the radius, velocity, and
 * acceleration. However, the safety vector of sphere should be 1.1 ?
 ******************************************************************************/
		safeD
			= cpdoml->GetRange() * 1.1
			+ (cpdos->GetVelocity()).length() * 1.1 * dt
			+ 0.5 * dt * dt * vFieldForce.length();
/*
				xmin = min(xmin, cpdos->GetPosition().x());
				xmax = max(xmax, cpdos->GetPosition().x());
				ymin = min(ymin, cpdos->GetPosition().y());
				ymax = max(ymax, cpdos->GetPosition().y());
				zmin = min(zmin, cpdos->GetPosition().z());
				zmax = max(zmax, cpdos->GetPosition().z());
				vmax = max(vmax, cpdos->GetVelocity().length());
				rmax = cpdoml->GetShapeAttributes().sphere.radius;
*/
			vecxloc.push_back(cpdos->GetPosition().x());
			vecyloc.push_back(cpdos->GetPosition().y());
			veczloc.push_back(cpdos->GetPosition().z());
			vecvloc.push_back(cpdos->GetVelocity().length());
			vecrloc.push_back(cpdoml->GetRange());

		vDOMap.push_back(DOMap(i, cpdos, cpdoml, safeD));
	}

	//if(vecxloc.size() == 0)
	//{
	//	return false;
	//}

	sort(vecxloc.begin(), vecxloc.end());
	sort(vecyloc.begin(), vecyloc.end());
	sort(veczloc.begin(), veczloc.end());
	sort(vecvloc.begin(), vecvloc.end());
	sort(vecrloc.begin(), vecrloc.end());

/******************************************************************************
 * Aries' Comment (2006/03/31)
 *
 *    I modified these codes. Although they are uglier, but faster.
 ******************************************************************************/
	unsigned int NumOfSphere = vecxloc.size();
	double SphereXMin        = vecxloc[0];
	double SphereXMax        = vecxloc[NumOfSphere-1];
	double SphereYMin        = vecyloc[0];
	double SphereYMax        = vecyloc[NumOfSphere-1];
	double SphereZMin        = veczloc[0];
	double SphereZMax        = veczloc[NumOfSphere-1];
	double SphereVMax        = vecvloc[NumOfSphere-1];
	double SphereRMax        = vecrloc[NumOfSphere-1];

	// The maximal safe distance, Safety factor = 1.1 (1.1*2.0=2.2)
	double ZoneRange
		= SphereVMax * dt
		+ 2.2 * SphereRMax + 0.5 * dt * dt * vFieldForce.length();

	// Determine how many "safety region" per direction
	int ncelx = ceil((SphereXMax - SphereXMin) / ZoneRange);
	int ncely = ceil((SphereYMax - SphereYMin) / ZoneRange);
	int ncelz = ceil((SphereZMax - SphereZMin) / ZoneRange);

	map<Trir, vector<DOMap>* ,TrirLeY> locMap;
	vector<DOMap> GlobalElement;

	NJRvector3d pos;
	for (unsigned long i=0; i<numberDO; ++i)
	{
		if (vDOMap[i].cpdoml()->GetScope() == "local")
		{
			pos = vDOMap[i].cpdos()->GetPosition();
			Trir zone
				(static_cast<int>((pos.x() - SphereXMin) / ZoneRange),
				 static_cast<int>((pos.y() - SphereYMin) / ZoneRange),
				 static_cast<int>((pos.z() - SphereZMin) / ZoneRange) );

			if (locMap.find(zone) == locMap.end())
			{
				locMap.insert(make_pair(zone, new vector<DOMap>()));
			}
			locMap[zone]->push_back(vDOMap[i]);
		}
		else
		{
			GlobalElement.push_back(vDOMap[i]);
		}
	}

	map<Trir, vector<DOMap>*, TrirLeY>::iterator iter1, iter2;
	int ct = 0;   // Counter
	int ix, iy, iz;
	vector<DOMap>* pvec1 = 0;

	for (iter1=locMap.begin(); iter1!=locMap.end(); ++iter1, ++ct)
	{
		if ((ct % NP) != rank)
		{
			continue;
		}
		ix = iter1->first.xIdx;
		iy = iter1->first.yIdx;
		iz = iter1->first.zIdx;
		//cerr << "[" << rank << "] ct = " << ct << " ; zone = (" << ix << ',' << iy << ',' << iz << ")\n";

		pvec1 = iter1->second;
		// Self and neightbor zone check
		// Self and neightbor zone check
		if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz  ))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz+1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix  , iy-1, iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix  , iy-1, iz  ))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix  , iy-1, iz+1))) != locMap.end() )
		{
			BuildIactTab( *pvec1 , *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz  ))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz+1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix-1, iy  , iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix-1, iy  , iz  ))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix  , iy  , iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		if ( (iter2 = locMap.find(Trir(ix+1, iy  , iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}

		// Self and self detection
		BuildIactTab(*pvec1);

		// Self and fix element detection
		BuildIactTab(*pvec1, GlobalElement);
	}

	// For processor to exchange their detected IactPairs
	unsigned int* pNumIactPair     = new unsigned int[NP];
	unsigned int  NumLocalIactPair = IactPairTab.size();

	MPI_Allgather( &NumLocalIactPair, 1, MPI_UNSIGNED,
					pNumIactPair    , 1, MPI_UNSIGNED, MPI_COMM_WORLD);

	unsigned int NumAllIactPair = 0;
	int* recvcounts = new int[NP];
	for (unsigned int i=0; i<NP; ++i)
	{
		NumAllIactPair += pNumIactPair[i];
		recvcounts[i]   = pNumIactPair[i] * 2;
	}
	int* displs = new int[NP];
	displs[0] = 0;
	for (unsigned int i=1; i<NP; ++i)
	{
		displs[i] = displs[i-1] + recvcounts[i-1];
	}

	unsigned long* pLocalIactPair = new unsigned long[NumLocalIactPair * 2];
	for (unsigned int i=0; i<NumLocalIactPair; ++i)
	{
		pLocalIactPair[2*i]   = IactPairTab[i].first;
		pLocalIactPair[2*i+1] = IactPairTab[i].second ;
	}

	unsigned long* pAllIactPair = new unsigned long[NumAllIactPair * 2];

	MPI_Allgatherv( pLocalIactPair, 2*NumLocalIactPair, MPI_UNSIGNED_LONG,
					pAllIactPair  , recvcounts, displs, MPI_UNSIGNED_LONG,
					MPI_COMM_WORLD);

	this->IactPairTab.clear();
	for (unsigned int i=0; i<NumAllIactPair; ++i)
	{
		IactPairTab.push_back
			(make_pair(pAllIactPair[2*i], pAllIactPair[2*i+1]));
	}

	// Clear the LocMap data
	for (iter1 = locMap.begin(); iter1!=locMap.end() ; ++iter1)
	{
		delete iter1->second;
	}

	if (rank == MASTER)
	{
		cerr
			<< "[" << rank << "] "
			<< "Size of total IactTab = "
			<< IactPairTab.size()
			<< endl;
	}

	time(&starttime);

	//DistributeIactPair();
	GraphPartitionIactPair();

	time(&endtime);
	timePartitioning += (endtime - starttime);

	ConstructDOandOverLapTab();

	cerr
		<< "[" << rank << "] "
		<< "Size of Cutting Zone (ncelx, necly, ncelz) = ("
		<< ncelx << ", " << ncely << ", " << ncelz << ")\n";

	cerr
		<< "[" << rank << "] "
		<< "Size of Interaction = "
		<< (unsigned int)NBSGPConsultant::IactPairTab.size() << endl;

	cerr
		<< "[" << rank << "] "
		<< "Size of Overlap Table = (" << overlapTab[0].size();
		for (unsigned int i=1; i<NP; ++i)
		{
			cerr << ", " << overlapTab[i].size();
		}
		cerr << ")" << endl;

	delete[] pNumIactPair;
	delete[] pAllIactPair;
	delete[] pLocalIactPair;
	delete[] recvcounts;
	delete[] displs;

	return true;
};

void NBSGPConsultant::CleanUp(DOContainer &cDO, IactContainer &cIact)
{
	vector<unsigned long> RedundantElementGlobal;
	vector<unsigned long> RedundantElementLocal;
	map<unsigned long, long> ElementMappingJumpGlobal;

	unsigned long numberDOGlobal = pDOWorld->GetSystemParameter()->GetDONumber();
	unsigned long numberDOLocal  = DOTab.size();

	const DOStatus* pdos  = 0;
	const DOModel*  pdoml = 0;
	unsigned long ul;
	long ulDead;
	for(ul=0; ul<numberDOLocal; ul++)
	{
		pdos     = pDOWorld->GetDOStatus(GetDO(ul));
		pdoml    = pDOWorld->GetDOModel(pdos->GetDOName());
		DOMap pm = (DOMap(ul, pdos, pdoml, 0.0));
		if (DOMap::ISMobile(pm))
		{
			if (! (InBoundary(GetDO(ul))) )
			{
				RedundantElementLocal.push_back(GetDO(ul));
			}
		}
	}

	for(ul=0, ulDead=0; ul<numberDOGlobal; ul++)
	{
		pdos     = pDOWorld->GetDOStatus(ul);
		pdoml    = pDOWorld->GetDOModel(pdos->GetDOName());
		DOMap pm = (DOMap(ul, pdos, pdoml, 0.0));
		if (DOMap::ISMobile(pm))
		{
			if (! (InBoundary(ul)) )
			{
				RedundantElementGlobal.push_back(ul);
				ulDead--;
			}
		}
		ElementMappingJumpGlobal[ul] = ulDead;
	}

	if (!RedundantElementGlobal.empty())
	{
		if (rank == 0)
		{
			cerr
				<< '['
				<< rank
				<< "] Global Redundant elements = "
				<< RedundantElementGlobal.size()
				<< " / "
				<< numberDOGlobal
				<< endl;
		}

		if (!RedundantElementLocal.empty())
		{
			cerr
				<< '['
				<< rank
				<< "] Local Redundant elements = "
				<< RedundantElementLocal.size()
				<< " / "
				<< numberDOLocal
				<< endl;
			cDO.Erase(RedundantElementLocal);
		}

		pIRTbl->ModifyPair(ElementMappingJumpGlobal);
		pDOWorld->EraseDOStatus(RedundantElementGlobal);
		G2LTab.clear();
		G2LTab.resize(pDOWorld->GetSystemParameter()->GetDONumber());
		vcIactMaster.clear();
		vcIactSlave.clear();
		cIact.Clear();
	}
};

void NBSGPConsultant::BuildIactTab
	(vector<DOMap>& v1, vector<DOMap>& v2)
{
	for (unsigned long i=0; i<v1.size(); ++i)
	{
		for (unsigned long j=0; j<v2.size(); ++j)
    	{
			const IactModel* cpiactml
				= pDOWorld
					->GetIactModel
						(v1[i].cpdoml()->GetDOGroup(),
						v2[j].cpdoml()->GetDOGroup()  );

			if (cpiactml == 0)
    	    {
				continue;
        	}

			if (  (DOMap::CalDistance(v1[i], v2[j]))
				> (v1[i].SafeLength() + v2[j].SafeLength() ) )
	        {
				continue;
    	    }

			if (v1[i].id() < v2[j].id())
        	{
				IactPairTab.push_back(make_pair(v1[i].id(), v2[j].id()));
	        }
			else
    	    {
				IactPairTab.push_back(make_pair(v2[j].id(), v1[i].id()));
        	}
		}
	}
}

void NBSGPConsultant::BuildIactTab(vector<DOMap>& v)
{
	for (unsigned long i=0; i<v.size(); ++i)
    {
		for (unsigned long j=i+1; j<v.size(); ++j)
        {
			const IactModel* cpiactml
				= pDOWorld
					->GetIactModel
						(v[i].cpdoml()->GetDOGroup(),
						v[j].cpdoml()->GetDOGroup()  );

			if (cpiactml == 0)
	        {
				continue;
	        }

			if ((DOMap::CalDistance(v[i], v[j]))
				> (v[i].SafeLength() + v[j].SafeLength()) )
	        {
				continue;
        	}

			if ( v[i].id() < v[j].id())
            {
				IactPairTab.push_back(make_pair(v[i].id(), v[j].id()));
            }
			else
            {
				IactPairTab.push_back(make_pair(v[j].id(), v[i].id()));
            }
		}
    }
}

const ImpactStatus* NBSGPConsultant::RetrieveImpactStatus
	(unsigned long lcMaster, unsigned long lcSlave) const
{
	return pIRTbl->GetImpactStatus(DOTab[lcMaster], DOTab[lcSlave]);
}

void NBSGPConsultant::CollectUserDefinedData(IactContainer& cIact)
{
	// Every processor collect their-owned user-defined data from all "Interaction"s
	cIact.CollectUserDefinedData();

	// All processors collect and sum their-owned user-defined data
	double dUserDefinedDataSend[2*uNumUserDefinedData];
	for(unsigned u=0; u<2*uNumUserDefinedData; u++)
		dUserDefinedDataSend[u] = cIact.GetUserDefinedValue(u);

	double dUserDefinedDataSum[2*uNumUserDefinedData];
	for(unsigned u=0; u<2*uNumUserDefinedData; u++)
		dUserDefinedDataSum[u] = 0.0;

	MPI_Allreduce
		(&dUserDefinedDataSend,
		 &dUserDefinedDataSum,
		 2*uNumUserDefinedData,
		 MPI_DOUBLE,
		 MPI_SUM,
		 MPI_COMM_WORLD);

	for(unsigned u=0; u<2*uNumUserDefinedData; u++)
		dUDV[u] = dUserDefinedDataSum[u];
};

void NBSGPConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	NBSGPConsultant::CollectUserDefinedData(cIact);

	pIRTbl->Clear();
	for (unsigned int i=0; i<cIact.size(); ++i)
	{
		const Interaction* pInt = cIact.GetInteraction(i);
		if ( pInt->IsActive() )
			pIRTbl
				->PushRecord
					(IactPairTab[i].first,
					 IactPairTab[i].second,
					 *pInt->GetImpactStatus());
	}

	cerr
		<< "[" << rank << "] "
		<< "Size of local IactRecordTab = "
		<< pIRTbl->GetTabSize()
		<< endl;

/*
	this
		->VecHitRatio.push_back
			(pIRTbl->GetTabSize()/static_cast<double>(cIact.size()));
*/

	int tblSize = pIRTbl->GetTabSize();
	int *numRecords = new int[NP];

	MPI_Allgather(&tblSize, 1, MPI_INT, numRecords, 1, MPI_INT, MPI_COMM_WORLD);

	int *offPair    = new int[NP];
	int *offC_Bond  = new int[NP];
	int *offKn_SF   = new int[NP];
	int *offUDV     = new int[NP];

	int *recvPair   = new int[NP];
	int *recvC_Bond = new int[NP];
	int *recvKn_SF  = new int[NP];
	int *recvUDV    = new int[NP];

	int totalSize = 0;
	for (unsigned u=0; u<NP; ++u)
	{
		totalSize     +=                           numRecords[u];
		recvPair  [u]  = 2                       * numRecords[u];
		recvC_Bond[u]  = 2                       * numRecords[u];
		recvKn_SF [u]  = 4                       * numRecords[u];
		recvUDV   [u]  = 4 * uNumUserDefinedData * numRecords[u];
	}

	offPair  [0] = 0;
	offC_Bond[0] = 0;
	offKn_SF [0] = 0;
	offUDV   [0] = 0;
	for(unsigned int u2=1; u2<NP; ++u2)
    {
		offPair  [u2] = offPair  [u2-1] + 2                       * numRecords[u2-1];
		offC_Bond[u2] = offC_Bond[u2-1] + 2                       * numRecords[u2-1];
		offKn_SF [u2] = offKn_SF [u2-1] + 4                       * numRecords[u2-1];
		offUDV   [u2] = offUDV   [u2-1] + 4 * uNumUserDefinedData * numRecords[u2-1];
	}

	unsigned long* p       = new unsigned long [2                       * totalSize];
	unsigned*      c_bond  = new unsigned      [2                       * totalSize];
	double*        kn_sf   = new double        [4                       * totalSize];
	double*        udv     = new double        [4 * uNumUserDefinedData * totalSize];

	unsigned long* lp      = new unsigned long [2                       * tblSize];
	unsigned*      lc_bond = new unsigned      [2                       * tblSize];
	double*        lkn_sf  = new double        [4                       * tblSize];
	double*        ludv    = new double        [4 * uNumUserDefinedData * tblSize];

	map<pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator iter;

	const map<pair<unsigned long, unsigned long>, ImpactStatus>& m
		= pIRTbl->GetData();

	unsigned u = 0;
	NJRvector3d vTemp;
	const double* cdpudv;
	for (iter=m.begin(); iter!=m.end(); ++iter)
    {
		lp[2*u  ] = iter->first.first;
		lp[2*u+1] = iter->first.second;

		if(iter->second.Contact())
			lc_bond[2*u] = 1;
		else
			lc_bond[2*u] = 0;

		if(iter->second.Bond())
			lc_bond[2*u+1] = 1;
		else
			lc_bond[2*u+1] = 0;

		lkn_sf[4*u  ] = iter->second.Kn();
		vTemp         = iter->second.ShearForce();
		lkn_sf[4*u+1] = vTemp.x();
		lkn_sf[4*u+2] = vTemp.y();
		lkn_sf[4*u+3] = vTemp.z();

		cdpudv = iter->second.RetrieveAllUserDefinedValue();
		for(unsigned u2=0; u2<4*uNumUserDefinedData; u2++)
			ludv[4*uNumUserDefinedData*u+u2] = *(cdpudv+u2);

		u++;
	};

	MPI_Allgatherv
		(lp,
		2*tblSize,
		MPI_UNSIGNED_LONG,
		p,
		recvPair,
		offPair,
		MPI_UNSIGNED_LONG,
		MPI_COMM_WORLD    );

	MPI_Allgatherv
		(lc_bond,
		2*tblSize,
		MPI_UNSIGNED,
		c_bond,
		recvC_Bond,
		offC_Bond,
		MPI_UNSIGNED,
		MPI_COMM_WORLD);

	MPI_Allgatherv
		(lkn_sf,
		4*tblSize,
		MPI_DOUBLE,
		kn_sf,
		recvKn_SF,
		offKn_SF,
		MPI_DOUBLE,
		MPI_COMM_WORLD);

	MPI_Allgatherv
		(ludv,
		4*uNumUserDefinedData*tblSize,
		MPI_DOUBLE,
		udv,
		recvUDV,
		offUDV,
		MPI_DOUBLE,
		MPI_COMM_WORLD);

	ImpactStatus s;
	NJRvector3d NewShearForce;
	for (unsigned u=0; u<NP; ++u)
	{
		if (u == rank)
		{
			continue;
		};

		for (int i=0; i<numRecords[u]; ++i)
		{
			int offsetPair   = offPair[u];
			int offsetC_Bond = offC_Bond[u];
			int offsetKn_SF  = offKn_SF[u];
			int offsetUDV    = offUDV[u];

			bool bContact = true;
			if(c_bond[offsetC_Bond+2*i] == 0)
				bContact = false;

			s.SetContact(bContact);

			bool bBond    = true;
			if(c_bond[offsetC_Bond+2*i+1] == 0)
				bBond = false;

			s.SetBond(bBond);
			s.SetKn(kn_sf[offsetKn_SF+4*i]);
			NewShearForce.Set(kn_sf[offsetKn_SF+4*i+1], kn_sf[offsetKn_SF+4*i+2], kn_sf[offsetKn_SF+4*i+3]);
			s.SetShearForce(NewShearForce);
			s.SetAllUserDefinedValue(&udv[offsetUDV+4*uNumUserDefinedData*i]);

			pIRTbl->PushRecord(p[offsetPair+2*i], p[offsetPair+2*i+1], s);
		}
	}

	cerr
		<< "[" << rank << "] "
		<< "At RebuildIactRecordTab: size = "
		<< pIRTbl->GetTabSize()
		<< endl;

	delete[] numRecords;
	delete[] p;
	delete[] c_bond;
	delete[] kn_sf;
	delete[] udv;
	delete[] lp;
	delete[] lc_bond;
	delete[] lkn_sf;
	delete[] ludv;
	delete[] offPair;
	delete[] offC_Bond;
	delete[] offKn_SF;
	delete[] offUDV;
	delete[] recvPair;
	delete[] recvC_Bond;
	delete[] recvKn_SF;
	delete[] recvUDV;
}
