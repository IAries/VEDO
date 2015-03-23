#ifndef _DISCRETE_OBJECT_FACTORY_H
#define _DISCRETE_OBJECT_FACTORY_H

#include <vedo/framework/DOModel.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/DiscreteObject.h>
#include <vedo/framework/DataType.h>
#include <list>
#include <string>

namespace vedo
{

class DOFactory
{

public:

	virtual DOShapeType shapetype() const
	{
		return NoType;
	}

	virtual DOBehaviorType behavior() const
	{
		return NoDOBehaviorType;
	}

	virtual DiscreteObject* Create(const DOModel* pdoml, const DOStatus* pdos) const = 0;
};



template<class DO> class DOcf : public DOFactory
{

public:

	DOShapeType shapetype() const
	{
		return shType;
	}

	DOBehaviorType behavior() const
	{
		return eBehavior;
	}

	DOcf(DOShapeType Type, DOBehaviorType Behavior): shType(Type), eBehavior(Behavior)
	{
	}

	DiscreteObject* Create(const DOModel* pdoml, const DOStatus* pdos) const
	{
		if (pdoml->GetShapeType() != shType)
		{
			return 0;
		}

		if (pdoml->GetBehavior() != eBehavior)
		{
			return 0;
		}

		return new DO(pdos, pdoml);
	};

private:

	DOShapeType	   shType;

	DOBehaviorType eBehavior;

	DOcf();

	DOcf(const DOcf&);

	DOcf& operator == (DOcf&);

};

}   // namespace vedo

#endif // _DISCRETE_OBJECT_FACTORY_H
