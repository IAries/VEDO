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

	IactRecordTab(const std::map<std::pair<vedo::vedo_uint_t, vedo::vedo_uint_t>, ImpactStatus>& m);

	inline vedo::vedo_uint_t GetTabSize() const
	{
		return mapImStatus.size();
	}

	inline void Clear()
	{
		mapImStatus.clear();
	}

	void print() const;

	inline const std::map< std::pair<vedo::vedo_uint_t, vedo::vedo_uint_t>, ImpactStatus >& GetData() const
	{
		return mapImStatus;
	}

	const ImpactStatus* GetImpactStatus(vedo::vedo_uint_t master, vedo::vedo_uint_t slave);

	void CleanUserDefinedValueInImpactStatus();

	void PushRecord(vedo::vedo_uint_t master, vedo::vedo_uint_t slave, const ImpactStatus& s);

	void ModifyPair(std::map<vedo::vedo_uint_t, vedo::vedo_int_t> mNewPairList);

	vedo::vedo_uint_t ContactNumber() const;

	vedo::vedo_uint_t BondNumber() const;

	void EraseElement(const vedo::vedo_uint_t& ulID);

	void EraseElements(const std::vector<vedo::vedo_uint_t>& ulIDList, const vedo::vedo_uint_t& ulDONum);

    void DumpIactStatus(const vedo_float_t& time, const char* filename) const;

	//binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	//binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	std::map<std::pair<vedo::vedo_uint_t, vedo::vedo_uint_t>, ImpactStatus> mapImStatus;
};

}   // namespace vedo

#endif // _IACT_RECORD_TAB_H
