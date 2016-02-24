#include <njr/Utility.h>
#include <vedo/framework/CDCFactoryDecorator.h>
#include <algorithm>
#include <typeinfo>

namespace vedo
{

CDCFactoryDecorator::CDCFactoryDecorator() : lcCDCFactory(0)
{
}

CDCFactoryDecorator::~CDCFactoryDecorator()
{
	for_each(lcCDCFactory.begin(), lcCDCFactory.end(), njr::Delete_ptr());
}

void CDCFactoryDecorator::AddCDCFactory(CDFactory* pcdf)
{
	std::list<CDFactory *>::const_iterator icdf;

	for (icdf=lcCDCFactory.begin(); icdf!=lcCDCFactory.end(); ++icdf)
	{
		if (   (pcdf->equationtype()) == ((*icdf)->equationtype())
			&& (pcdf->slavetype()   ) == ((*icdf)->slavetype()   )
			&& (pcdf->mastertype()  ) == ((*icdf)->mastertype()  ) )
		{
		    delete *icdf;
			lcCDCFactory.remove(*icdf);
			break;
		}
	}
	lcCDCFactory.push_back(pcdf);
}

ContactDetector* CDCFactoryDecorator::Create
	(const DiscreteObject* cpdoslave, const DiscreteObject* cpdomaster, const IactModel* cpiactmodel) const
{
    ContactDetector* pcd;
	std::list<CDFactory *>::const_iterator icdf;

	for (icdf=lcCDCFactory.begin(); icdf!=lcCDCFactory.end(); ++icdf)
	{
		pcd = (*icdf)->Create(cpdoslave,cpdomaster,cpiactmodel);
		if (pcd != 0)
		{
			return pcd;
		}
    }

	std::cerr
		<< "Error!! Code: CDCFactoryDecorator::Create (const DiscreteObject*, const DiscreteObject*, const IactModel*)" << std::endl
		<< "        Note: Unkown Contact Detector(CD c factory decorator)"          << std::endl
		<< "              SlaveModel   = " << cpdoslave->GetDOModel()->GetDOName()  << std::endl
		<< "              MasterModel  = " << cpdomaster->GetDOModel()->GetDOName() << std::endl
		<< "              EquationType = " << cpiactmodel->GetEquationType() 	    << std::endl;
	exit(-1);
}

}   // namespace vedo
