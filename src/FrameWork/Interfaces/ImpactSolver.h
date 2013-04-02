#ifndef _IMPACT_SOLVER_H
#define _IMPACT_SOLVER_H

#include <Framework/Interfaces/ContactDetector.h>
#include <Framework/Interfaces/DOWorld.h>
#include <Framework/Interfaces/DiscreteObject.h>
#include <Framework/Interfaces/ISwModels.h>
#include <Framework/Interfaces/ImpactStatus.h>

namespace vedo
{

class ImpactSolver
{

public:

	virtual ~ImpactSolver();

	virtual njr::Vector3d NextStep
		(const ContactDetector* pcd,
		DiscreteObject* pdoSlave,
		DiscreteObject* pdoMaster,
		double dt) = 0;

	virtual bool InitialStep
		(const ContactDetector* pcd,
		DiscreteObject* pdoSlave,
		DiscreteObject* pdoMaster   ) = 0;

	virtual std::string type() const
	{
		return "ImpactSolver";
	};

	inline const ImpactStatus* GetImpactStatus() const
	{
		return &ImStatus;
	}

	inline void SetImpactStatus(const ImpactStatus* status)
	{
		ImStatus = (*status);
	}

	inline const double* RetrieveUserDefinedValue()
	{
		return ImStatus.RetrieveUserDefinedValue();
	};

	inline void CleanStatus()
	{
		ImStatus.Clean();
	}

protected:

	const IactModel* cpIactModel;

	ImpactSolver(const IactModel* cpIactml);

	ImpactStatus ImStatus;

	ISwModels* ism;

private:

	ImpactSolver(const ImpactSolver&);

	ImpactSolver& operator = (const ImpactSolver&);
};

};   // namespace vedo

#endif // _IMPACT_SOLVER_H
