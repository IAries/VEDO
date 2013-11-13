#include <mpi.h>
#include <IrisGM/Interfaces/NBSParallelConsultant.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <Framework/Interfaces/Constants.h>
#include <NJR/Interfaces/Utility.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <iostream>
#include <iterator>
#include <map>

/******************************************************************************
 * The two undef prevents confliction min and max in std namespace conflicts
 * with the macros in <mpi.h>
 ******************************************************************************/
#undef max
#undef min

namespace vedo
{

unsigned long NBSParallelConsultant::GetDONum() const
{
	return (unsigned long) DOTab.size();
};

unsigned long NBSParallelConsultant::GetIactNum() const
{
	return (unsigned long)IactPairTab.size();
};

unsigned long NBSParallelConsultant::GetDO(unsigned long i) const
{
	return DOTab[i];
};

unsigned long NBSParallelConsultant::GetIactMaster (unsigned long i) const
{
	return G2LTab[IactPairTab[i].first];
};

unsigned long NBSParallelConsultant::GetIactSlave (unsigned long i) const
{
	return G2LTab[IactPairTab[i].second];
};

void NBSParallelConsultant::WriteDOSubWorld(DOContainer &vDO) const
{
	std::list<DOModel*> listModel;
	transform
		(pDOWorld->GetDOModel().begin(),
		pDOWorld->GetDOModel().end(),
		back_inserter(listModel),
		njr::Copy_obj()                 );

	std::list<IactModel*> listIModel;
	transform
		(pDOWorld->GetIactModel().begin(),
		pDOWorld->GetIactModel().end(),
		back_inserter(listIModel),
		njr::Copy_obj()                   );

	std::vector<DOStatus*> vecStatus;

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
			oSP->GetFieldAcceleration(),
			oSP->GetZoneOfInterest(),
			oSP->GetPeriodicBoundaryConditions());

    DOWorld w(ps,listModel, listIModel, vecStatus);

	char ltoa[256];
	std::string file = sfilename.c_str();
	file.append("_");

	sprintf(ltoa, "SUB_%d.xml\0", rank);
	file.append(ltoa);

	w.WriteXML (file.c_str());
}

NBSParallelConsultant::NBSParallelConsultant
	(DOWorld* DOWorld,
	IactRecordTab* pIactRecordTab,
	char filename[],
	unsigned long ulwrite,
	unsigned long ulupdate):
	Consultant(DOWorld, pIactRecordTab, filename, ulwrite),
	G2LTab
		(pDOWorld->GetSystemParameter()->GetDONumber(),
		 pDOWorld->GetSystemParameter()->GetDONumber()  )
{
	culUpIact = ulupdate;
	int u;
	MPI_Comm_size(MPI_COMM_WORLD, &u);
	NP = u;
	MPI_Comm_rank(MPI_COMM_WORLD, &u);
	rank = u;

	overlapTab.assign(NP, std::vector<unsigned long>());

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
			(std::pair<unsigned long, double*>
				(ImpactBufferSize, new double [ImpactBufferSize]) );

		recvBufVec.push_back
			(std::pair<unsigned long, double*>
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
			(std::make_pair
				(initialSyncCnt,
					std::make_pair
						(new unsigned long[initialSyncCnt],
						new double[initialSyncCnt * 15])   ));
//	}
//	else
//	{
		for (unsigned int i=0; i<NP; ++i)
        {
			syncRecvBufVec.push_back
				(std::make_pair
					(initialSyncCnt,
						std::make_pair
							(new unsigned long[initialSyncCnt],
							new double[initialSyncCnt * 15])   ));
        }
//	}

	//timeReset = 0.0;
	Reset();
}

NBSParallelConsultant::~NBSParallelConsultant()
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

	#ifdef _VEDO_DEBUG
		// for run time statistics
		//std::cout << '[' << rank << "] timeReset: " << timeReset/3600.0 << std::endl;
	#endif   // _VEDO_DEBUG

/*
    char ltoa[256];
	std::string file = sfilename.c_str();
	file.append("_");

	sprintf(ltoa,"vechit_%d.xml\0", rank );
	file.append(ltoa);

	std::ofstream outFile( file.c_str() );
	for (int i=0; i<this->VecHitRatio.size(); ++i)
    {
		outFile << this->VecHitRatio[i] << std::endl;
    }
	outFile.close();
*/
}

