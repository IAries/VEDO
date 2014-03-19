#include <vedo/njr/interfaces/Utility.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/DOContainer.h>
#include <algorithm>
#include <functional>

namespace vedo
{

DOContainer::DOContainer(): lcDO(0), lcDOS(0)
{
};

DOContainer::~DOContainer()
{
	DOContainer::Clear();
};

void DOContainer::AddFieldImpact(const njr::Vector3d& vFieldImpact)
{
	std::vector<DiscreteObject *>::iterator ido;
	for(ido=lcDO.begin(); ido!=lcDO.end(); ++ido)
	{
		(*ido)->AddImpact(vFieldImpact*((*ido)->GetSudoMass()));
	}
};

void DOContainer::AddImpact
	(const unsigned long& ul,
	 const njr::Vector3d& vImpact,
	 const njr::Vector3d& vAngularImpact)
{
	lcDO[ul]->AddImpact(vImpact, vAngularImpact);
};

/*
void DOContainer::AddExternalImpact
	(const std::vector<std::pair<njr::Vector3d, njr::Vector3d> >&
	 vvExternalImpact                                        )
{
	std::vector<DiscreteObject*>::iterator ido;
	std::vector<std::pair<njr::Vector3d, njr::Vector3d> >::iterator iExternalImpact;
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
		 bind2nd(std::mem_fun(&DiscreteObject::AddConstrainedImpact), dt));
};

void DOContainer::Response(const double dt)
{
	for_each
		(lcDO.begin(),
		 lcDO.end(),
		 bind2nd(std::mem_fun(&DiscreteObject::Response), dt));
};

void DOContainer::EnforcePeriodicBoundaryConditions(const Boundary& pbc)
{
	typedef std::mem_fun1_t<void, DiscreteObject, const Boundary&> Func_Type;
	Func_Type func_obj(&DiscreteObject::EnforcePeriodicBoundaryConditions);
	njr::binder2nd_refArg<Func_Type> binded_func(func_obj, pbc);
	for_each(lcDO.begin(), lcDO.end(), binded_func);
};

void DOContainer::Clear()
{
	for_each
		(lcDO.begin(),
		lcDO.end(),
		njr::Delete_ptr() );
	lcDO.clear();
	lcDOS.clear();
};

void DOContainer::Add(DiscreteObject* pdo)
{
	if (pdo == 0)
    {
		std::cerr
			<< "Error!! Code: DOContainer::Add(DiscreteObject*)" << std::endl
			<< "        Note: SimMediator do not accept null Discrete Object" << std::endl;
		exit(-1);
	}

	lcDO.push_back(pdo);
	lcDOS.push_back(pdo->GetDOStatus());
};

void DOContainer::Erase(const std::vector<unsigned long>& EraseList)
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
void DOContainer::Erase(const std::vector<unsigned long>& EraseList)
{
	const unsigned long numberDO      = lcDO.size();
	const unsigned long ErasenumberDO = EraseList.size();
	const unsigned long NewnumberDO   = numberDO - ErasenumberDO;
	std::vector<DiscreteObject*> NewlcDO(NewnumberDO);
	std::vector<const DOStatus*> NewlcDOS(NewnumberDO);

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

};   // namespace vedo
