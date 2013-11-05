#ifndef _SIMULATEMEDIATOR_H
#define _SIMULATEMEDIATOR_H

#include <Framework/Interfaces/Assembler.h>
#include <Framework/Interfaces/Consultant.h>
#include <Framework/Interfaces/DOContainer.h>
#include <Framework/Interfaces/DOWorld.h>
#include <Framework/Interfaces/IactContainer.h>
#include <ctime>
#include <string>
#include <fstream>

namespace vedo
{

class SimMediator
{

public:

	SimMediator(Consultant* Consultant, const Assembler* Assem);
	SimMediator
		(Consultant* Consultant,
		const Assembler* Assem,
		const unsigned int,
		const unsigned int);
	virtual ~SimMediator();
	virtual bool Run();
	virtual bool Run(const std::vector<std::pair<njr::Vector3d, njr::Vector3d> >&);
	virtual bool ReDistribute();
	virtual void Initiate();
	virtual void TimeInitiate();

	inline const DOContainer* GetDOContainer() const
	{
		return &cDO;
	};

	inline const IactContainer* GetIactContainer() const
	{
		return &cIact;
	};

	inline const Consultant* GetConsultant() const
	{
		return pConsultant;
	};

	inline const Assembler* GetAssembler() const
	{
		return cpAssembler;
	};

	void ShowInteraction();

	void WriteInteractionForce(const char* filename, const std::vector<std::pair<njr::Vector3d, njr::Vector3d> >* extImpact=0);

	void CalculateSystemEnergy();

	std::time_t starttime;              // Starting time
	std::time_t endtime;                // Endind time
	double      timeSystem;             // Time of system preparing, starting and ending
	double      timeImpactSolving;      // Time of impact solving
	double      timeFieldForceAdding;   // Time of field force adding
	double      timeResponseUpdating;   // Time of response updating
	double      timeContactDetection;   // Time of contact detection
	double      timeNextStep;           // Time of "NextStep"
	double      timeSyncDOContainer;    // Time of "SyncDOContainer"
	double      timePartitioning;       // Time of partitioning
	double      timeComputing;          // Time of computing
	double      timeCommunication;      // Time of communication
	double      timeTotal;              // Total time

private:

	Consultant*      pConsultant;
	const Assembler* cpAssembler;
	DOContainer      cDO;
	IactContainer    cIact;
	unsigned int     rank;                  // Rank of processores
	unsigned int     NP;                    // Number of processores
	std::ofstream    FileLog;               // Log file
	std::ofstream    FileLogSystemStatus;   // Log file of system status
};

};   // namespace vedo

#endif // _SIMULATEMEDIATOR_H
