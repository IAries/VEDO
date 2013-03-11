#ifndef _CONTACT_DETECTOR_FACTORY_DECORATOR_H
#define _CONTACT_DETECTOR_FACTORY_DECORATOR_H

#include <FrameWork/Interfaces/CDFactory.h>
#include <list>

class CDCFactoryDecorator : public CDFactory
{

public:

	CDCFactoryDecorator();

	~CDCFactoryDecorator();

	void AddCDCFactory(CDFactory* pcdf);

 	ContactDetector* Create
	 	(const DiscreteObject* cpdoslave,
		const DiscreteObject* cpdomaster,
		const IactModel* cpiactmodel) const;

private:

	std::list<CDFactory*> lcCDCFactory;

};

#endif // _CONTACT_DETECTOR_FACTORY_DECORATOR_H
