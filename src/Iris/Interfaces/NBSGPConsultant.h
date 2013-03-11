#ifndef _NBSGP_CONSULTANT_H
#define _NBSGP_CONSULTANT_H

#include <FrameWork/Interfaces/Consultant.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <cmath>
#include <ctime>
#include <iterator>
#include <limits>
#include <map>
#include <utility>
#include <vector>

class NBSGPConsultant : public Consultant
{

public:

	NBSGPConsultant
		(DOWorld* DOWorld,
		IactRecordTab* pIactRecordTab,
		char filename[],
		unsigned long ulwrite,
		unsigned long ulupdate        );

	~NBSGPConsultant();

	virtual unsigned long GetDONum() const;

	virtual unsigned long GetIactNum() const;

	virtual unsigned long GetDO(unsigned long i) const;

	virtual unsigned long GetIactMaster (unsigned long i) const;

	virtual unsigned long GetIactSlave (unsigned long i) const;

	virtual void SyncDOContainer(DOContainer& cDO);

	void WriteDOSubWorld(DOContainer&) const ;

	inline bool ISReset()
	{
		return ((ulRoundCount % culUpIact) == 0);
	}

	virtual bool Reset();

	virtual void CleanUp(DOContainer& cDO, IactContainer& cIact);

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	inline int getRank()
	{
		return rank;
	};

	const ImpactStatus* RetrieveImpactStatus
		(unsigned long lcMaster, unsigned long lcSlave) const;

	static const int MASTER = 0;

	int MAX_CUT;

	double MAX_ALLOWED_UNB;

	int TIMES_MAX_CUT_EXCEEDED;

	virtual void SyncWorld(DOContainer& vDO);

//	void MasterSyncWorld(DOContainer& vDO, IactContainer& cIact);

//	void SlaveSyncWorld(DOContainer& vDO, IactContainer& cIact);

	virtual void RebuildIactRecordTab(IactContainer& cIact);

protected:

	virtual void CollectUserDefinedData(IactContainer&);

private:

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);

	void ConstructDOandOverLapTab();

	void GraphPartitionIactPair();

	// The global number of "Discrete Element (Object)"
	std::vector<unsigned long> DOTab;

	std::vector< std::vector<unsigned long> > overlapTab;

	std::vector<unsigned long> G2LTab;

	//MPIAssistant assistant;

	// The space for transfer Impact
	mutable std::vector< std::pair<unsigned long, double*> > sendBufVec;
	mutable std::vector< std::pair<unsigned long, double*> > recvBufVec;

	// The space for synchorize the world
	mutable std::vector< std::pair< unsigned long, std::pair<unsigned long*, double*> > > syncSendBufVec;
	mutable std::vector< std::pair< unsigned long, std::pair<unsigned long*, double*> > > syncRecvBufVec;
};

#endif // _NBSGP_CONSULTANT_H
