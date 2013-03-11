#include <FrameWork/Interfaces/DOCFactoryDecorator.h>
#include <NJR/Interfaces/Utility.h>   // for NJR::Delete_ptr()
#include <algorithm>

using namespace std;

DOCFactoryDecorator::DOCFactoryDecorator() : lcDOCFactory(0)
{
};

DOCFactoryDecorator::~DOCFactoryDecorator()
{
	for_each(lcDOCFactory.begin(),lcDOCFactory.end(), NJR::Delete_ptr());
};

void DOCFactoryDecorator::AddDOCFactory(DOFactory* pdof)
{
	list<DOFactory *>::const_iterator idof;

	for (idof=lcDOCFactory.begin(); idof!=lcDOCFactory.end(); ++idof)
	{
		if (pdof->behavior() == (*idof)->behavior() &&
			pdof->shapetype() == (*idof)->shapetype())
		{
			delete *idof;
			lcDOCFactory.remove(*idof);
			break;
		}
	}

	lcDOCFactory.push_back ( pdof );
};

DiscreteObject* DOCFactoryDecorator::Create
	(const DOModel* pdoml, const DOStatus* pdos) const
{
	list<DOFactory *>::const_iterator idof;
	DiscreteObject* pdo;
	for (idof=lcDOCFactory.begin(); idof!=lcDOCFactory.end(); ++idof)
	{
		pdo = (*idof)->Create(pdoml, pdos);
		if (pdo != 0)
		{
			return pdo;
		};
    };

	cerr
		<< "Unkown Discrete Object (DO factory decorator) DOModel "
		<< pdoml->GetDOName().c_str()
		<< endl;
	exit (0);
};