void NBSParallelConsultant::ConstructDOandOverLapTab()
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
};

void NBSParallelConsultant::SyncDOContainer(DOContainer & vDO)
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
			= std::max
				(ImpactBufferSize, std::max
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
		if( i == rank )
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
	MPI_Waitall(NP-1, reqArray, statusArray);
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
					(njr::Vector3d
						(impactRecBuf[6*j],
						impactRecBuf[6*j+1],
						impactRecBuf[6*j+2]),
					njr::Vector3d
						(impactRecBuf[6*j+3],
						impactRecBuf[6*j+4],
						impactRecBuf[6*j+5]) );
		}
	}
}

void NBSParallelConsultant::SyncWorld(DOContainer& vDO)
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
	// Ask How many elements each processor has
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
					(njr::Vector3d
						(recStatus[15*j],
						recStatus[15*j+1],
						recStatus[15*j+2]));

			(pDOWorld->GetDOStatus())[ridx]
				->SetVelocity
					(njr::Vector3d
						(recStatus[15*j+3],
						recStatus[15*j+4],
						recStatus[15*j+5]) );

			(pDOWorld->GetDOStatus())[ridx]
				->SetOrientation
					(njr::Vector3d
						(recStatus[15*j+6],
						recStatus[15*j+7],
						recStatus[15*j+8]),
					njr::Vector3d
						(recStatus[15*j+9],
						recStatus[15*j+10],
						recStatus[15*j+11]) );

			(pDOWorld->GetDOStatus())[ridx]
				->SetAngularVelocity
					(njr::Vector3d
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

bool NBSParallelConsultant::NextStep(DOContainer& vDO, IactContainer& cIact)
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
		CleanUp(vDO, cIact);
		HasMobileElement = NBSParallelConsultant::Reset();
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
			pDOWorld->WriteIDO("terminate.ido", pIRTbl);
		}
		//this->WriteDOSubWorld(vDO);
	}

	return HasMobileElement;
}

bool NBSParallelConsultant::Reset()
{
	vcDO.clear();
	IactPairTab.clear();

    unsigned long numberDO = pDOWorld->GetSystemParameter()->GetDONumber();

	std::vector<double> vecxloc;
	std::vector<double> vecyloc;
	std::vector<double> veczloc;
	std::vector<double> vecvloc;
	std::vector<double> vecrloc;

	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	for (unsigned long i=0; i<numberDO; ++i)
	{
		vcDO.push_back(i);
		cpdos  = pDOWorld->GetDOStatus(i);
		cpdoml = pDOWorld->GetDOModel(cpdos->GetDOName());

		if (cpdoml->GetScope() == "local")
		{
			vecxloc.push_back(cpdos->GetPosition().x());
			vecyloc.push_back(cpdos->GetPosition().y());
			veczloc.push_back(cpdos->GetPosition().z());
			vecvloc.push_back(cpdos->GetVelocity().length());
			vecrloc.push_back(cpdoml->GetRange());
		}
	}

	//if(vecxloc.size() == 0)
	//{
	//	return false;
	//}

	// Determine the longest direction - "Cutting Direction"
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
	double DiscardRatio      = 0.02;
	double TempDouble        = 1.0 - DiscardRatio;

	double xSpan
		= vecxloc[TempDouble*NumOfSphere-1] - vecxloc[DiscardRatio*NumOfSphere];

	double ySpan
		= vecyloc[TempDouble*NumOfSphere-1] - vecyloc[DiscardRatio*NumOfSphere];

	double zSpan
		= veczloc[TempDouble*NumOfSphere-1] - veczloc[DiscardRatio*NumOfSphere];

	if (xSpan >= ySpan)
	{
		if (xSpan >= zSpan)
		{
			subReset
				(TrirLeX(),
				SphereXMin,
				SphereXMax,
				SphereYMin,
				SphereYMax,
				SphereZMin,
				SphereZMax,
				SphereVMax,
				SphereRMax);
		}
		else
		{
			subReset
				(TrirLeZ(),
				SphereXMin,
				SphereXMax,
				SphereYMin,
				SphereYMax,
				SphereZMin,
				SphereZMax,
				SphereVMax,
				SphereRMax);
		}
	}
	else
	{
		if (ySpan >= zSpan)
		{
			subReset
				(TrirLeY(),
				SphereXMin,
				SphereXMax,
				SphereYMin,
				SphereYMax,
				SphereZMin,
				SphereZMax,
				SphereVMax,
				SphereRMax);
		}
		else
		{
			subReset
				(TrirLeZ(),
				SphereXMin,
				SphereXMax,
				SphereYMin,
				SphereYMax,
				SphereZMin,
				SphereZMax,
				SphereVMax,
				SphereRMax);
		}
	}
/*
	double xSpan
		= vecxloc[vecxloc.size()*(1.0-DiscardRatio)-1]
		- vecxloc[DiscardRatio*vecxloc.size()];
	double ySpan
		= vecyloc[vecyloc.size()*(1.0-DiscardRatio)-1]
		- vecyloc[DiscardRatio*vecyloc.size()];
	double zSpan
		= veczloc[veczloc.size()*(1.0-DiscardRatio)-1]
		- veczloc[DiscardRatio*veczloc.size()];

	double maxSpan = max(xSpan, max(ySpan,zSpan));

    if (xSpan == maxSpan)
    {
		subReset(TrirLeX());
    }
	else if (ySpan == maxSpan)
	{
		subReset(TrirLeY());
	}
	else
	{
		subReset(TrirLeZ());
    }
*/
	return true;
};

