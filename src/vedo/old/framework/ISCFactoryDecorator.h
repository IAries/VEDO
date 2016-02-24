#ifndef _IMPACT_SOLVER_FACTORY_DECORATOR_H
#define _IMPACT_SOLVER_FACTORY_DECORATOR_H

#include <vedo/framework/ISFactory.h>
#include <vedo/framework/DataType.h>

namespace vedo
{

class ISCFactoryDecorator : public ISFactory
{

public:

	ISCFactoryDecorator();

	~ISCFactoryDecorator();

	void AddISCFactory(ISFactory* pisf);

	ImpactSolver* Create(const DiscreteObject* cpdoslave, const DiscreteObject* cpdomaster, const IactModel* cpiactmodel) const;

private:

	std::list<ISFactory*> lcISCFactory;

};

}   // namespace vedo

#endif // _IMPACT_SOLVER_FACTORY_DECORATOR_H
