#ifndef _IACT_RECORD_TAB_H
#define _IACT_RECORD_TAB_H

#include <vedo/framework/interfaces/ImpactSolver.h>
#include <vedo/framework/interfaces/ImpactStatus.h>
#include <map>
#include <vector>

namespace vedo
{

class IactRecordTab
{

public:

	IactRecordTab();

	IactRecordTab
		(const std::map<std::pair<unsigned long, unsigned long>,
		ImpactStatus>& m);

	inline unsigned long GetTabSize() const
	{
		return mapImStatus.size();
	}

	inline void Clear()
	{
		mapImStatus.clear();
	}

	void print() const;

	inline const std::map< std::pair<unsigned long, unsigned long>, ImpactStatus >&
		GetData() const
	{
		return mapImStatus;
	}

	const ImpactStatus* GetImpactStatus
		(unsigned long master, unsigned long slave);

	void CleanUserDefinedValueInImpactStatus();

	void PushRecord
		(unsigned long master, unsigned long slave, const ImpactStatus& s);

	void ModifyPair(std::map<unsigned long, long> mNewPairList);

	unsigned long ContactNumber() const;

	void EraseElement(const unsigned long& ulID);

	void EraseElements(const std::vector<unsigned long>& ulIDList, const unsigned long& ulDONum);

    void DumpIactStatus(const char* filename) const;

	//binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	//binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus> mapImStatus;
};

};   // namespace vedo

#endif // _IACT_RECORD_TAB_H
