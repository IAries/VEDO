#include <njr/Utility.h>
#include <vedo/framework/DOCFactoryDecorator.h>
#include <algorithm>

namespace vedo
{

DOCFactoryDecorator::DOCFactoryDecorator() : lcDOCFactory(0)
{
}

DOCFactoryDecorator::~DOCFactoryDecorator()
{
	for_each(lcDOCFactory.begin(),lcDOCFactory.end(), njr::Delete_ptr());
}

void DOCFactoryDecorator::AddDOCFactory(DOFactory* pdof)
{
	std::list<DOFactory *>::const_iterator idof;

	for (idof=lcDOCFactory.begin(); idof!=lcDOCFactory.end(); ++idof)
	{
		if ((pdof->behavior() == (*idof)->behavior()) && (pdof->shapetype() == (*idof)->shapetype()))
		{
			delete *idof;
			lcDOCFactory.remove(*idof);
			break;
		}
	}

	lcDOCFactory.push_back ( pdof );
}

DiscreteObject* DOCFactoryDecorator::Create(const DOModel* pdoml, const DOStatus* pdos) const
{
	std::list<DOFactory *>::const_iterator idof;
	DiscreteObject* pdo;
	for (idof=lcDOCFactory.begin(); idof!=lcDOCFactory.end(); ++idof)
	{
		pdo = (*idof)->Create(pdoml, pdos);
		if (pdo != 0)
		{
			return pdo;
		}
    }

	std::cerr
		<< "Error!! Code: DOCFactoryDecorator::Create (const DOModel*, const DOStatus*)" << std::endl
		<< "        Note: Unkown Discrete Object (DO factory decorator) DOModel \'" << pdoml->GetDOName().c_str() << '\'' << std::endl;
	exit(-1);
}

}   // namespace vedo
