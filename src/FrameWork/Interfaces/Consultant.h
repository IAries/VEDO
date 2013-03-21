/******************************************************************************
 * Consultant is responsible for deciding
 * 1. which one DiscreteObject has to be created
 * 2. which one Interaction has to be created
 * 3. Synchronize All external impact of each Discrete Object
 *
 * History: Ver 1.0 Created 2003 Dec by C.T. Jitin Yang
 ******************************************************************************/

#ifndef _CONSULTANT_H
#define _CONSULTANT_H

#include <FrameWork/Interfaces/Constants.h>
#include <FrameWork/Interfaces/DOContainer.h>
#include <FrameWork/Interfaces/DOMap.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <FrameWork/Interfaces/IactContainer.h>
#include <FrameWork/Interfaces/IactRecordTab.h>
#include <ctime>
#include <vector>

namespace VEDO
{

// The EnsureLength function
void EnsureLength
	(unsigned int base,
	 unsigned long target,
	 unsigned long& length,
	 double*& array        );



typedef std::pair<unsigned long, unsigned long> IactPair;

class X_Comp
{
public:

	X_Comp(const std::vector<DOMap>&);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	double CalcIactCoordinate(const IactPair&);
};



class Y_Comp
{
public:

	Y_Comp(const std::vector<DOMap>& vDOMap);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	double CalcIactCoordinate(const IactPair&);
};



class Z_Comp
{
public:

	Z_Comp(const std::vector<DOMap>& vDOMap);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	double CalcIactCoordinate(const IactPair&);
};



class Consultant
{

public:

	Consultant
		(DOWorld* DOWorld,
		IactRecordTab* pIactRecordTab,
		char filename[],
		unsigned long ulwrite);

	// Retrieves the number of active Discrete Objects
	virtual unsigned long GetDONum() const;

	// Retrieves the number of active Interactions
	virtual unsigned long GetIactNum() const;

	// Retrieves the ith active Discrete Objects
	virtual unsigned long GetDO(unsigned long i) const;

	// Retrieves the Master Discrete Object of the ith active Interaction
	virtual unsigned long GetIactMaster(unsigned long i) const;

	// Retrieves the slave Discrete Object of the ith active Interaction
	virtual unsigned long GetIactSlave(unsigned long i) const;

	// Retireves the current Simulation Problem
	virtual const DOWorld* GetDOWorld() const;

	virtual const IactRecordTab* GetIactRecordTab() const;

	inline unsigned long ContactNumber() const
	{
		return pIRTbl->ContactNumber();
	};

	/**************************************************************************
	 * Synchronize the external impact of each Discrete Object inside this
	 * DOContainer;
	 **************************************************************************/
	virtual void SyncDOContainer(DOContainer& cDO);

	// DO all kernel instances need to be rebuilt?
	virtual bool ISReset() = 0;

	// Do all kernel need to record the data?
	inline bool ISRecord()
	{
		return ((ulRoundCount % culRecord) == 0);
	};

	/**************************************************************************
	 * Updates the data of Consultant and updates DOWorld and then make DOWorld
	 * include the last status of each Discrete Objects
	 **************************************************************************/
	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	virtual bool Reset() = 0;

	virtual void RecordIDO();

	virtual const ImpactStatus* RetrieveImpactStatus
		(unsigned long lcMaster, unsigned long lcSlave) const;

	virtual ~Consultant();

	inline void SetRankNP(const int r, const int np)
	{
		rank = r;
		NP   = np;
	};

	inline void ResetTimePartitioning()
	{
		timePartitioning = 0.0;
	};

	bool InBoundary(unsigned long) const;

	virtual void CleanUp(DOContainer& cDO, IactContainer& cIact);

	virtual void SyncWorld(DOContainer& vDO);

	virtual void RebuildIactRecordTab(IactContainer& cIact) = 0;

	std::time_t starttime;          // Starting time
	std::time_t endtime;            // Endind time
	double      timePartitioning;   // Time of partitioning

	inline void FreezeAllElements()
	{
		pDOWorld->FreezeAllElements();
	};

	inline void CalculateSystemEnergy()
	{
		pDOWorld->CalculateSystemEnergy();
	};

	double GetUserDefinedValue(unsigned) const;

protected:

	DOWorld*                   pDOWorld;
	IactRecordTab*             pIRTbl;
	std::vector<unsigned long> vcDO;
	std::vector<unsigned long> vcIactMaster;
	std::vector<unsigned long> vcIactSlave;

	const unsigned long        culRecord;
	unsigned long              culUpIact;
	unsigned long              ulRecordCount;
	unsigned long              ulRoundCount;

	std::string                sfilename;

	unsigned int               rank;   // Rank   of processores
	unsigned int               NP;     // Number of processores

	unsigned long              ImpactBufferSize;

	std::vector<IactPair>      IactPairTab;

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);
	virtual void BuildIactTab(std::vector<DOMap>& v);

	double dUDV[2*VEDO::uNumUserDefinedData];   // 0 ~ uNumUserDefinedData: Accumulative user-defined value
                                                     // uNumUserDefinedData ~ 2*uNumUserDefinedData-1: User-defined value

	virtual void CollectUserDefinedData(IactContainer&);
};

};   // namespace VEDO

#endif // _CONSULTANT_H
