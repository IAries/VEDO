#include <vedo/framework/ISCFactoryDecorator.h>
#include <njr/Utility.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>

namespace vedo
{

ISCFactoryDecorator::ISCFactoryDecorator() : lcISCFactory(0)
{
}

ISCFactoryDecorator::~ISCFactoryDecorator()
{
	for_each(lcISCFactory.begin(), lcISCFactory.end(), njr::Delete_ptr());
}

void ISCFactoryDecorator::AddISCFactory(ISFactory* pisf)
{
	std::list<ISFactory *>::const_iterator iisf;

	for (iisf=lcISCFactory.begin(); iisf != lcISCFactory.end(); ++iisf)
	{
		if (pisf->equationtype() == (*iisf)->equationtype())
		{
			delete *iisf;
			lcISCFactory.remove(*iisf);
			break;
		}
	}

	lcISCFactory.push_back(pisf);
}

ImpactSolver* ISCFactoryDecorator::Create
	(const DiscreteObject* cpdoslave, const DiscreteObject* cpdomaster, const IactModel* cpiactmodel) const
{
	std::list<ISFactory *>::const_iterator iisf;

	ImpactSolver* pis;

	for (iisf=lcISCFactory.begin(); iisf != lcISCFactory.end(); ++iisf)
	{
		pis=(*iisf)->Create(cpdoslave, cpdomaster, cpiactmodel);
		if (pis != 0)
		{
			return pis;
		}
    }

	std::cerr
		<< "Error!! Code: ISCFactoryDecorator::Create (const DiscreteObject*, const DiscreteObject*, const IactModel*)" << std::endl
		<< "        Note: Unkown Impact Solver \'" << cpiactmodel->GetEquationType().c_str() << '\'' << std::endl;
	exit(-1);
}

}   // namespace vedo
