#ifndef _IACT_RECORD_TAB_H
#define _IACT_RECORD_TAB_H

#include <FrameWork/Interfaces/ImpactSolver.h>
#include <FrameWork/Interfaces/ImpactStatus.h>
#include <map>
#include <vector>

namespace VEDO
{

class IactRecordTab
{

public:

	IactRecordTab();

	IactRecordTab(const char* filename);

	IactRecordTab
		(const std::map<std::pair<unsigned long, unsigned long>,
		ImpactStatus>& m);

	// I/O function

	bool ReadIRT(const char* filename);

	bool ReadIRT2010(const char* filename);

	bool ReadIRT2011(const char* filename);

	void WriteIRT(const char* filename) const;

	//bool ReadTextIRT(const char* filename);

	void WriteTextIRT (const char* filename) const;

	inline unsigned long int GetTabSize() const
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

private:

	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus> mapImStatus;
};

};   // namespace VEDO

#endif // _IACT_RECORD_TAB_H
