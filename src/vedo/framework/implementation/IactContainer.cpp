#include <vedo/njr/interfaces/Utility.h>
#include <vedo/framework/interfaces/IactContainer.h>
#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>
#include <map>
#include <typeinfo>

namespace vedo
{

IactContainer::IactContainer(): vcIact(0)
{
	if(uNumUDDImpactStatus != 0)
	{
	    dpUDVInEachProcessor = new vedo_float_t[2*uNumUDDImpactStatus];
        for(vedo_uint_t u=0; u<2*uNumUDDImpactStatus; u++)
        {
            *(dpUDVInEachProcessor+u) = 0.0;
        }
	}
}

IactContainer::~IactContainer()
{
	IactContainer::Clear();
	delete dpUDVInEachProcessor;
}

void IactContainer::CalculateImpact(const vedo_float_t dt)
{
	for_each(vcIact.begin(), vcIact.end(), std::mem_fun(&Interaction::ControlError));
	for_each(vcIact.begin(), vcIact.end(), bind2nd(std::mem_fun(&Interaction::SolveImpact), dt));
}

void IactContainer::CalculateImpact(const vedo_float_t dt, vedo::vedo_uint_t ul)
{
	if (ul<vcIact.size())
	{
		vcIact[ul]->SolveImpact(dt);
	}
}

void IactContainer::CheckContactStatus()
{
	for_each(vcIact.begin(), vcIact.end(), std::mem_fun(&Interaction::DetectContact));
}

void IactContainer::Clear()
{
	for_each(vcIact.begin(), vcIact.end(), njr::Delete_ptr());
	vcIact.clear();
}

void IactContainer::Add(Interaction* pIact)
{
	if (pIact != 0)
	{
		vcIact.push_back(pIact);
	}
}

void IactContainer::CleanSolverStatus(vedo::vedo_uint_t i)
{
	if (i < vcIact.size())
	{
		vcIact[i]->CleanSolverStatus();
	}
}

vedo_float_t IactContainer::GetUserDefinedValue(vedo_uint_t u) const
{
	if (u < 2*uNumUDDImpactStatus)
	{
		return *(dpUDVInEachProcessor+u);
	}
	else
	{
		std::cerr << "Error!! Code: IactContainer::GetUserDefinedValue(vedo_uint_t)" << std::endl;
		exit(-1);
	}
}

void IactContainer::CollectUserDefinedData()
{
	if (uNumUDDImpactStatus != 0)
    {
        const vedo_float_t* dpUDV;

        for (vedo_uint_t u=uNumUDDImpactStatus; u<2*uNumUDDImpactStatus; u++)
        {
            *(dpUDVInEachProcessor+u) = 0.0;
        }

        for (vedo::vedo_uint_t ul=0; ul<vcIact.size(); ul++)
        {
            dpUDV = vcIact[ul]->RetrieveUserDefinedValue();
            for(vedo_uint_t u=0; u<2*uNumUDDImpactStatus; u++)
            {
                *(dpUDVInEachProcessor+u) += *(dpUDV+u);
            }
        }
    }
}

bool IactContainer::InteractionDetectContact(vedo::vedo_uint_t ul)
{
	if (ul < vcIact.size())
	{
		vcIact[ul]->DetectContact();
		return true;
	}
	else
	{
		return false;
	}
}

void IactContainer::BackupImpactStatus()
{
	for_each(vcIact.begin(), vcIact.end(), std::mem_fun(&Interaction::BackupImpactSolver));
}

void IactContainer::Dump(std::string dumpfile)
{
	std::ofstream f(dumpfile.c_str());
	copy(vcIact.begin(), vcIact.end(), std::ostream_iterator<Interaction* >(f));
	f.close();
}


}   // namespace vedo



static std::ostream& operator << (std::ostream& os, vedo::Interaction *pIact)
{
	static vedo::vedo_uint_t i = 0;
	os
		<< i++
		<< ' '
		<< pIact->GetDetector()->type()
		<< '('
		<< (vedo::vedo_int_t) pIact->GetContactInfo()->bActive
		<< ')'
		<< ' '
		<< pIact->GetSolver()->type()
		<< std::endl;
	return os;
}
