#ifndef _SIMULATEMEDIATOR_H
#define _SIMULATEMEDIATOR_H

#include <vedo/framework/Assembler.h>
#include <vedo/framework/Consultant.h>
#include <vedo/framework/DOContainer.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/IactContainer.h>
#include <vedo/framework/DataType.h>
#include <ctime>
#include <string>
#include <fstream>

namespace vedo
{

class SimMediator
{

public:

	SimMediator(Consultant* Consultant, const Assembler* Assem);

	SimMediator(Consultant* Consultant, const Assembler* Assem, const _uint_t, const _uint_t);

	virtual ~SimMediator();

	virtual bool Run();

	virtual bool Run(const std::vector<std::pair<Vector3df, Vector3df> >&);

	virtual bool Run(const std::map<_uint_t, DOStatus>&);

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

	void WriteInteractionForce(const char* filename, const std::vector<std::pair<Vector3df, Vector3df> >* extImpact=0);

	void CalculateSystemEnergy();

	std::time_t  starttime;          // Starting time

	std::time_t  endtime;            // Endind time

	_float_t timeSystem;             // Time of system preparing, starting and ending

	_float_t timeImpactSolving;      // Time of impact solving

	_float_t timeFieldForceAdding;   // Time of field force adding

	_float_t timeResponseUpdating;   // Time of response updating

	_float_t timeContactDetection;   // Time of contact detection

	_float_t timeNextStep;           // Time of "NextStep"

	_float_t timeSyncDOContainer;    // Time of "SyncDOContainer"

	_float_t timePartitioning;       // Time of partitioning

	_float_t timeComputing;          // Time of computing

	_float_t timeCommunication;      // Time of communication

	_float_t timeTotal;              // Total time

	void SetDOStatus(const _uint_t&, const DOStatus&);

private:

	Consultant*      pConsultant;

	const Assembler* cpAssembler;

	DOContainer      cDO;                   // Local discrete objects in each computing unit

	IactContainer    cIact;                 // Local interactions in each computing unit

	_uint_t          rank;                  // Rank of processores

	_uint_t          NP;                    // Number of processores

	std::ofstream    FileLog;               // Log file

	std::ofstream    FileLogSystemStatus;   // Log file of system status

	bool             bFirstRun;
};

}   // namespace vedo

#endif // _SIMULATEMEDIATOR_H
