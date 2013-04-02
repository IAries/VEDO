#ifndef _IACT_RECORD_TAB_H
#define _IACT_RECORD_TAB_H

#include <Framework/Interfaces/ImpactSolver.h>
#include <Framework/Interfaces/ImpactStatus.h>
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

	// I/O function

	bool ReadIRT2010(const char* filename);

	bool ReadIRT2011(const char* filename);

	//bool ReadTextIRT(const char* filename);

	void WriteCSV (std::string filename) const;

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

	//binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	//binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus> mapImStatus;
};

};   // namespace vedo

#endif // _IACT_RECORD_TAB_H
