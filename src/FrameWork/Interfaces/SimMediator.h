#ifndef _SIMULATEMEDIATOR_H
#define _SIMULATEMEDIATOR_H

#include <FrameWork/Interfaces/Assembler.h>
#include <FrameWork/Interfaces/Consultant.h>
#include <FrameWork/Interfaces/DOContainer.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <FrameWork/Interfaces/IactContainer.h>
#include <ctime>
#include <string>
#include <fstream>

namespace VEDO
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
	virtual bool Run(const std::vector<std::pair<NJR::Vector3d, NJR::Vector3d> >&);
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

	void WriteInteractionForce(const char* filename, const std::vector<std::pair<NJR::Vector3d, NJR::Vector3d> >* extImpact=0);

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
	unsigned int     rank;            // Rank of processores
	unsigned int     NP;              // Number of processores
	std::ofstream    FileLog;         // Log file
	std::ofstream    FileLogEnergy;   // Energy log file
};

};   // namespace VEDO

#endif // _SIMULATEMEDIATOR_H
