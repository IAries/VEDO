#include <FrameWork/Interfaces/CDCFactoryDecorator.h>
#include <NJR/Interfaces/Utility.h>   // for NJR::Delete_ptr()
#include <algorithm>
#include <typeinfo>

using namespace std;

CDCFactoryDecorator::CDCFactoryDecorator() : lcCDCFactory(0)
{
};

CDCFactoryDecorator::~CDCFactoryDecorator()
{
	for_each(lcCDCFactory.begin(), lcCDCFactory.end(), NJR::Delete_ptr());
};

void CDCFactoryDecorator::AddCDCFactory(CDFactory* pcdf)
{
	list<CDFactory *>::const_iterator icdf;

	for (icdf=lcCDCFactory.begin(); icdf!=lcCDCFactory.end(); ++icdf)
	{
		if( (pcdf->equationtype())==((*icdf)->equationtype()) &&
			(pcdf->slavetype())   ==((*icdf)->slavetype())&&
			(pcdf->mastertype())  ==((*icdf)->mastertype())     )
		{
		    delete *icdf;
			lcCDCFactory.remove(*icdf);
			break;
		}
	}
	lcCDCFactory.push_back(pcdf);
};

ContactDetector* CDCFactoryDecorator::Create
	(const DiscreteObject* cpdoslave,
	const DiscreteObject* cpdomaster,
	const IactModel* cpiactmodel) const
{
    ContactDetector* pcd;
	list<CDFactory *>::const_iterator icdf;

	for (icdf=lcCDCFactory.begin(); icdf!=lcCDCFactory.end(); ++icdf)
	{
		pcd = (*icdf)->Create(cpdoslave,cpdomaster,cpiactmodel);
		if (pcd != 0)
		{
			return pcd;
		}
    }

	cerr
		<< "Unkown Contact Detector(CD c factory decorator)"
		<< endl
		<< "SlaveModel = "
		<< cpdoslave->GetDOModel()->GetDOName()
		<< endl
		<< "MasterModel = "
		<< cpdomaster->GetDOModel()->GetDOName()
		<< endl
		<< "EquationType = "
		<< cpiactmodel->GetEquationType()
		<< endl;
	exit (0);
};
