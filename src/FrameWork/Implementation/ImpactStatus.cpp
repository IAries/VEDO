#include <FrameWork/Interfaces/ImpactStatus.h>
#include <cstdlib>
#include <cstring>

using namespace std;

ImpactStatus::ImpactStatus(): bContact(false), bBond(false), dKn(0.0), vShearForce()
{
	for(unsigned u=0; u<4*uNumUserDefinedData; u++)
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
	memcpy(dUDV, is.dUDV, 4*uNumUserDefinedData*sizeof(double));
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
	if (u < uNumUserDefinedData)
	{
		dUDV[u+3*uNumUserDefinedData] = d;
	}
	else
	{
		cerr << "ImpactStatus::SetUserDefinedValue(unsigned, double&) ERROR!!" << endl;
		exit(-1);
	}
};

double ImpactStatus::GetUserDefinedValue(unsigned u) const
{
	if (u < uNumUserDefinedData)
	{
		return dUDV[u+3*uNumUserDefinedData];
	}
	else
	{
		cerr << "ImpactStatus::GetUserDefinedValue(unsigned) ERROR!!" << endl;
		exit(-1);
	}
};

void ImpactStatus::AddAccumulativeUserDefinedValue(unsigned u, double d)
{
	if (u < uNumUserDefinedData)
	{
		dUDV[u                    ] += d;
		dUDV[u+uNumUserDefinedData] += d;
	}
	else
	{
		cerr << "ImpactStatus::AddAccumulativeUserDefinedValue(unsigned, double&) ERROR!!" << endl;
		exit(-1);
	}
};

const double* ImpactStatus::RetrieveUserDefinedValue()
{
	memcpy
		(&dUDV[2*uNumUserDefinedData]      ,
		 &dUDV[uNumUserDefinedData]        ,
		 uNumUserDefinedData*sizeof(double) );

	for(unsigned u=0; u<uNumUserDefinedData; u++)
		dUDV[u+uNumUserDefinedData] = 0.0;

	return &dUDV[2*uNumUserDefinedData];
};

const double* ImpactStatus::RetrieveAllUserDefinedValue() const
{
	return &dUDV[0];
};

void ImpactStatus::SetAllUserDefinedValue(const double* dp)
{
	for(unsigned u=0; u<4*uNumUserDefinedData; u++)
		dUDV[u] = *(dp+u);
};

void ImpactStatus::CleanAllUserDefinedValue()
{
	for(unsigned u=0; u<4*uNumUserDefinedData; u++)
		dUDV[u] = 0.0;
};

void ImpactStatus::CleanUserDefinedValue()
{
	for(unsigned u=0; u<uNumUserDefinedData; u++)
		dUDV[u+3*uNumUserDefinedData] = 0.0;
};

void ImpactStatus::CleanAccumulativeUserDefinedValue()
{
	for(unsigned u=0; u<3*uNumUserDefinedData; u++)
		dUDV[u] = 0.0;
};

void ImpactStatus::Clean()
{
	bContact = false;
	bBond    = false;
	dKn      = 0.0;
	CleanShearForce();

	for(unsigned u=0; u<4*uNumUserDefinedData; u++)
		dUDV[u] = 0.0;
};