bool NBSParallelConsultant::CleanUp(DOContainer &cDO, IactContainer &cIact)
{
	SystemParameter* csp   = pDOWorld->GetSystemParameter();
	if (csp->GetZoneOfInterest().Active())
	{
		std::vector<unsigned long> RedundantElementGlobal;
		std::vector<unsigned long> RedundantElementLocal;
		std::map<unsigned long, long> ElementMappingJumpGlobal;

		unsigned long numberDOGlobal = csp->GetDONumber();
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
			#ifdef _VEDO_DEBUG
				if (rank == 0)
					std::cout
						<< '['
						<< rank
						<< "] Global Redundant elements = "
						<< RedundantElementGlobal.size()
						<< " / "
						<< numberDOGlobal
						<< std::endl;
			#endif   // _VEDO_DEBUG

			if (!RedundantElementLocal.empty())
			{
				#ifdef _VEDO_DEBUG
					std::cout
						<< '['
						<< rank
						<< "] Local Redundant elements = "
						<< RedundantElementLocal.size()
						<< " / "
						<< numberDOLocal
						<< std::endl;
				#endif   // _VEDO_DEBUG

				cDO.Erase(RedundantElementLocal);
			}

			pIRTbl->ModifyPair(ElementMappingJumpGlobal);
			pDOWorld->EraseDOStatus(RedundantElementGlobal);
			G2LTab.clear();
			G2LTab.resize(pDOWorld->GetSystemParameter()->GetDONumber());
			vcIactMaster.clear();
			vcIactSlave.clear();
			cIact.Clear();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
};

void NBSParallelConsultant::BuildIactTab
	(std::vector<DOMap>& v1, std::vector<DOMap>& v2)
{
	const Boundary* pbc
		= &(pDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions());

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

			if (  (DOMap::CalDistance(v1[i], v2[j], pbc))
				> (v1[i].SafeLength() + v2[j].SafeLength() ) )
	        {
				continue;
    	    }

			if (v1[i].id() < v2[j].id())
        	{
				IactPairTab.push_back(std::make_pair(v1[i].id(), v2[j].id()));
	        }
			else
    	    {
				IactPairTab.push_back(std::make_pair(v2[j].id(), v1[i].id()));
        	}
		}
	}
}

void NBSParallelConsultant::BuildIactTab(std::vector<DOMap>& v)
{
	const Boundary* pbc = &(pDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions()); //PBC by Liao 2009/5/28

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

			if ((DOMap::CalDistance(v[i], v[j], pbc))
				> (v[i].SafeLength() + v[j].SafeLength()) )
	        {
				continue;
        	}

			if ( v[i].id() < v[j].id())
            {
				IactPairTab.push_back(std::make_pair(v[i].id(), v[j].id()));
            }
			else
            {
				IactPairTab.push_back(std::make_pair(v[j].id(), v[i].id()));
            }
		}
    }
}

