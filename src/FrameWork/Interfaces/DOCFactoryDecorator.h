#ifndef _DISCRETE_OBJECT_FACTORY_DECORATOR_H
#define _DISCRETE_OBJECT_FACTORY_DECORATOR_H

#include <FrameWork/Interfaces/DOFactory.h>

namespace VEDO
{

class DOCFactoryDecorator : public DOFactory
{

public:

	DOCFactoryDecorator();

	~DOCFactoryDecorator();

	void AddDOCFactory(DOFactory* pdof);

	DiscreteObject* Create(const DOModel* pdoml, const DOStatus* pdos) const;

private:

	std::list<DOFactory*> lcDOCFactory;

};

};   // namespace VEDO

#endif // _DISCRETE_OBJECT_FACTORY_DECORATOR_H
