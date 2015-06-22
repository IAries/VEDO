#ifndef _CONSULTANT_H
#define _CONSULTANT_H

#include <vedo/framework/DOContainer.h>
#include <vedo/framework/DOMap.h>
#include <vedo/framework/DOModel.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/IactContainer.h>
#include <vedo/framework/IactRecordTab.h>
#include <vedo/framework/DataType.h>
#include <aries/utility/Constants.h>
#include <ctime>
#include <vector>

namespace vedo
{

// The EnsureLength function
void EnsureLength(_uint_t base, _uint_t target, _uint_t& length, _float_t*& array);



typedef std::pair<_uint_t, _uint_t> IactPair;

class X_Comp
{

public:

	X_Comp(const std::vector<DOMap>&);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	_float_t CalcIactCoordinate(const IactPair&);

};



class Y_Comp
{

public:

	Y_Comp(const std::vector<DOMap>& vDOMap);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	_float_t CalcIactCoordinate(const IactPair&);

};



class Z_Comp
{

public:

	Z_Comp(const std::vector<DOMap>& vDOMap);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	_float_t CalcIactCoordinate(const IactPair&);

};



class Consultant
{

public:

	Consultant(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, _uint_t ulwrite);

	// Retrieves the number of active Discrete Objects
	virtual _uint_t GetDONum() const;

	// Retrieves the number of active Interactions
	virtual _uint_t GetIactNum() const;

	// Retrieves the ith active Discrete Objects
	virtual _uint_t GetDO(_uint_t i) const;

	// Retrieves the Master Discrete Object of the ith active Interaction
	virtual _uint_t GetIactMaster(_uint_t i) const;

	// Retrieves the slave Discrete Object of the ith active Interaction
	virtual _uint_t GetIactSlave(_uint_t i) const;

	// Retireves the current Simulation Problem
	virtual const DOWorld* GetDOWorld() const;

	virtual const IactRecordTab* GetIactRecordTab() const;

	inline _uint_t ContactNumber() const
	{
		return pIRTbl->ContactNumber();
	}

	// Synchronize the external impact of each Discrete Object inside this DOContainer;
	virtual void SyncDOContainer(DOContainer& cDO);

	// DO all kernel instances need to be rebuilt?
	virtual bool ISReset();

	// Do all kernel need to record the data?
	inline bool ISRecord()
	{
		return ((ulRoundCount % culRecord) == 0);
	}

	// Updates the data of Consultant and updates DOWorld and then make DOWorld include the last status of each Discrete Objects
	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	virtual bool Reset();

	virtual void RecordIDO();

	virtual const ImpactStatus* RetrieveImpactStatus(_uint_t lcMaster, _uint_t lcSlave) const;

	virtual ~Consultant();

	inline void SetRankNP(const _uint_t r, const _uint_t np)
	{
		rank = r;
		NP   = np;
	}

	inline void ResetTimePartitioning()
	{
		timePartitioning = 0.0;
	}

	bool InBoundary(_uint_t) const;

	virtual bool CleanUp(DOContainer& cDO, IactContainer& cIact);

	virtual void SyncWorld(DOContainer& vDO);

	virtual void RebuildIactRecordTab(IactContainer& cIact);

	std::time_t  starttime;          // Starting time

	std::time_t  endtime;            // Endind time

	_float_t timePartitioning;   // Time of partitioning

	inline void FreezeAllElements()
	{
		pDOWorld->FreezeAllElements();
	}

	bool EraseSingleElement(const _uint_t& ulID);

	bool EraseElements(const std::vector<_uint_t>& ulIDList);

	inline void CalculateSystemEnergy()
	{
		pDOWorld->CalculateSystemEnergy();
	}

	_float_t GetUserDefinedValue(_uint_t) const;

protected:

	DOWorld*                 pDOWorld;

	IactRecordTab*           pIRTbl;

	std::vector<_uint_t>     vcDO;           // All discrete objects (even in parallel computing)

	std::vector<_uint_t>     vcIactMaster;   // All (master) interactions (even in parallel computing)

	std::vector<_uint_t>     vcIactSlave;    // All (slave) interactions (even in parallel computing)

	std::vector<IactPair>    IactPairTab;    // Local interactions in each computing unit

	const _uint_t            culRecord;

	_uint_t                  culUpIact;

	_uint_t                  ulRecordCount;

	_uint_t                  ulRoundCount;

	std::string              sfilename;

	_uint_t                  rank;           // Rank of computing unit

	_uint_t                  NP;             // Number of toal computing units

	_uint_t                  ImpactBufferSize;

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);

	_float_t* dpUDVIS;   // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value in ImpactStatus
	//_float_t dUDVIS[2*uNumUDDImpactStatus];   // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value in ImpactStatus
                                            // uNumUDDImpactStatus ~ 2*uNumUDDImpactStatus-1: User-defined value in ImpactStatus

//	_float_t dUDVDS[2*uNumUDDDOStatus];       // 0 ~ uNumUDDDOStatus: Accumulative user-defined value in DOStatus
                                            // uNumUDDDOStatus ~ 2*uNumUDDImpactStatus-1: User-defined value in DOStatus

	virtual void CollectUserDefinedData(IactContainer&);
};

}   // namespace vedo

#endif // _CONSULTANT_H
