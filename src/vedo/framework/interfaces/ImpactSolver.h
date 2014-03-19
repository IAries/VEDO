#ifndef _IMPACT_SOLVER_H
#define _IMPACT_SOLVER_H

#include <vedo/framework/interfaces/Boundary.h>
#include <vedo/framework/interfaces/ContactDetector.h>
#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/DiscreteObject.h>
#include <vedo/framework/interfaces/ISwModels.h>
#include <vedo/framework/interfaces/ImpactStatus.h>

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

	inline void SetPeriodicBoundaryConditions(const Boundary* bc)
	{
		pBC = bc;
	};

	inline const double* RetrieveUserDefinedValue()
	{
		return ImStatus.RetrieveUserDefinedValue();
	};

	inline void CleanStatus()
	{
		ImStatus.Clean();
	}

protected:

	const Boundary* pBC;

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
