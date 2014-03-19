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
	    dpUDVInEachProcessor = new double[2*uNumUDDImpactStatus];
        for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
            *(dpUDVInEachProcessor+u) = 0.0;
	}
};

IactContainer::~IactContainer()
{
	IactContainer::Clear();
	delete dpUDVInEachProcessor;
};

void IactContainer::CalculateImpact(const double dt)
{
	for_each
		(vcIact.begin(),
		vcIact.end(),
		std::mem_fun(&Interaction::ControlError));

	for_each
		(vcIact.begin(),
		vcIact.end(),
		bind2nd(std::mem_fun(&Interaction::SolveImpact), dt));
};

void IactContainer::CalculateImpact(const double dt, unsigned long ul)
{
	if (ul<vcIact.size())
		vcIact[ul]->SolveImpact(dt);
};

void IactContainer::CheckContactStatus()
{
	for_each
		(vcIact.begin(),
		vcIact.end(),
		std::mem_fun(&Interaction::DetectContact));
};

void IactContainer::Clear()
{
	for_each(vcIact.begin(), vcIact.end(), njr::Delete_ptr());
	vcIact.clear();
};

void IactContainer::Add(Interaction* pIact)
{
	if(pIact != 0)
		vcIact.push_back(pIact);
};

void IactContainer::CleanSolverStatus(unsigned long i)
{
	if (i < vcIact.size())
		vcIact[i]->CleanSolverStatus();
};

double IactContainer::GetUserDefinedValue(unsigned u) const
{
	if(u < 2*uNumUDDImpactStatus)
	{
		return *(dpUDVInEachProcessor+u);
	}
	else
	{
		std::cerr
			<< "Error!! Code: IactContainer::GetUserDefinedValue(unsigned)"
			<< std::endl;
		exit(-1);
	};
};

void IactContainer::CollectUserDefinedData()
{
	if(uNumUDDImpactStatus != 0)
    {
        const double* dpUDV;

        for(unsigned u=uNumUDDImpactStatus; u<2*uNumUDDImpactStatus; u++)
            *(dpUDVInEachProcessor+u) = 0.0;

        for(unsigned long ul=0; ul<vcIact.size(); ul++)
        {
            dpUDV = vcIact[ul]->RetrieveUserDefinedValue();
            for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
                *(dpUDVInEachProcessor+u) += *(dpUDV+u);
        }
    }
};

bool IactContainer::InteractionDetectContact(unsigned long ul)
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
};

void IactContainer::Dump(std::string dumpfile)
{
	std::ofstream f(dumpfile.c_str());
	copy(vcIact.begin(), vcIact.end(), std::ostream_iterator<Interaction* >(f));
	f.close();
};


};   // namespace vedo



static std::ostream& operator << (std::ostream& os, vedo::Interaction *pIact)
{
	static unsigned int i = 0;
	os << i++
		<< ' '
		<< pIact->GetDetector()->type()
		<< '('
		<< (int) pIact->GetContactInfo()->bActive
		<< ')'
		<< ' '
		<< pIact->GetSolver()->type()
		<< std::endl;
	return os;
};
