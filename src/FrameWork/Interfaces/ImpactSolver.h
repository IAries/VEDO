#ifndef _IMPACT_SOLVER_H
#define _IMPACT_SOLVER_H

#include <FrameWork/Interfaces/ContactDetector.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <FrameWork/Interfaces/DiscreteObject.h>
#include <FrameWork/Interfaces/ISwModels.h>
#include <FrameWork/Interfaces/ImpactStatus.h>

namespace VEDO
{

class ImpactSolver
{

public:

	virtual ~ImpactSolver();

	virtual NJR::Vector3d NextStep
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

};   // namespace VEDO

#endif // _IMPACT_SOLVER_H
