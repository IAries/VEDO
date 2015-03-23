#ifndef _IACT_RECORD_TAB_H
#define _IACT_RECORD_TAB_H

#include <vedo/framework/ImpactSolver.h>
#include <vedo/framework/ImpactStatus.h>
#include <vedo/framework/DataType.h>
#include <map>
#include <vector>

namespace vedo
{

class IactRecordTab
{

public:

	IactRecordTab();

	IactRecordTab(const std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>& m);

	inline _uint_t GetTabSize() const
	{
		return mapImStatus.size();
	}

	inline void Clear()
	{
		mapImStatus.clear();
	}

	void print() const;

	inline const std::map< std::pair<_uint_t, _uint_t>, ImpactStatus >& GetData() const
	{
		return mapImStatus;
	}

	const ImpactStatus* GetImpactStatus(_uint_t master, _uint_t slave);

	void CleanUserDefinedValueInImpactStatus();

	void PushRecord(_uint_t master, _uint_t slave, const ImpactStatus& s);

	void ModifyPair(std::map<_uint_t, _uint_t> mNewPairList);

	_uint_t ContactNumber() const;

	_uint_t BondNumber() const;

	void EraseElement(const _uint_t& ulID);

	void EraseElements(const std::vector<_uint_t>& ulIDList, const _uint_t& ulDONum);

    void DumpIactStatus(const _float_t& time, const char* filename) const;

	//binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	//binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	std::map<std::pair<_uint_t, _uint_t>, ImpactStatus> mapImStatus;
};

}   // namespace vedo

#endif // _IACT_RECORD_TAB_H
