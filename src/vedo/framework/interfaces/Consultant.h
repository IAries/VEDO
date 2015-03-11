#ifndef _CONSULTANT_H
#define _CONSULTANT_H

#include <vedo/constants/interfaces/Constants.h>
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
void EnsureLength(vedo_uint_t base, vedo_uint_t target, vedo_uint_t& length, vedo_float_t*& array);



typedef std::pair<vedo_uint_t, vedo_uint_t> IactPair;

class X_Comp
{

public:

	X_Comp(const std::vector<DOMap>&);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	vedo_float_t CalcIactCoordinate(const IactPair&);

};



class Y_Comp
{

public:

	Y_Comp(const std::vector<DOMap>& vDOMap);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	vedo_float_t CalcIactCoordinate(const IactPair&);

};



class Z_Comp
{

public:

	Z_Comp(const std::vector<DOMap>& vDOMap);

	bool operator() (const IactPair&, const IactPair&);

private:

	std::vector<DOMap> map;

	vedo_float_t CalcIactCoordinate(const IactPair&);

};



class Consultant
{

public:

	Consultant(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, vedo_uint_t ulwrite);

	// Retrieves the number of active Discrete Objects
	virtual vedo_uint_t GetDONum() const;

	// Retrieves the number of active Interactions
	virtual vedo_uint_t GetIactNum() const;

	// Retrieves the ith active Discrete Objects
	virtual vedo_uint_t GetDO(vedo_uint_t i) const;

	// Retrieves the Master Discrete Object of the ith active Interaction
	virtual vedo_uint_t GetIactMaster(vedo_uint_t i) const;

	// Retrieves the slave Discrete Object of the ith active Interaction
	virtual vedo_uint_t GetIactSlave(vedo_uint_t i) const;

	// Retireves the current Simulation Problem
	virtual const DOWorld* GetDOWorld() const;

	virtual const IactRecordTab* GetIactRecordTab() const;

	inline vedo_uint_t ContactNumber() const
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

	virtual const ImpactStatus* RetrieveImpactStatus(vedo_uint_t lcMaster, vedo_uint_t lcSlave) const;

	virtual ~Consultant();

	inline void SetRankNP(const vedo_uint_t r, const vedo_uint_t np)
	{
		rank = r;
		NP   = np;
	}

	inline void ResetTimePartitioning()
	{
		timePartitioning = 0.0;
	}

	bool InBoundary(vedo_uint_t) const;

	virtual bool CleanUp(DOContainer& cDO, IactContainer& cIact);

	virtual void SyncWorld(DOContainer& vDO);

	virtual void RebuildIactRecordTab(IactContainer& cIact);

	std::time_t  starttime;          // Starting time

	std::time_t  endtime;            // Endind time

	vedo_float_t timePartitioning;   // Time of partitioning

	inline void FreezeAllElements()
	{
		pDOWorld->FreezeAllElements();
	}

	bool EraseSingleElement(const vedo_uint_t& ulID);

	bool EraseElements(const std::vector<vedo_uint_t>& ulIDList);

	inline void CalculateSystemEnergy()
	{
		pDOWorld->CalculateSystemEnergy();
	}

	vedo_float_t GetUserDefinedValue(vedo_uint_t) const;

protected:

	DOWorld*                 pDOWorld;

	IactRecordTab*           pIRTbl;

	std::vector<vedo_uint_t> vcDO;

	std::vector<vedo_uint_t> vcIactMaster;

	std::vector<vedo_uint_t> vcIactSlave;

	const vedo_uint_t        culRecord;

	vedo_uint_t              culUpIact;

	vedo_uint_t              ulRecordCount;

	vedo_uint_t              ulRoundCount;

	std::string              sfilename;

	vedo_uint_t              rank;   // Rank   of processores

	vedo_uint_t              NP;     // Number of processores

	vedo_uint_t              ImpactBufferSize;

	std::vector<IactPair>    IactPairTab;

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);

	vedo_float_t* dpUDVIS;   // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value in ImpactStatus
	//vedo_float_t dUDVIS[2*uNumUDDImpactStatus];   // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value in ImpactStatus
                                            // uNumUDDImpactStatus ~ 2*uNumUDDImpactStatus-1: User-defined value in ImpactStatus

//	vedo_float_t dUDVDS[2*uNumUDDDOStatus];       // 0 ~ uNumUDDDOStatus: Accumulative user-defined value in DOStatus
                                            // uNumUDDDOStatus ~ 2*uNumUDDImpactStatus-1: User-defined value in DOStatus

	virtual void CollectUserDefinedData(IactContainer&);
};

}   // namespace vedo

#endif // _CONSULTANT_H
