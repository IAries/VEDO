#ifndef _IMPACT_SOLVER_FACTORY_H
#define _IMPACT_SOLVER_FACTORY_H

#include <FrameWork/Interfaces/DOWorld.h>
#include <FrameWork/Interfaces/DiscreteObject.h>
#include <FrameWork/Interfaces/ImpactSolver.h>
#include <list>
#include <string>

namespace vedo
{

class ISFactory
{

public:

	virtual std::string equationtype() const
	{
		return "NoEqType";
	};

	virtual ImpactSolver* Create
		(const DiscreteObject* cpdoslave,
		const DiscreteObject* cpdomaster,
		const IactModel* cpiactmodel) const = 0;
};

template<class IS> class IScf : public ISFactory
{

public:

	inline std::string equationtype() const
	{
		return sEquationType;
	};

	inline IScf(std::string EquationType) : sEquationType(EquationType)
	{
	};

	ImpactSolver* Create
		(const DiscreteObject* cpdoslave,
		const DiscreteObject* cpdomaster,
		const IactModel* cpiactmodel) const
	{
		return
			(cpiactmodel->GetEquationType() == sEquationType)
			? new IS(cpdoslave, cpdomaster, cpiactmodel) : 0;
	};

private:

	std::string sEquationType;
	IScf();
	IScf(const IScf&);
	IScf& operator == (IScf&);
};

};   // namespace vedo

#endif	// _IMPACT_SOLVER_FACTORY_H