void NBSParallelConsultant::DistributeIactPair()
{
	unsigned long avgIact = IactPairTab.size() / NP;
	std::vector< std::pair<unsigned long, unsigned long> > tmpIactVec;
	tmpIactVec.reserve(avgIact + NP);
	if (rank != (NP-1))
    {
	    tmpIactVec.assign
	    	(IactPairTab.begin() +  rank      * avgIact,
			 IactPairTab.begin() + (rank + 1) * avgIact - 1);
	}
	else
    {
		tmpIactVec.assign
			(IactPairTab.begin() + rank * avgIact, IactPairTab.end());
	}

	IactPairTab.swap(tmpIactVec);   // For higher efficiency, instead of "IactPairTab = tmpIactVec;"
}

const ImpactStatus* NBSParallelConsultant::RetrieveImpactStatus
	(unsigned long lcMaster, unsigned long lcSlave) const
{
	return pIRTbl->GetImpactStatus(DOTab[lcMaster], DOTab[lcSlave]);
}

void NBSParallelConsultant::CollectUserDefinedData(IactContainer& cIact)
{
	if(uNumUDDImpactStatus != 0)
	{
		// Every processor collect their-owned user-defined data from all "Interaction"s
		cIact.CollectUserDefinedData();

		// All processors collect and sum their-owned user-defined data
		double* dpUserDefinedDataSend = new double[2*uNumUDDImpactStatus];
		for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
			*(dpUserDefinedDataSend+u) = cIact.GetUserDefinedValue(u);

		double* dpUserDefinedDataSum = new double(2*uNumUDDImpactStatus);
		for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
			*(dpUserDefinedDataSum+u) = 0.0;

		MPI_Allreduce
			(dpUserDefinedDataSend,
			 dpUserDefinedDataSum ,
			 2*uNumUDDImpactStatus,
			 MPI_DOUBLE           ,
			 MPI_SUM              ,
			 MPI_COMM_WORLD        );

        memcpy(dpUDVIS, dpUserDefinedDataSum, 2*uNumUDDImpactStatus*sizeof(double));
	}
};

void NBSParallelConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	NBSParallelConsultant::CollectUserDefinedData(cIact);

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

	#ifdef _VEDO_DEBUG
		std::cout
			<< "[" << rank << "] "
			<< "Size of local IactRecordTab = "
			<< pIRTbl->GetTabSize()
			<< std::endl;
	#endif   // _VEDO_DEBUG

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
		recvUDV   [u]  = 4 * uNumUDDImpactStatus * numRecords[u];
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
		offUDV   [u2] = offUDV   [u2-1] + 4 * uNumUDDImpactStatus * numRecords[u2-1];
	}

	unsigned long* p       = new unsigned long [2                       * totalSize];
	unsigned*      c_bond  = new unsigned      [2                       * totalSize];
	double*        kn_sf   = new double        [4                       * totalSize];
	double*        udv     = new double        [4 * uNumUDDImpactStatus * totalSize];

	unsigned long* lp      = new unsigned long [2                       * tblSize];
	unsigned*      lc_bond = new unsigned      [2                       * tblSize];
	double*        lkn_sf  = new double        [4                       * tblSize];
	double*        ludv    = new double        [4 * uNumUDDImpactStatus * tblSize];

	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator iter;

	const std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>& m
		= pIRTbl->GetData();

	unsigned u = 0;
	njr::Vector3d vTemp;
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
		for(unsigned u2=0; u2<4*uNumUDDImpactStatus; u2++)
			ludv[4*uNumUDDImpactStatus*u+u2] = *(cdpudv+u2);

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
		4*uNumUDDImpactStatus*tblSize,
		MPI_DOUBLE,
		udv,
		recvUDV,
		offUDV,
		MPI_DOUBLE,
		MPI_COMM_WORLD);

	ImpactStatus s;
	njr::Vector3d NewShearForce;
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
			s.SetAllUserDefinedValue(&udv[offsetUDV+4*uNumUDDImpactStatus*i]);

			pIRTbl->PushRecord(p[offsetPair+2*i], p[offsetPair+2*i+1], s);
		}
	}

	#ifdef _VEDO_DEBUG
		std::cout
			<< "[" << rank << "] "
			<< "At RebuildIactRecordTab: size = "
			<< pIRTbl->GetTabSize()
			<< std::endl;
	#endif   // _VEDO_DEBUG

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
};

};   // namespace vedo
