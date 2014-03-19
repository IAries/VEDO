#ifndef _CONSULTANT_H
#define _CONSULTANT_H

#include <vedo/Constants.h>
#include <vedo/framework/interfaces/DOContainer.h>
#include <vedo/framework/interfaces/DOMap.h>
#include <vedo/framework/interfaces/DOModel.h>
#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/IactContainer.h>
#include <vedo/framework/interfaces/IactRecordTab.h>
#include <ctime>
#include <vector>

namespace vedo
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
	virtual bool ISReset();

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

	virtual bool Reset();

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

	virtual bool CleanUp(DOContainer& cDO, IactContainer& cIact);

	virtual void SyncWorld(DOContainer& vDO);

	virtual void RebuildIactRecordTab(IactContainer& cIact);

	std::time_t starttime;          // Starting time
	std::time_t endtime;            // Endind time
	double      timePartitioning;   // Time of partitioning

	inline void FreezeAllElements()
	{
		pDOWorld->FreezeAllElements();
	};

	bool EraseSingleElement(const unsigned long& ulID);

	bool EraseElements(const std::vector<unsigned long>& ulIDList);

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

	double* dpUDVIS;   // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value in ImpactStatus
	//double dUDVIS[2*uNumUDDImpactStatus];   // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value in ImpactStatus
                                            // uNumUDDImpactStatus ~ 2*uNumUDDImpactStatus-1: User-defined value in ImpactStatus

//	double dUDVDS[2*uNumUDDDOStatus];       // 0 ~ uNumUDDDOStatus: Accumulative user-defined value in DOStatus
                                            // uNumUDDDOStatus ~ 2*uNumUDDImpactStatus-1: User-defined value in DOStatus

	virtual void CollectUserDefinedData(IactContainer&);
};

};   // namespace vedo

#endif // _CONSULTANT_H
