#include <FrameWork/Interfaces/ImpactStatus.h>
#include <cstdlib>
#include <cstring>

namespace VEDO
{

ImpactStatus::ImpactStatus(): bContact(false), bBond(false), dKn(0.0), vShearForce()
{
	for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
		dUDV[u] = 0.0;
};

ImpactStatus::ImpactStatus(const ImpactStatus& is)
{
	*this = is;
};

const ImpactStatus& ImpactStatus::operator = (const ImpactStatus& is)
{
	bContact    = is.bContact;
	bBond       = is.bBond;
	dKn         = is.dKn;
	vShearForce = is.vShearForce;
	memcpy(dUDV, is.dUDV, 4*VEDO::uNumUserDefinedData*sizeof(double));
	return *this;
};

ImpactStatus::~ImpactStatus()
{
};

void ImpactStatus::CleanShearForce()
{
	vShearForce.Set(0.0, 0.0, 0.0);
};

void ImpactStatus::SetUserDefinedValue(unsigned u, double d)
{
	if (u < VEDO::uNumUserDefinedData)
	{
		dUDV[u+3*VEDO::uNumUserDefinedData] = d;
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::SetUserDefinedValue(unsigned, double)" << std::endl;
		exit(-1);
	}
};

double ImpactStatus::GetUserDefinedValue(unsigned u) const
{
	if (u < VEDO::uNumUserDefinedData)
	{
		return dUDV[u+3*VEDO::uNumUserDefinedData];
	}
	else
	{
		std::cerr
			<< "Error!! Code: ImpactStatus::GetUserDefinedValue(unsigned)"
			<< std::endl;
		exit(-1);
	}
};

void ImpactStatus::AddAccumulativeUserDefinedValue(unsigned u, double d)
{
	if (u < VEDO::uNumUserDefinedData)
	{
		dUDV[u                               ] += d;
		dUDV[u+VEDO::uNumUserDefinedData] += d;
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::AddAccumulativeUserDefinedValue(unsigned, double)" << std::endl;
		exit(-1);
	}
};

const double* ImpactStatus::RetrieveUserDefinedValue()
{
	memcpy
		(&dUDV[2*VEDO::uNumUserDefinedData]      ,
		 &dUDV[VEDO::uNumUserDefinedData]        ,
		 VEDO::uNumUserDefinedData*sizeof(double) );

	for(unsigned u=0; u<VEDO::uNumUserDefinedData; u++)
		dUDV[u+VEDO::uNumUserDefinedData] = 0.0;

	return &dUDV[2*VEDO::uNumUserDefinedData];
};

const double* ImpactStatus::RetrieveAllUserDefinedValue() const
{
	return &dUDV[0];
};

void ImpactStatus::SetAllUserDefinedValue(const double* dp)
{
	for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
		dUDV[u] = *(dp+u);
};

void ImpactStatus::CleanAllUserDefinedValue()
{
	for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
		dUDV[u] = 0.0;
};

void ImpactStatus::CleanUserDefinedValue()
{
	for(unsigned u=0; u<VEDO::uNumUserDefinedData; u++)
		dUDV[u+3*VEDO::uNumUserDefinedData] = 0.0;
};

void ImpactStatus::CleanAccumulativeUserDefinedValue()
{
	for(unsigned u=0; u<3*VEDO::uNumUserDefinedData; u++)
		dUDV[u] = 0.0;
};

void ImpactStatus::Clean()
{
	bContact = false;
	bBond    = false;
	dKn      = 0.0;
	CleanShearForce();

	for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
		dUDV[u] = 0.0;
};

};   // namespace VEDO
