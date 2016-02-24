#include <mpi.h>
#include <aries/utility/Constants.h>
#include <njr/Utility.h>
#include <vedo/common/NBSConsultant.h>
#include <vedo/iris-gm/NBSParallelConsultant.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <limits>
#include <iostream>
#include <iterator>
#include <map>

// The two undef prevents confliction min and max in std namespace conflicts with the macros in <mpi.h>
#undef max
#undef min

namespace vedo
{

aries::Constants* aries_cp = aries::Constants::Instance();

_uint_t NBSParallelConsultant::GetDONum() const
{
	return (_uint_t)vuDOGlobalID.size();
}

_uint_t NBSParallelConsultant::GetIactNum() const
{
	return (_uint_t)IactPairTab.size();
}

_uint_t NBSParallelConsultant::GetDO(_uint_t i) const
{
	return vuDOGlobalID[i];
}

_uint_t NBSParallelConsultant::GetIactMaster (_uint_t i) const
{
	return G2LTab[IactPairTab[i].first];
}

_uint_t NBSParallelConsultant::GetIactSlave (_uint_t i) const
{
	return G2LTab[IactPairTab[i].second];
}

void NBSParallelConsultant::WriteDOSubWorld(DOContainer &vDO) const
{
	std::list<DOModel*> listModel;
	transform(pDOWorld->GetDOModel().begin(), pDOWorld->GetDOModel().end(), back_inserter(listModel), njr::Copy_obj());

	std::list<IactModel*> listIModel;
	transform(pDOWorld->GetIactModel().begin(), pDOWorld->GetIactModel().end(), back_inserter(listIModel), njr::Copy_obj());

	std::vector<DOStatus*> vecStatus;

	for (_uint_t i=0; i<vDO.size(); ++i)
	{
		vecStatus.push_back(new DOStatus( *(vDO[i]->GetDOStatus())));
	}

	SystemParameter* oSP = new SystemParameter(*(pDOWorld->GetSystemParameter()));
	oSP->SetDONumber(vDO.size());
    DOWorld w(oSP, listModel, listIModel, vecStatus);

	char ltoa[256];
	std::string file = sfilename.c_str();
	file.append("_");

	sprintf(ltoa, "SUB_%d.xml\0", rank);
	file.append(ltoa);

	w.WriteXML (file.c_str());
}

NBSParallelConsultant::NBSParallelConsultant
	(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, _uint_t ulwrite, _uint_t ulupdate):
		Consultant(DOWorld, pIactRecordTab, filename, ulwrite),
		G2LTab(pDOWorld->GetSystemParameter()->GetDONumber(), pDOWorld->GetSystemParameter()->GetDONumber())
{
	culUpIact = ulupdate;
	int u;
	MPI_Comm_size(MPI_COMM_WORLD, &u);
	NP = u;
	MPI_Comm_rank(MPI_COMM_WORLD, &u);
	rank = u;

	overlapTab.assign(NP, std::vector<_uint_t>());

/******************************************************************************
 * Aries' Comment (2006/04/05)
 *
 *    Maybe we can find some rule between "the number of elements" and
 * "initialImpactCnt", then we can determine the initial buffer for some special
 * case.
 ******************************************************************************/
	//_uint_t initialImpactCnt = ImpactBufferSize;

	for (_uint_t i=0; i<NP; ++i)
    {
		sendBufVec.push_back(std::pair<_uint_t, _float_t*>(ImpactBufferSize, new _float_t [ImpactBufferSize]));
		recvBufVec.push_back(std::pair<_uint_t, _float_t*>(ImpactBufferSize, new _float_t [ImpactBufferSize]));
	}

	_uint_t initialSyncCnt = 1.2 * (pDOWorld->GetSystemParameter()->GetDONumber());

/******************************************************************************
 * Aries' Comment (2006/02/25)
 *
 *    For meeeage passing with MPI, we have to expend the size of buffer
 * parallel environmnent, this is useless.
 ******************************************************************************/
//	if (rank != MASTER)
//	{
		syncSendBufVec.push_back
			(std::make_pair
				(initialSyncCnt,
				 std::make_pair
				 	(new _uint_t[initialSyncCnt],
				 	 new _float_t[initialSyncCnt * 21])));
//	}
//	else
//	{
		for (_uint_t i=0; i<NP; ++i)
        {
			syncRecvBufVec.push_back
				(std::make_pair
					(initialSyncCnt,
						std::make_pair
							(new _uint_t[initialSyncCnt],
							 new _float_t[initialSyncCnt * 21])));
		}
//	}

	//timeReset = 0.0;
	Reset();
}

NBSParallelConsultant::~NBSParallelConsultant()
{
	for (_uint_t i=0; i<NP; ++i)
	{
		delete sendBufVec[i].second;
		delete recvBufVec[i].second;
	}
	for (_uint_t i=0; i<syncSendBufVec.size(); ++i)
	{
		delete syncSendBufVec[i].second.first;
		delete syncSendBufVec[i].second.second;
	}
	for (_uint_t i=0; i<syncRecvBufVec.size(); ++i)
	{
		delete syncRecvBufVec[i].second.first;
		delete syncRecvBufVec[i].second.second;
	}

	#ifdef _DEBUG
		// for run time statistics
		//std::cout << '[' << rank << "] timeReset: " << timeReset/3600.0 << std::endl;
	#endif   // _DEBUG

/*
    char ltoa[256];
	std::string file = sfilename.c_str();
	file.append("_");

	sprintf(ltoa,"vechit_%d.xml\0", rank );
	file.append(ltoa);

	std::ofstream outFile( file.c_str() );
	for (_uint_t i=0; i<this->VecHitRatio.size(); ++i)
    {
		outFile << this->VecHitRatio[i] << std::endl;
    }
	outFile.close();
*/
}

void NBSParallelConsultant::ConstructDOandOverLapTab()
{
	vuDOGlobalID.clear();
	_uint_t DONum = pDOWorld->GetSystemParameter()->GetDONumber();
	G2LTab.reserve(DONum);
	G2LTab.assign(DONum, DONum);
    for (_uint_t i=0; i<NP; ++i)
    {
		overlapTab[i].clear();
    }

	// Contrust the DO Table and G2LTab
	for (_uint_t i=0; i<IactPairTab.size(); ++i)
    {
		G2LTab[IactPairTab[i].first ] = 0;
		G2LTab[IactPairTab[i].second] = 0;
	}

	for (_uint_t i=0; i<DONum; i++)
    {
		//if ((G2LTab[i] == DONum) && (pDOWorld->GetDOModel(pDOWorld->GetDOName(i))->GetScope() != DOScopeType::global))
		if (G2LTab[i] == DONum)
		{
			//G2LTab[i] = 0;
			continue;
        }
		vuDOGlobalID.push_back(i);
		G2LTab[i] = vuDOGlobalID.size() - 1;
	}

	char* occur    = new char[DONum   ];
	char* evpoccur = new char[DONum*NP];

	// Contrust the overlapTable
	for (_uint_t i=0; i<DONum; ++i)
    {
		occur[i] = (G2LTab[i]==DONum) ? static_cast<char>(0) : static_cast<char>(1);
    }

    MPI_Allgather(occur, (_int_t)DONum, MPI_CHAR, evpoccur, (_int_t)DONum, MPI_CHAR, MPI_COMM_WORLD);

	for (_uint_t i=0; i<NP; ++i)
    {
		if (i == rank)
        {
			continue;
        }
		for (_uint_t j=0; j<DONum; ++j)
        {
			if ((occur[j]==static_cast<char>(1)) && (evpoccur[i*DONum+j]==static_cast<char>(1)))
            {
				overlapTab[i].push_back(j);
            }
		}
	}

	// Pick the orphan
	if (rank == MASTER)
    {
		for(_uint_t i=0; i<DONum; i++)
        {
			_uint_t sum = 0;
			for (_uint_t j=0; j<NP; ++j)
            {
				sum += static_cast<_uint_t>(evpoccur[j*DONum+i]);
            }
			if (sum == 0)
            {
				vuDOGlobalID.push_back(i);
				G2LTab[i] = vuDOGlobalID.size()-1;
			}
		}
	}
	delete[] occur;
	delete[] evpoccur;
}

void NBSParallelConsultant::SyncDOContainer(DOContainer & vDO)
{
	for(_uint_t i=0; i<overlapTab.size(); i++)
    {
		EnsureLength(6, overlapTab[i].size(), sendBufVec[i].first, sendBufVec[i].second);
		EnsureLength(6, overlapTab[i].size(), recvBufVec[i].first, recvBufVec[i].second);
		ImpactBufferSize = std::max(ImpactBufferSize, std::max(sendBufVec[i].first, recvBufVec[i].first));
	}

	// Send impact
	MPI_Request*  reqArray     = new MPI_Request[NP-1];
	MPI_Status*   statusArray  = new MPI_Status [NP-1];
    _uint_t       idxReq       = 0;
	_uint_t       idx          = 0;
	_float_t*     impactSedBuf = 0;
	for(_uint_t i=0; i<overlapTab.size(); i++)
    {
		if(i == rank)
        {
			continue;
		}
		impactSedBuf = sendBufVec[i].second;
		for(_uint_t j=0; j<overlapTab[i].size(); j++)
		{
			idx = G2LTab[overlapTab[i][j]];
			impactSedBuf[6*j]   = vDO[idx]->GetImpact().x();
			impactSedBuf[6*j+1] = vDO[idx]->GetImpact().y();
			impactSedBuf[6*j+2] = vDO[idx]->GetImpact().z();
			impactSedBuf[6*j+3] = vDO[idx]->GetAngularImpact().x();
			impactSedBuf[6*j+4] = vDO[idx]->GetAngularImpact().y();
			impactSedBuf[6*j+5] = vDO[idx]->GetAngularImpact().z();
		}
		//assistant.transferImpactSend(impactSedBuf, 6*overlapTab[i].size(), i);
		MPI_Isend(impactSedBuf, 6*overlapTab[i].size(), MPI_DOUBLE, (_int_t)i, 1, MPI_COMM_WORLD, &reqArray[idxReq]);
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
	MPI_Status*   statusArray2 = new MPI_Status [NP-1];
	              idxReq       = 0;
	_float_t* impactRecBuf = 0;
	for (_uint_t i=0; i<overlapTab.size(); i++)
    {
		if (i == rank)
        {
			continue;
        }
		impactRecBuf = recvBufVec[i].second;
		//assistant.transferImpactRecv(impactRecBuf,  6*overlapTab[i].size(), i);
		MPI_Recv(impactRecBuf, 6*overlapTab[i].size(), MPI_DOUBLE, (_int_t)i, 1, MPI_COMM_WORLD, &statusArray2[idxReq]);
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
	for (_uint_t i=0; i<overlapTab.size(); ++i)
    {
        if(i == rank)
        {
			continue;
        }
		impactRecBuf = recvBufVec[i].second;
		for(_uint_t j=0; j<overlapTab[i].size(); j++)
        {
			idx = G2LTab[ overlapTab[i][j] ];
			vDO[idx]->AddImpact
				(vedo::Vector3df(impactRecBuf[6*j  ], impactRecBuf[6*j+1], impactRecBuf[6*j+2]),
				 vedo::Vector3df(impactRecBuf[6*j+3], impactRecBuf[6*j+4], impactRecBuf[6*j+5]) );
		}
	}
}

void NBSParallelConsultant::SyncWorld(DOContainer& vDO)
{
	//To ensure the size is enough
	/*
	if (this->rank == MASTER)
	{
		for (_uint_t i=0; i<NP; ++i)
		{
			EnsureLength
				(21,
				pDOWorld->GetSystemParameter()->GetDONumber(),
				this->syncRecvBufVec[i].first,
				syncRecvBufVec[i].second);
		}
	}
	else
	{
		EnsureLength
			(21,
			vuDOGlobalID.size(),
			syncSendBufVec[0].first,
			syncSendBufVec[0].second);
	}
	*/

	// Ask How many elements each processor has
	_uint_t* recCnt = new _uint_t[NP];
	_uint_t       s = vDO.size();
	#ifdef _INT32
		MPI_Allgather(&s, 1, MPI_UNSIGNED, recCnt, 1, MPI_UNSIGNED, MPI_COMM_WORLD);
	#else
		#ifdef _INT64
			MPI_Allgather(&s, 1, MPI_UNSIGNED_LONG_LONG, recCnt, 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#else
			MPI_Allgather(&s, 1, MPI_UNSIGNED_LONG_LONG, recCnt, 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#endif   // _INT64
	#endif   // _INT32

	// Update the information of elements from local processor
	const DOStatus* SourceDOStatus = 0;
	      DOStatus* TargetDOStatus = 0;
	for (_uint_t i=0; i<s; ++i)
    {
		SourceDOStatus = vDO[i]->GetDOStatus();
		TargetDOStatus = (pDOWorld->GetDOStatus())[ vuDOGlobalID[i] ];
		TargetDOStatus->SetPosition(SourceDOStatus->GetPosition());
		TargetDOStatus->SetVelocity(SourceDOStatus->GetVelocity());
		TargetDOStatus->SetImpact(SourceDOStatus->GetImpact());
		TargetDOStatus->SetAngularImpact(SourceDOStatus->GetAngularImpact());
		TargetDOStatus->SetOrientation(SourceDOStatus->GetOrientationX(), SourceDOStatus->GetOrientationZ());
		TargetDOStatus->SetAngularVelocity(SourceDOStatus->GetAngularVelocity());
	}

	// Get the information of elements from the other processors
	// Send the information to the other processors
	MPI_Request* reqArray    = new MPI_Request[2*NP-2];
	MPI_Status*  statusArray = new MPI_Status [2*NP-2];
	_uint_t      sedCnt      = vuDOGlobalID.size();
	_uint_t*     sedID       = syncSendBufVec[0].second.first;
	_float_t*    sedStatus   = syncSendBufVec[0].second.second;
    _uint_t      idxReq      = 0;

	for (_uint_t i=0; i<NP; i++)
	{
		if (i == rank)
        {
			continue;
		}
/*
		for (_uint_t j=0; j<sedCnt; ++j)
		{
			sedID[j] = vuDOGlobalID[j];
			const DOStatus* dos = vDO[j]->GetDOStatus();
			sedStatus[21*j]     = dos->GetPosition().x();
			sedStatus[21*j+1]   = dos->GetPosition().y();
			sedStatus[21*j+2]   = dos->GetPosition().z();
			sedStatus[21*j+3]   = dos->GetVelocity().x();
			sedStatus[21*j+4]   = dos->GetVelocity().y();
			sedStatus[21*j+5]   = dos->GetVelocity().z();
			sedStatus[21*j+6]   = dos->GetOrientationX().x();
			sedStatus[21*j+7]   = dos->GetOrientationX().y();
			sedStatus[21*j+8]   = dos->GetOrientationX().z();
			sedStatus[21*j+9]   = dos->GetOrientationZ().x();
			sedStatus[21*j+10]  = dos->GetOrientationZ().y();
			sedStatus[21*j+11]  = dos->GetOrientationZ().z();
			sedStatus[21*j+12]  = dos->GetAngularVelocity().x();
			sedStatus[21*j+13]  = dos->GetAngularVelocity().y();
			sedStatus[21*j+14]  = dos->GetAngularVelocity().z();
			sedStatus[21*j+15]  = dos->GetImpact().x();
			sedStatus[21*j+16]  = dos->GetImpact().y();
			sedStatus[21*j+17]  = dos->GetImpact().z();
			sedStatus[21*j+18]  = dos->GetAngularImpact().x();
			sedStatus[21*j+19]  = dos->GetAngularImpact().y();
			sedStatus[21*j+20]  = dos->GetAngularImpact().z();
		}
*/
		for (_uint_t j=0, j0=0; j<sedCnt; ++j)
		{
			sedID[j] = vuDOGlobalID[j];
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
			sedStatus[j0++] = dos->GetImpact().x();
			sedStatus[j0++] = dos->GetImpact().y();
			sedStatus[j0++] = dos->GetImpact().z();
			sedStatus[j0++] = dos->GetAngularImpact().x();
			sedStatus[j0++] = dos->GetAngularImpact().y();
			sedStatus[j0++] = dos->GetAngularImpact().z();
		}

		#ifdef _INT32
			MPI_Isend(sedID, (_int_t)sedCnt , MPI_UNSIGNED, (_int_t)i, 1, MPI_COMM_WORLD, &reqArray[idxReq]);
		#else
			#ifdef _INT64
				MPI_Isend(sedID, (_int_t)sedCnt , MPI_UNSIGNED_LONG_LONG, (_int_t)i, 1, MPI_COMM_WORLD, &reqArray[idxReq]);
			#else
				MPI_Isend(sedID, (_int_t)sedCnt , MPI_UNSIGNED_LONG_LONG, (_int_t)i, 1, MPI_COMM_WORLD, &reqArray[idxReq]);
			#endif   // _INT64
		#endif   // _INT32
		idxReq++;
		MPI_Isend(sedStatus, 21*(_int_t)sedCnt, MPI_DOUBLE , (_int_t)i, 2, MPI_COMM_WORLD, &reqArray[idxReq]);
		idxReq++;
	}

	// Receive the information from the other processors
    _uint_t*    recID        = 0;
	_float_t*   recStatus    = 0;
	            idxReq       = 0;
	MPI_Status* statusArray2 = new MPI_Status [2*NP-2];
	for (_uint_t i=0; i<NP; ++i)
    {
		if (i == rank)
        {
			continue;
        }
		recID     = syncRecvBufVec[i].second.first;
		recStatus = syncRecvBufVec[i].second.second;
		#ifdef _INT32
			MPI_Recv(recID, (_int_t)recCnt[i], MPI_UNSIGNED, (_int_t)i, 1, MPI_COMM_WORLD, &statusArray2[idxReq]);
		#else
			#ifdef _INT64
				MPI_Recv(recID, (_int_t)recCnt[i], MPI_UNSIGNED_LONG_LONG, (_int_t)i, 1, MPI_COMM_WORLD, &statusArray2[idxReq]);
			#else
				MPI_Recv(recID, (_int_t)recCnt[i], MPI_UNSIGNED_LONG_LONG, (_int_t)i, 1, MPI_COMM_WORLD, &statusArray2[idxReq]);
			#endif   // _INT64
		#endif   // _INT32
		idxReq++;
		MPI_Recv(recStatus, 21*(_int_t)recCnt[i], MPI_DOUBLE, (_int_t)i, 2, MPI_COMM_WORLD, &statusArray2[idxReq]);
		idxReq++;
	}

	// Waitall to ensure every "MPI_Isend" has completed.
	MPI_Waitall(2*NP-2, reqArray, statusArray);

	// Update the information of elements from the other processores
	_uint_t ridx = 0;
	for(_uint_t i=0; i<NP; ++i)
    {
		if (i == rank)
        {
			continue;
        }
		recID     = syncRecvBufVec[i].second.first;
		recStatus = syncRecvBufVec[i].second.second;
		for (_uint_t j=0; j<recCnt[i]; ++j)
        {
			ridx = recID[j];
			if ((G2LTab[ridx] >= 0) && (G2LTab[ridx] < vuDOGlobalID.size()))
            {
				continue;
			}
			(pDOWorld->GetDOStatus())[ridx]->SetPosition
				(vedo::Vector3df(recStatus[21*j   ], recStatus[21*j+ 1], recStatus[21*j+ 2]));

			(pDOWorld->GetDOStatus())[ridx]->SetVelocity
				(vedo::Vector3df(recStatus[21*j+ 3], recStatus[21*j+ 4], recStatus[21*j+ 5]));

			(pDOWorld->GetDOStatus())[ridx]->SetOrientation
				(vedo::Vector3df(recStatus[21*j+ 6], recStatus[21*j+ 7], recStatus[21*j+ 8]),
				 vedo::Vector3df(recStatus[21*j+ 9], recStatus[21*j+10], recStatus[21*j+11]) );

			(pDOWorld->GetDOStatus())[ridx]->SetAngularVelocity
				(vedo::Vector3df(recStatus[21*j+12], recStatus[21*j+13], recStatus[21*j+14]));

			(pDOWorld->GetDOStatus())[ridx]->SetImpact
				(vedo::Vector3df(recStatus[21*j+15], recStatus[21*j+16], recStatus[21*j+17]));

			(pDOWorld->GetDOStatus())[ridx]->SetAngularImpact
				(vedo::Vector3df(recStatus[21*j+18], recStatus[21*j+19], recStatus[21*j+20]));
		}
	}
	delete[] recCnt;
	delete[] reqArray;
	delete[] statusArray;
	delete[] statusArray2;
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

	cIact.BackupImpactStatus();

	const SystemParameter* csp = pDOWorld->GetSystemParameter();

	if (csp->GetTimeCurrent() >= csp->GetTimeStop())
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

    _uint_t numberDO = pDOWorld->GetSystemParameter()->GetDONumber();

	std::vector<_float_t> vecxloc;
	std::vector<_float_t> vecyloc;
	std::vector<_float_t> veczloc;
	std::vector<_float_t> vecvloc;
	std::vector<_float_t> vecrloc;

	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	for (_uint_t i=0; i<numberDO; ++i)
	{
		vcDO.push_back(i);
		cpdos  = pDOWorld->GetDOStatus(i);
		cpdoml = pDOWorld->GetDOModel(cpdos->GetDOName());

		if (cpdoml->GetScope() == local)
		{
			vecxloc.push_back(cpdos->GetPosition().x());
			vecyloc.push_back(cpdos->GetPosition().y());
			veczloc.push_back(cpdos->GetPosition().z());
			vecvloc.push_back(cpdos->GetVelocity().length());
			vecrloc.push_back(cpdoml->GetRange());
		}
	}

	if(vecxloc.size() == 0)
	{
		std::cerr
			<< "Error!! Code: bool NBSParallelConsultant::Reset()" << std::endl
			<< "        Note: vecxloc.size() == 0"                 << std::endl;
		exit(-1);
	}

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
	_uint_t  NumOfSphere  = vecxloc.size();
	_float_t SphereXMin   = vecxloc[0];
	_float_t SphereXMax   = vecxloc[NumOfSphere-1];
	_float_t SphereYMin   = vecyloc[0];
	_float_t SphereYMax   = vecyloc[NumOfSphere-1];
	_float_t SphereZMin   = veczloc[0];
	_float_t SphereZMax   = veczloc[NumOfSphere-1];
	_float_t SphereVMax   = vecvloc[NumOfSphere-1];
	_float_t SphereRMax   = vecrloc[NumOfSphere-1];
	_float_t DiscardRatio = 0.02;
	_float_t TempDouble   = 1.0 - DiscardRatio;

	_float_t xSpan = vecxloc[TempDouble*NumOfSphere-1] - vecxloc[DiscardRatio*NumOfSphere];
	_float_t ySpan = vecyloc[TempDouble*NumOfSphere-1] - vecyloc[DiscardRatio*NumOfSphere];
	_float_t zSpan = veczloc[TempDouble*NumOfSphere-1] - veczloc[DiscardRatio*NumOfSphere];

	if (xSpan >= ySpan)
	{
		if (xSpan >= zSpan)
		{
			subReset(TrirLeX(), SphereXMin, SphereXMax, SphereYMin, SphereYMax, SphereZMin, SphereZMax, SphereVMax, SphereRMax);
		}
		else
		{
			subReset(TrirLeZ(), SphereXMin, SphereXMax, SphereYMin, SphereYMax, SphereZMin, SphereZMax, SphereVMax, SphereRMax);
		}
	}
	else
	{
		if (ySpan >= zSpan)
		{
			subReset(TrirLeY(), SphereXMin, SphereXMax, SphereYMin, SphereYMax, SphereZMin, SphereZMax, SphereVMax, SphereRMax);
		}
		else
		{
			subReset(TrirLeZ(), SphereXMin, SphereXMax, SphereYMin, SphereYMax, SphereZMin, SphereZMax, SphereVMax, SphereRMax);
		}
	}
/*
	_float_t xSpan = vecxloc[vecxloc.size()*(1.0-DiscardRatio)-1] - vecxloc[DiscardRatio*vecxloc.size()];
	_float_t ySpan = vecyloc[vecyloc.size()*(1.0-DiscardRatio)-1] - vecyloc[DiscardRatio*vecyloc.size()];
	_float_t zSpan = veczloc[veczloc.size()*(1.0-DiscardRatio)-1] - veczloc[DiscardRatio*veczloc.size()];

	_float_t maxSpan = max(xSpan, max(ySpan,zSpan));

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
}

bool NBSParallelConsultant::CleanUp(DOContainer &cDO, IactContainer &cIact)
{
	SystemParameter* csp = pDOWorld->GetSystemParameter();
	if (csp->GetZoneOfInterest().Active())
	{
		std::vector<_uint_t> RedundantElementGlobal;
		std::vector<_uint_t> RedundantElementLocal;
		std::map<_uint_t, _uint_t> ElementMappingJumpGlobal;

		_uint_t numberDOGlobal = csp->GetDONumber();
		_uint_t numberDOLocal  = vuDOGlobalID.size();

		const DOStatus* pdos  = 0;
		const DOModel*  pdoml = 0;
		_uint_t ul;
		_uint_t ulDead;
		for (ul=0; ul<numberDOLocal; ul++)
		{
			pdos     = pDOWorld->GetDOStatus(GetDO(ul));
			pdoml    = pDOWorld->GetDOModel(pdos->GetDOName());
			DOMap pm = (DOMap(ul, pdos, pdoml, 0.0));
			if (DOMap::ISMobile(pm))
			{
				if (!(InBoundary(GetDO(ul))))
				{
					RedundantElementLocal.push_back(GetDO(ul));
				}
			}
		}

		for (ul=0, ulDead=0; ul<numberDOGlobal; ul++)
		{
			pdos     = pDOWorld->GetDOStatus(ul);
			pdoml    = pDOWorld->GetDOModel(pdos->GetDOName());
			DOMap pm = (DOMap(ul, pdos, pdoml, 0.0));
			if (DOMap::ISMobile(pm))
			{
				if (!(InBoundary(ul)))
				{
					RedundantElementGlobal.push_back(ul);
					ulDead--;
				}
			}
			ElementMappingJumpGlobal[ul] = ulDead;
		}

		if (!RedundantElementGlobal.empty())
		{
			#ifdef _DEBUG
				if (rank == 0)
				{
					std::cout
						<< '[' << rank << "] Global Redundant elements = "
						<< RedundantElementGlobal.size() << " / " << numberDOGlobal << std::endl;
				}
			#endif   // _DEBUG

			if (!RedundantElementLocal.empty())
			{
				#ifdef _DEBUG
					std::cout
						<< '[' << rank << "] Local Redundant elements = "
						<< RedundantElementLocal.size() << " / " << numberDOLocal << std::endl;
				#endif   // _DEBUG

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
}

void NBSParallelConsultant::BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2)
{
	const Boundary* pbc = &(pDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions());

	for (_uint_t i=0; i<v1.size(); ++i)
	{
		for (_uint_t j=0; j<v2.size(); ++j)
    	{
			const IactModel* cpiactml = pDOWorld->GetIactModel(v1[i].cpdoml()->GetDOGroup(), v2[j].cpdoml()->GetDOGroup());

			if (cpiactml == 0)
    	    {
				continue;
        	}

			if ((DOMap::CalDistance(v1[i], v2[j], pbc)) > (v1[i].SafeLength() + v2[j].SafeLength()))
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

	for (_uint_t i=0; i<v.size(); ++i)
    {
		for (_uint_t j=i+1; j<v.size(); ++j)
        {
			const IactModel* cpiactml = pDOWorld->GetIactModel(v[i].cpdoml()->GetDOGroup(), v[j].cpdoml()->GetDOGroup());

			if (cpiactml == 0)
	        {
				continue;
	        }

			if ((DOMap::CalDistance(v[i], v[j], pbc)) > (v[i].SafeLength() + v[j].SafeLength()))
	        {
				continue;
        	}

			if (v[i].id() < v[j].id())
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
	_uint_t avgIact = IactPairTab.size() / NP;

	std::vector< std::pair<_uint_t, _uint_t> > tmpIactVec;
	tmpIactVec.reserve(avgIact + NP);

	if (avgIact == 0)  // Condition: IactPairTab.size() < NP
	{
		if (rank == (NP-1))
		{
			tmpIactVec.assign(IactPairTab.begin(), IactPairTab.end());
		}
	}
	else
	{
		if (rank != (NP-1))  // Processor 0~NP-1
		{
			tmpIactVec.assign(IactPairTab.begin() + rank * avgIact, IactPairTab.begin() + (rank + 1) * avgIact - 1);
		}
		else  // Processor NP
		{
			tmpIactVec.assign(IactPairTab.begin() + rank * avgIact, IactPairTab.end());
		}
	}

	IactPairTab.swap(tmpIactVec);   // For higher efficiency, instead of "IactPairTab = tmpIactVec;"
}

const ImpactStatus* NBSParallelConsultant::RetrieveImpactStatus(_uint_t lcMaster, _uint_t lcSlave) const
{
	return pIRTbl->GetImpactStatus(vuDOGlobalID[lcMaster], vuDOGlobalID[lcSlave]);
}

void NBSParallelConsultant::CollectUserDefinedData(IactContainer& cIact)
{
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	if (uNumUDDImpactStatus != 0)
	{
		// Every processor collect their-owned user-defined data from all "Interaction"s
		cIact.CollectUserDefinedData();

		// All processors collect and sum their-owned user-defined data
		_float_t* dpUserDefinedDataSend = new _float_t[2*uNumUDDImpactStatus];
		for(_uint_t u=0; u<2*uNumUDDImpactStatus; u++)
		{
			*(dpUserDefinedDataSend+u) = cIact.GetUserDefinedValue(u);
		}

		_float_t* dpUserDefinedDataSum = new _float_t(2*uNumUDDImpactStatus);
		for(_uint_t u=0; u<2*uNumUDDImpactStatus; u++)
		{
			*(dpUserDefinedDataSum+u) = 0.0;
		}

		MPI_Allreduce(dpUserDefinedDataSend, dpUserDefinedDataSum, 2*(_int_t)uNumUDDImpactStatus, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

        memcpy(dpUDVIS, dpUserDefinedDataSum, 2*uNumUDDImpactStatus*sizeof(_float_t));
	}
}

void NBSParallelConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	NBSParallelConsultant::CollectUserDefinedData(cIact);

	pIRTbl->Clear();
	const Interaction* pInt = 0;
	for (_uint_t i=0; i<cIact.size(); ++i)
	{
		pInt = cIact.GetInteraction(i);
		if (pInt->IsActive())
		{
			pIRTbl->PushRecord(IactPairTab[i].first, IactPairTab[i].second, *pInt->GetImpactStatus());
		}
	}

	#ifdef _DEBUG
		std::cout << "[" << rank << "] " << "Size of local IactRecordTab = " << pIRTbl->GetTabSize() << std::endl;
	#endif   // _DEBUG

	//this->VecHitRatio.push_back(pIRTbl->GetTabSize()/static_cast<_float_t>(cIact.size()));

	_uint_t tblSize = pIRTbl->GetTabSize();
	_uint_t *numRecords = new _uint_t[NP];

	#ifdef _INT32
		MPI_Allgather(&tblSize, 1, MPI_UNSIGNED, numRecords, 1, MPI_UNSIGNED, MPI_COMM_WORLD);
	#else
		#ifdef _INT64
			MPI_Allgather(&tblSize, 1, MPI_UNSIGNED_LONG_LONG, numRecords, 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#else
			MPI_Allgather(&tblSize, 1, MPI_UNSIGNED_LONG_LONG, numRecords, 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#endif   // _INT64
	#endif   // _INT32

	_int_t *offPair           = new _int_t[NP];
	_int_t *offC_Bond         = new _int_t[NP];
	_int_t *offKn_V_SF_Imp_O  = new _int_t[NP];
	_int_t *offUDV            = new _int_t[NP];

	_int_t *recvPair          = new _int_t[NP];
	_int_t *recvC_Bond        = new _int_t[NP];
	_int_t *recvKn_V_SF_Imp_O = new _int_t[NP];
	_int_t *recvUDV           = new _int_t[NP];

	_uint_t totalSize = 0;
	for (_uint_t u=0; u<NP; ++u)
	{
		totalSize            +=               numRecords[u];
		recvPair         [u]  = 2  * (_int_t) numRecords[u];
		recvC_Bond       [u]  = 4  * (_int_t) numRecords[u];
		recvKn_V_SF_Imp_O[u]  = 18 * (_int_t) numRecords[u];
		recvUDV          [u]  = 4  * (_int_t)(numRecords[u] * uNumUDDImpactStatus);
	}

	offPair         [0] = 0;
	offC_Bond       [0] = 0;
	offKn_V_SF_Imp_O[0] = 0;
	offUDV          [0] = 0;
	for(_uint_t u2=1; u2<NP; ++u2)
    {
		offPair         [u2] = offPair         [u2-1] + 2  * (_int_t) numRecords[u2-1];
		offC_Bond       [u2] = offC_Bond       [u2-1] + 4  * (_int_t) numRecords[u2-1];
		offKn_V_SF_Imp_O[u2] = offKn_V_SF_Imp_O[u2-1] + 18 * (_int_t) numRecords[u2-1];
		offUDV          [u2] = offUDV          [u2-1] + 4  * (_int_t)(numRecords[u2-1] * uNumUDDImpactStatus);
	}

	_uint_t*  p              = new _uint_t [2                        * totalSize];
	_uint_t*  c_bond         = new _uint_t [4                        * totalSize];
	_float_t* kn_v_sf_imp_o  = new _float_t[18                       * totalSize];
	_float_t* udv            = new _float_t[4  * uNumUDDImpactStatus * totalSize];

	_uint_t*  lp             = new _uint_t [2                        * tblSize  ];
	_uint_t*  lc_bond        = new _uint_t [4                        * tblSize  ];
	_float_t* lkn_v_sf_imp_o = new _float_t[18                       * tblSize  ];
	_float_t* ludv           = new _float_t[4  * uNumUDDImpactStatus * tblSize  ];

	std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::const_iterator iter;
	const std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>& m = pIRTbl->GetData();

	_uint_t u = 0;
	vedo::Vector3df vTemp;
	const _float_t* cdpudv;
	for (iter=m.begin(); iter!=m.end(); ++iter)
    {
		lp[2*u  ] = iter->first.first;
		lp[2*u+1] = iter->first.second;

		if (iter->second.Contact())
		{
			lc_bond[4*u] = 1;
		}
		else
		{
			lc_bond[4*u] = 0;
		}

		if (iter->second.LastContact())
		{
			lc_bond[4*u+1] = 1;
		}
		else
		{
			lc_bond[4*u+1] = 0;
		}

		if (iter->second.Bond())
		{
			lc_bond[4*u+2] = 1;
		}
		else
		{
			lc_bond[4*u+2] = 0;
		}

		if (iter->second.LastBond())
		{
			lc_bond[4*u+3] = 1;
		}
		else
		{
			lc_bond[4*u+3] = 0;
		}

		lkn_v_sf_imp_o[18*u   ] = iter->second.Kn();
		lkn_v_sf_imp_o[18*u+1 ] = iter->second.InitialVelocity();
		vTemp                   = iter->second.ShearForce();
		lkn_v_sf_imp_o[18*u+2 ] = vTemp.x();
		lkn_v_sf_imp_o[18*u+3 ] = vTemp.y();
		lkn_v_sf_imp_o[18*u+4 ] = vTemp.z();
		vTemp                   = iter->second.ImpactPoint();
		lkn_v_sf_imp_o[18*u+5 ] = vTemp.x();
		lkn_v_sf_imp_o[18*u+6 ] = vTemp.y();
		lkn_v_sf_imp_o[18*u+7 ] = vTemp.z();
		vTemp                   = iter->second.ImpactDirection();
		lkn_v_sf_imp_o[18*u+8 ] = vTemp.x();
		lkn_v_sf_imp_o[18*u+9 ] = vTemp.y();
		lkn_v_sf_imp_o[18*u+10] = vTemp.z();
		vTemp                   = iter->second.ImpactToMaster();
		lkn_v_sf_imp_o[18*u+11] = vTemp.x();
		lkn_v_sf_imp_o[18*u+12] = vTemp.y();
		lkn_v_sf_imp_o[18*u+13] = vTemp.z();
		vTemp                   = iter->second.AngularImpactToMaster();
		lkn_v_sf_imp_o[18*u+14] = vTemp.x();
		lkn_v_sf_imp_o[18*u+15] = vTemp.y();
		lkn_v_sf_imp_o[18*u+16] = vTemp.z();
		lkn_v_sf_imp_o[18*u+17] = iter->second.Overlap();

		cdpudv = iter->second.RetrieveAllUserDefinedValue();
		for(_uint_t u2=0; u2<4*uNumUDDImpactStatus; u2++)
		{
			ludv[4*uNumUDDImpactStatus*u+u2] = *(cdpudv+u2);
		}

		u++;
	}

	#ifdef _INT32
		MPI_Allgatherv(lp            , 2 *(_int_t)tblSize                      , MPI_UNSIGNED, p            , recvPair         , offPair         , MPI_UNSIGNED, MPI_COMM_WORLD);
		MPI_Allgatherv(lc_bond       , 4 *(_int_t)tblSize                      , MPI_UNSIGNED, c_bond       , recvC_Bond       , offC_Bond       , MPI_UNSIGNED, MPI_COMM_WORLD);
		MPI_Allgatherv(lkn_v_sf_imp_o, 18*(_int_t)tblSize                      , MPI_DOUBLE  , kn_v_sf_imp_o, recvKn_V_SF_Imp_O, offKn_V_SF_Imp_O, MPI_DOUBLE  , MPI_COMM_WORLD);
		MPI_Allgatherv(ludv          , 4 *(_int_t)(uNumUDDImpactStatus*tblSize), MPI_DOUBLE  , udv          , recvUDV          , offUDV          , MPI_DOUBLE  , MPI_COMM_WORLD);
	#else
		#ifdef _INT64
			MPI_Allgatherv(lp            , 2 *(_int_t)tblSize                      , MPI_UNSIGNED_LONG_LONG, p            , recvPair         , offPair         , MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
			MPI_Allgatherv(lc_bond       , 4 *(_int_t)tblSize                      , MPI_UNSIGNED_LONG_LONG, c_bond       , recvC_Bond       , offC_Bond       , MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
			MPI_Allgatherv(lkn_v_sf_imp_o, 18*(_int_t)tblSize                      , MPI_DOUBLE            , kn_v_sf_imp_o, recvKn_V_SF_Imp_O, offKn_V_SF_Imp_O, MPI_DOUBLE            , MPI_COMM_WORLD);
			MPI_Allgatherv(ludv          , 4 *(_int_t)(uNumUDDImpactStatus*tblSize), MPI_DOUBLE            , udv          , recvUDV          , offUDV          , MPI_DOUBLE            , MPI_COMM_WORLD);
		#else
			MPI_Allgatherv(lp            , 2 *(_int_t)tblSize                      , MPI_UNSIGNED_LONG_LONG, p            , recvPair         , offPair         , MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
			MPI_Allgatherv(lc_bond       , 4 *(_int_t)tblSize                      , MPI_UNSIGNED_LONG_LONG, c_bond       , recvC_Bond       , offC_Bond       , MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
			MPI_Allgatherv(lkn_v_sf_imp_o, 18*(_int_t)tblSize                      , MPI_DOUBLE            , kn_v_sf_imp_o, recvKn_V_SF_Imp_O, offKn_V_SF_Imp_O, MPI_DOUBLE            , MPI_COMM_WORLD);
			MPI_Allgatherv(ludv          , 4 *(_int_t)(uNumUDDImpactStatus*tblSize), MPI_DOUBLE            , udv          , recvUDV          , offUDV          , MPI_DOUBLE            , MPI_COMM_WORLD);
		#endif   // _INT64
	#endif   // _INT32

	ImpactStatus s;
	vedo::Vector3df NewVector;
	bool bTemp;
	for (_uint_t u=0; u<NP; ++u)
	{
		if (u == rank)
		{
			continue;
		}

		for (_uint_t i=0; i<numRecords[u]; ++i)
		{
			_uint_t offsetPair          = (_uint_t)offPair[u];
			_uint_t offsetC_Bond        = (_uint_t)offC_Bond[u];
			_uint_t offsetKn_V_SF_Imp_O = (_uint_t)offKn_V_SF_Imp_O[u];
			_uint_t offsetUDV           = (_uint_t)offUDV[u];

			if (c_bond[offsetC_Bond+4*i] == 1)
			{
				bTemp = true;
			}
			else
			{
				bTemp = false;
			}
			s.SetContact(bTemp);

			if (c_bond[offsetC_Bond+4*i+1] == 1)
			{
				bTemp = true;
			}
			else
			{
				bTemp = false;
			}
			s.SetLastContact(bTemp);

			if (c_bond[offsetC_Bond+4*i+2] == 1)
			{
				bTemp = true;
			}
			else
			{
				bTemp = false;
			}
			s.SetBond(bTemp);

			if (c_bond[offsetC_Bond+4*i+3] == 1)
			{
				bTemp = true;
			}
			else
			{
				bTemp = false;
			}
			s.SetLastBond(bTemp);

			s.SetKn(kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i]);
			s.SetInitialVelocity(kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+1]);
			NewVector.set
				(kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+2],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+3],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+4] );
			s.SetShearForce(NewVector);
			NewVector.set
				(kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+5],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+6],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+7] );
			s.SetImpactPoint(NewVector);
			NewVector.set
				(kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+8 ],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+9 ],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+10] );
			s.SetImpactDirection(NewVector);
			NewVector.set
				(kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+11],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+12],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+13] );
			s.SetImpactToMaster(NewVector);
			NewVector.set
				(kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+14],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+15],
				 kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+16] );
			s.SetAngularImpactToMaster(NewVector);
			s.SetOverlap(kn_v_sf_imp_o[offsetKn_V_SF_Imp_O+18*i+17]);
			s.SetAllUserDefinedValue(&udv[offsetUDV+4*uNumUDDImpactStatus*i]);

			pIRTbl->PushRecord(p[offsetPair+2*i], p[offsetPair+2*i+1], s);
		}
	}

	#ifdef _DEBUG
		std::cout << "[" << rank << "] " << "At RebuildIactRecordTab: size = " << pIRTbl->GetTabSize() << std::endl;
	#endif   // _DEBUG

	delete[] numRecords;
	delete[] p;
	delete[] c_bond;
	delete[] kn_v_sf_imp_o;
	delete[] udv;
	delete[] lp;
	delete[] lc_bond;
	delete[] lkn_v_sf_imp_o;
	delete[] ludv;
	delete[] offPair;
	delete[] offC_Bond;
	delete[] offKn_V_SF_Imp_O;
	delete[] offUDV;
	delete[] recvPair;
	delete[] recvC_Bond;
	delete[] recvKn_V_SF_Imp_O;
	delete[] recvUDV;
}

}   // namespace vedo
