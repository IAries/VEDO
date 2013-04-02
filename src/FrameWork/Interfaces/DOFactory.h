#ifndef _DISCRETE_OBJECT_FACTORY_H
#define _DISCRETE_OBJECT_FACTORY_H

#include <Framework/Interfaces/DOWorld.h>
#include <Framework/Interfaces/DiscreteObject.h>
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
	};

	virtual std::string behavior() const
	{
		return "NoBehavior";
	};

	virtual DiscreteObject* Create
		(const DOModel* pdoml, const DOStatus* pdos) const = 0;
};



template<class DO> class DOcf : public DOFactory
{

public:

	DOShapeType shapetype() const
	{
		return shType;
	};

	std::string behavior() const
	{
		return sBehavior;
	};

	DOcf(DOShapeType Type, std::string Behavior)
		: shType(Type), sBehavior(Behavior)
	{
	};

	DiscreteObject* Create(const DOModel* pdoml, const DOStatus* pdos) const
	{
		if (pdoml->GetShapeType() != shType)
		{
			return 0;
		}

		if (pdoml->GetBehavior() != sBehavior)
		{
			return 0;
		}

		return new DO(pdos, pdoml);
	};

private:

	DOShapeType	shType;
	std::string	sBehavior;
	DOcf();
	DOcf(const DOcf&);
	DOcf& operator == (DOcf&);

};

};   // namespace vedo

#endif // _DISCRETE_OBJECT_FACTORY_H
