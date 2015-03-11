#ifndef _INTERACTION_CONTAINER_H
#define _INTERACTION_CONTAINER_H

#include <vedo/constants/interfaces/Constants.h>
#include <vedo/framework/interfaces/Interaction.h>
#include <vector>

namespace vedo
{

class IactContainer
{

public:

	IactContainer();

	~IactContainer();

	void CalculateImpact(const vedo_float_t dt);

	void CalculateImpact(const vedo_float_t dt, vedo::vedo_uint_t ul);

	void CheckContactStatus();

	void Clear();

	void Dump(std::string dumpfile);

	void Add(Interaction* piact);

	inline vedo::vedo_uint_t size() const
	{
		return (vedo::vedo_uint_t)vcIact.size();
	}

	inline const Interaction* GetInteraction(vedo::vedo_uint_t ul)
	{
		return (ul<vcIact.size()) ? vcIact[ul] : 0;
	}

	bool InteractionDetectContact(vedo::vedo_uint_t ul);

	void BackupImpactStatus();

	void CleanSolverStatus(vedo::vedo_uint_t i);

	vedo_float_t GetUserDefinedValue(vedo_uint_t u) const;

	inline vedo_float_t* GetUserDefinedValue() const
	{
	    return dpUDVInEachProcessor;
	}

	void CollectUserDefinedData();

private:

	std::vector<Interaction*> vcIact;

	vedo_float_t* dpUDVInEachProcessor;   // 0 ~ uNumUserDefinedData: Accumulative user-defined value
	//vedo_float_t dUDVInEachProcessor[2*uNumUDDImpactStatus];   // 0 ~ uNumUserDefinedData: Accumulative user-defined value
                                                         // uNumUserDefinedData ~ 2*uNumUserDefinedData-1: User-defined value
};

}   // namespace vedo

#endif // _INTERACTION_CONTAINER_H
