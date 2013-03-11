#include <FrameWork/Interfaces/ISCFactoryDecorator.h>
#include <NJR/Interfaces/Utility.h>   // for NJR::Delete_ptr()
#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

ISCFactoryDecorator::ISCFactoryDecorator() : lcISCFactory(0)
{
};

ISCFactoryDecorator::~ISCFactoryDecorator()
{
	for_each(lcISCFactory.begin(), lcISCFactory.end(), NJR::Delete_ptr());
};

void ISCFactoryDecorator::AddISCFactory(ISFactory* pisf)
{
	list<ISFactory *>::const_iterator iisf;

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
};

ImpactSolver* ISCFactoryDecorator::Create
   (const DiscreteObject* cpdoslave,
	const DiscreteObject* cpdomaster,
	const IactModel* cpiactmodel) const
{
	list<ISFactory *>::const_iterator iisf;

	ImpactSolver* pis;

	for (iisf=lcISCFactory.begin(); iisf != lcISCFactory.end(); ++iisf)
	{
		pis=(*iisf)->Create(cpdoslave, cpdomaster, cpiactmodel);
		if (pis != 0)
		{
			return pis;
		}
    }
	cerr
		<< "Unkown Impact Solver(IS c factory decorator "
		<< cpiactmodel->GetEquationType().c_str()
		<< endl;
	exit (0);
};
