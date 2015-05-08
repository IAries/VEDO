#include <vedo/framework/DOContainer.h>
#include <njr/Utility.h>
#include <aries/utility/Vector3d.h>
#include <aries/utility/binder2nd_refArg.h>
#include <algorithm>
#include <functional>

namespace vedo
{

DOContainer::DOContainer(): lcDO(0), lcDOS(0)
{
}

DOContainer::~DOContainer()
{
	DOContainer::Clear();
}

void DOContainer::AddFieldImpact(const Vector3df& vFieldImpact)
{
	std::vector<DiscreteObject *>::iterator ido;
	for (ido=lcDO.begin(); ido!=lcDO.end(); ++ido)
	{
		(*ido)->AddImpact(vFieldImpact*((*ido)->GetSudoMass()));
	}
}

void DOContainer::AddImpact(const _uint_t& ul, const Vector3df& vImpact, const Vector3df& vAngularImpact)
{
	lcDO[ul]->AddImpact(vImpact, vAngularImpact);
}

/*
void DOContainer::AddExternalImpact(const std::vector<std::pair<Vector3df, Vector3df> >& vvExternalImpact)
{
	std::vector<DiscreteObject*>::iterator ido;
	std::vector<std::pair<Vector3df, Vector3df> >::iterator iExternalImpact;
	_uint_t ul = 0;
	for(ido=lcDO.begin(); ido!=lcDO.end(), ul<vvExternalImpact.size(); ++ido, ++ul)
	{
		(*ido)->AddImpact(vvExternalImpact[ul].first, vvExternalImpact[ul].second);
	}
}
*/

void DOContainer::AddConstrainedImpact(const _float_t dt)
{
	for_each(lcDO.begin(), lcDO.end(), bind2nd(std::mem_fun(&DiscreteObject::AddConstrainedImpact), dt));
}

void DOContainer::Response(const _float_t dt)
{
	for_each(lcDO.begin(), lcDO.end(), bind2nd(std::mem_fun(&DiscreteObject::Response), dt));
}

void DOContainer::EnforcePeriodicBoundaryConditions(const Boundary& pbc)
{
	typedef std::mem_fun1_t<void, DiscreteObject, const Boundary&> Func_Type;
	Func_Type func_obj(&DiscreteObject::EnforcePeriodicBoundaryConditions);
	aries::binder2nd_refArg<Func_Type> binded_func(func_obj, pbc);
	for_each(lcDO.begin(), lcDO.end(), binded_func);
}

void DOContainer::Clear()
{
	for_each(lcDO.begin(), lcDO.end(), njr::Delete_ptr());
	lcDO.clear();
	lcDOS.clear();
}

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
}

void DOContainer::Erase(const std::vector<_uint_t>& EraseList)
{
	const _uint_t numberDO      = lcDO.size();
	const _uint_t ErasenumberDO = EraseList.size();
	_uint_t ul;

	_uint_t ulEraseCounter = 0;
	for (ul=EraseList[0]; ul<numberDO; ul++)
	{
		if (ulEraseCounter < ErasenumberDO)
		{
			if (ul < EraseList[ulEraseCounter])
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
}

/*
void DOContainer::Erase(const std::vector<_uint_t>& EraseList)
{
	const _uint_t numberDO      = lcDO.size();
	const _uint_t ErasenumberDO = EraseList.size();
	const _uint_t NewnumberDO   = numberDO - ErasenumberDO;
	std::vector<DiscreteObject*> NewlcDO(NewnumberDO);
	std::vector<const DOStatus*> NewlcDOS(NewnumberDO);

	_uint_t ul;
	_uint_t ulEraseCounter = 0;
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
}
*/

}   // namespace vedo
