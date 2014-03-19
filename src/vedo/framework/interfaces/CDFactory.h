#ifndef _CONTACT_DETECTOR_FACTORY_H
#define _CONTACT_DETECTOR_FACTORY_H

#include <vedo/framework/interfaces/ContactDetector.h>
#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/DiscreteObject.h>

#include <string>

namespace vedo
{

class CDFactory
{

public:

	virtual DOShapeType slavetype() const
	{
		return NoType;
	};

	virtual DOShapeType mastertype() const
	{
		return NoType;
	};

	virtual std::string equationtype() const
	{
		return "NoEqType";
	};

	virtual ContactDetector* Create
		(const DiscreteObject* cpdoslave,
		const DiscreteObject* cpdomaster,
		const IactModel* cpiactmodel) const = 0;
};



template<class CD> class CDcf : public CDFactory
{

public:

	DOShapeType slavetype() const
	{
		return shSType;
	};

	DOShapeType mastertype() const
	{
		return shMType;
	};

	std::string equationtype() const
	{
		return sEquationType;
	};

	CDcf
		(const DOShapeType SType,
		const DOShapeType MType,
		const std::string EquationType)
	: shSType(SType), shMType(MType), sEquationType(EquationType)
	{
	};

	ContactDetector* Create
		(const DiscreteObject* cpdoslave,
		const DiscreteObject* cpdomaster,
		const IactModel* cpiactmodel) const
	{

		if (cpdoslave->GetDOModel()->GetShapeType() != shSType)
		{
			return 0;
		}

		if (cpdomaster->GetDOModel()->GetShapeType() != shMType)
		{
			return 0;
		}

		if ( cpiactmodel->GetEquationType() == sEquationType)
		{
			return new CD;
		}

		std::string eqtype = " <CT>" + cpiactmodel->GetEquationType();

		if ((sEquationType.find("List- ") == 0) &&
			(sEquationType.find(eqtype) != std::string::npos) )
		{
			return new CD;
		}
		else
		{
			return 0 ;
		}
	};

private:

	DOShapeType	shSType;

	DOShapeType shMType;

	std::string sEquationType;

	CDcf();

	CDcf(const CDcf&);

	CDcf& operator == (CDcf&);

};

};   // namespace vedo

#endif // _CONTACT_DETECTOR_FACTORY_H
