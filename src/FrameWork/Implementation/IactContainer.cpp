#include <NJR/Interfaces/Utility.h>
#include <FrameWork/Interfaces/IactContainer.h>
#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>
#include <map>
#include <typeinfo>

using namespace std;

IactContainer::IactContainer(): vcIact(0)
{
	for(unsigned u=0; u<2*uNumUserDefinedData; u++)
		dUDVInEachProcessor[u] = 0.0;
};

IactContainer::~IactContainer()
{
	IactContainer::Clear();
};

void IactContainer::CalculateImpact(const double dt)
{
	for_each
		(vcIact.begin(),
		vcIact.end(),
		mem_fun(&Interaction::ControlError));

	for_each
		(vcIact.begin(),
		vcIact.end(),
		bind2nd(mem_fun(&Interaction::SolveImpact), dt));
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
		mem_fun(&Interaction::DetectContact));
};

void IactContainer::Clear()
{
	for_each(vcIact.begin(), vcIact.end(), NJR::Delete_ptr());
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
	if(u<2*uNumUserDefinedData)
	{
		return dUDVInEachProcessor[u];
	}
	else
	{
		cerr << "IactContainer::GetUserDefinedValue(unsigned u) ERROR!!" << endl;
		exit(-1);
	};
};

void IactContainer::CollectUserDefinedData()
{
	const double* dpUDV;

	for(unsigned u=uNumUserDefinedData; u<2*uNumUserDefinedData; u++)
		dUDVInEachProcessor[u] = 0.0;

	for(unsigned long ul=0; ul<vcIact.size(); ul++)
	{
		dpUDV = vcIact[ul]->RetrieveUserDefinedValue();
		for(unsigned u=0; u<2*uNumUserDefinedData; u++)
			dUDVInEachProcessor[u] += *(dpUDV+u);
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

void IactContainer::Dump(string dumpfile)
{
	ofstream f(dumpfile.c_str());
	copy(vcIact.begin(), vcIact.end(), ostream_iterator<Interaction* >(f));
	f.close();
};

static ostream& operator << (ostream& os, Interaction *pIact)
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
		<< endl;
	return os;
};
