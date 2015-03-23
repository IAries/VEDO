#ifndef _INTERACTION_CONTAINER_H
#define _INTERACTION_CONTAINER_H

#include <vedo/framework/Interaction.h>
#include <vedo/framework/DataType.h>
#include <vector>

namespace vedo
{

class IactContainer
{

public:

	IactContainer();

	~IactContainer();

	void CalculateImpact(const _float_t dt);

	void CalculateImpact(const _float_t dt, _uint_t ul);

	void CheckContactStatus();

	void Clear();

	void Dump(std::string dumpfile);

	void Add(Interaction* piact);

	inline _uint_t size() const
	{
		return (_uint_t)vcIact.size();
	}

	inline const Interaction* GetInteraction(_uint_t ul)
	{
		return (ul<vcIact.size()) ? vcIact[ul] : 0;
	}

	bool InteractionDetectContact(_uint_t ul);

	void BackupImpactStatus();

	void CleanSolverStatus(_uint_t i);

	_float_t GetUserDefinedValue(_uint_t u) const;

	inline _float_t* GetUserDefinedValue() const
	{
	    return dpUDVInEachProcessor;
	}

	void CollectUserDefinedData();

private:

	std::vector<Interaction*> vcIact;

	_float_t* dpUDVInEachProcessor;   // 0 ~ uNumUserDefinedData: Accumulative user-defined value
	//_float_t dUDVInEachProcessor[2*uNumUDDImpactStatus];   // 0 ~ uNumUserDefinedData: Accumulative user-defined value
                                                         // uNumUserDefinedData ~ 2*uNumUserDefinedData-1: User-defined value
};

}   // namespace vedo

#endif // _INTERACTION_CONTAINER_H
