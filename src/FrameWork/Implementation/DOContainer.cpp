#include <NJR/Interfaces/Utility.h>
#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/DOContainer.h>
#include <algorithm>
#include <functional>

using namespace std;

DOContainer::DOContainer(): lcDO(0), lcDOS(0)
{
};

DOContainer::~DOContainer()
{
	DOContainer::Clear();
};

void DOContainer::AddFieldImpact(const NJRvector3d& vFieldImpact)
{
	vector<DiscreteObject *>::iterator ido;
	for(ido=lcDO.begin(); ido!=lcDO.end(); ++ido)
	{
		(*ido)->AddImpact(vFieldImpact*((*ido)->GetSudoMass()));
	}
};

void DOContainer::AddImpact
	(const unsigned long& ul,
	 const NJRvector3d& vImpact,
	 const NJRvector3d& vAngularImpact)
{
	lcDO[ul]->AddImpact(vImpact, vAngularImpact);
};

/*
void DOContainer::AddExternalImpact
	(const vector<pair<NJRvector3d, NJRvector3d> >&
	 vvExternalImpact                                        )
{
	vector<DiscreteObject*>::iterator ido;
	vector<pair<NJRvector3d, NJRvector3d> >::iterator iExternalImpact;
	unsigned long ul = 0;
	for(ido=lcDO.begin();
		ido!=lcDO.end(), ul<vvExternalImpact.size();
		++ido, ++ul)
	{
		(*ido)->AddImpact(vvExternalImpact[ul].first, vvExternalImpact[ul].second);
	}
};
*/

void DOContainer::AddConstrainedImpact(const double dt)
{
	for_each
		(lcDO.begin(),
		 lcDO.end(),
		 bind2nd(mem_fun(&DiscreteObject::AddConstrainedImpact), dt));
};

void DOContainer::Response(const double dt)
{
	for_each
		(lcDO.begin(),
		 lcDO.end(),
		 bind2nd(mem_fun(&DiscreteObject::Response), dt));
};

void DOContainer::EnforcePeriodicBoundaryConditions(const Boundary& pbc)
{
	typedef mem_fun1_t<void, DiscreteObject, const Boundary&> Func_Type;
	Func_Type func_obj(&DiscreteObject::EnforcePeriodicBoundaryConditions);
	NJR::binder2nd_refArg<Func_Type> binded_func(func_obj, pbc);
	for_each(lcDO.begin(), lcDO.end(), binded_func);
};

void DOContainer::Clear()
{
	for_each
		(lcDO.begin(),
		lcDO.end(),
		NJR::Delete_ptr() );
	lcDO.clear();
	lcDOS.clear();
};

void DOContainer::Add(DiscreteObject* pdo)
{
	if (pdo == 0)
    {
		cerr
			<< "(DOContainer:ADD) "
			<< "Sorry! SimMediator do not accept null Discrete Object"
			<< endl
			<< "Sysem do not know how to create discrete object you want"
			<< endl;
		exit(1);
	}

	lcDO.push_back(pdo);
	lcDOS.push_back(pdo->GetDOStatus());
};

void DOContainer::Erase(const vector<unsigned long>& EraseList)
{
	const unsigned long numberDO      = lcDO.size();
	const unsigned long ErasenumberDO = EraseList.size();
	unsigned long ul;

	unsigned long ulEraseCounter = 0;
	for(ul=EraseList[0]; ul<numberDO; ul++)
	{
		if(ulEraseCounter < ErasenumberDO)
		{
			if(ul < EraseList[ulEraseCounter])
			{
				lcDO[ul-ulEraseCounter] = lcDO[ul];
				lcDOS[ul-ulEraseCounter] = lcDOS[ul];
			}
			else
			{
				ulEraseCounter++;
			}
		}
		else
		{
			lcDO[ul-ulEraseCounter] = lcDO[ul];
			lcDOS[ul-ulEraseCounter] = lcDOS[ul];
		}
	}
	lcDO.resize(numberDO-ErasenumberDO);
	lcDOS.resize(numberDO-ErasenumberDO);
};

/*
void DOContainer::Erase(const vector<unsigned long>& EraseList)
{
	const unsigned long numberDO      = lcDO.size();
	const unsigned long ErasenumberDO = EraseList.size();
	const unsigned long NewnumberDO   = numberDO - ErasenumberDO;
	vector<DiscreteObject*> NewlcDO(NewnumberDO);
	vector<const DOStatus*> NewlcDOS(NewnumberDO);

	unsigned long ul;
	unsigned long ulEraseCounter = 0;
	for(ul=0; ul<numberDO; ul++)
	{
		if(ulEraseCounter < ErasenumberDO)
		{
			if(ul < EraseList[ulEraseCounter])
			{
				NewlcDO[ul-ulEraseCounter] = lcDO[ul];
				NewlcDOS[ul-ulEraseCounter] = lcDOS[ul];
			}
			else
			{
				lcDO[ul]->~DiscreteObject();
				lcDOS[ul]->~DOStatus();
				ulEraseCounter++;
			}
		}
		else
		{
			NewlcDO[ul-ulEraseCounter] = lcDO[ul];
			NewlcDOS[ul-ulEraseCounter] = lcDOS[ul];
		}
	}
	lcDO.clear();
	lcDOS.clear();
	lcDO.resize(NewnumberDO);
	lcDOS.resize(NewnumberDO);
//	lcDO = NewlcDO;
//	lcDOS = NewlcDOS;
	for(ul=0; ul<lcDO.size(); ul++)
	{
		lcDO[ul] = NewlcDO[ul];
		lcDOS[ul] = NewlcDOS[ul];
		NewlcDO[ul]->~DiscreteObject();
		NewlcDOS[ul]->~DOStatus();
	}
	NewlcDO.clear();
	NewlcDOS.clear();
};
*/
