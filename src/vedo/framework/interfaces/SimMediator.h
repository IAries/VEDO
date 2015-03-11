#ifndef _SIMULATEMEDIATOR_H
#define _SIMULATEMEDIATOR_H

#include <vedo/framework/interfaces/Assembler.h>
#include <vedo/framework/interfaces/Consultant.h>
#include <vedo/framework/interfaces/DOContainer.h>
#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/IactContainer.h>
#include <ctime>
#include <string>
#include <fstream>

namespace vedo
{

class SimMediator
{

public:

	SimMediator(Consultant* Consultant, const Assembler* Assem);

	SimMediator(Consultant* Consultant, const Assembler* Assem, const vedo_uint_t, const vedo_uint_t);

	virtual ~SimMediator();

	virtual bool Run();

	virtual bool Run(const std::vector<std::pair<njr::Vector3d, njr::Vector3d> >&);

	virtual bool ReDistribute();

	virtual void Initiate();

	virtual void TimeInitiate();

	inline const DOContainer* GetDOContainer() const
	{
		return &cDO;
	}

	inline const IactContainer* GetIactContainer() const
	{
		return &cIact;
	}

	inline const Consultant* GetConsultant() const
	{
		return pConsultant;
	}

	inline const Assembler* GetAssembler() const
	{
		return cpAssembler;
	}

//	void ShowInteraction();

	void WriteInteractionForce(const char* filename, const std::vector<std::pair<njr::Vector3d, njr::Vector3d> >* extImpact=0);

	void CalculateSystemEnergy();

	std::time_t  starttime;              // Starting time

	std::time_t  endtime;                // Endind time

	vedo_float_t timeSystem;             // Time of system preparing, starting and ending

	vedo_float_t timeImpactSolving;      // Time of impact solving

	vedo_float_t timeFieldForceAdding;   // Time of field force adding

	vedo_float_t timeResponseUpdating;   // Time of response updating

	vedo_float_t timeContactDetection;   // Time of contact detection

	vedo_float_t timeNextStep;           // Time of "NextStep"

	vedo_float_t timeSyncDOContainer;    // Time of "SyncDOContainer"

	vedo_float_t timePartitioning;       // Time of partitioning

	vedo_float_t timeComputing;          // Time of computing

	vedo_float_t timeCommunication;      // Time of communication

	vedo_float_t timeTotal;              // Total time

private:

	Consultant*      pConsultant;

	const Assembler* cpAssembler;

	DOContainer      cDO;

	IactContainer    cIact;

	vedo_uint_t      rank;                  // Rank of processores

	vedo_uint_t      NP;                    // Number of processores

	std::ofstream    FileLog;               // Log file

	std::ofstream    FileLogSystemStatus;   // Log file of system status

	bool             bFirstRun;
};

}   // namespace vedo

#endif // _SIMULATEMEDIATOR_H
