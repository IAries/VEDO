#ifndef _INTERACTION_CONTAINER_H
#define _INTERACTION_CONTAINER_H

#include <vedo/Constants.h>
#include <vedo/framework/interfaces/Interaction.h>
#include <vector>

namespace vedo
{

class IactContainer
{

public:

	IactContainer();

	~IactContainer();

	void CalculateImpact(const double dt);

	void CalculateImpact(const double dt, unsigned long ul);

	void CheckContactStatus();

	void Clear();

	void Dump(std::string dumpfile);

	void Add(Interaction* piact);

	inline unsigned long size() const
	{
		return (unsigned long)vcIact.size();
	};

	inline const Interaction* GetInteraction(unsigned long ul)
	{
		return (ul<vcIact.size()) ? vcIact[ul] : 0;
	};

	bool InteractionDetectContact(unsigned long ul);

	void CleanSolverStatus(unsigned long i);

	double GetUserDefinedValue(unsigned u) const;

	inline double* GetUserDefinedValue() const
	{
	    return dpUDVInEachProcessor;
	};

	void CollectUserDefinedData();

private:

	std::vector<Interaction*> vcIact;

	double* dpUDVInEachProcessor;   // 0 ~ uNumUserDefinedData: Accumulative user-defined value
	//double dUDVInEachProcessor[2*uNumUDDImpactStatus];   // 0 ~ uNumUserDefinedData: Accumulative user-defined value
                                                         // uNumUserDefinedData ~ 2*uNumUserDefinedData-1: User-defined value
};

};   // namespace vedo

#endif // _INTERACTION_CONTAINER_H
