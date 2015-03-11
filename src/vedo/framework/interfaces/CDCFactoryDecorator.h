#ifndef _CONTACT_DETECTOR_FACTORY_DECORATOR_H
#define _CONTACT_DETECTOR_FACTORY_DECORATOR_H

#include <vedo/framework/interfaces/CDFactory.h>
#include <list>

namespace vedo
{

class CDCFactoryDecorator : public CDFactory
{

public:

	CDCFactoryDecorator();

	~CDCFactoryDecorator();

	void AddCDCFactory(CDFactory* pcdf);

	ContactDetector* Create
		(const DiscreteObject* cpdoslave, const DiscreteObject* cpdomaster, const IactModel* cpiactmodel) const;

private:

	std::list<CDFactory*> lcCDCFactory;

};

}   // namespace vedo

#endif // _CONTACT_DETECTOR_FACTORY_DECORATOR_H
