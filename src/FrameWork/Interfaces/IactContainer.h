#ifndef _INTERACTION_CONTAINER_H
#define _INTERACTION_CONTAINER_H

#include <FrameWork/Interfaces/Constants.h>
#include <FrameWork/Interfaces/Interaction.h>
#include <vector>

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

	void CollectUserDefinedData();

private:

	std::vector<Interaction*> vcIact;

	double dUDVInEachProcessor[uNumUserDefinedData*2];   // 0 ~ uNumUserDefinedData: Accumulative user-defined value
									                     // uNumUserDefinedData ~ 2*uNumUserDefinedData-1: User-defined value
};

#endif // _INTERACTION_CONTAINER_H
