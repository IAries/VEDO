#include <FrameWork/Interfaces/ImpactStatus.h>
#include <cstdlib>
#include <cstring>

namespace vedo
{

ImpactStatus::ImpactStatus(): bContact(false), bBond(false), dKn(0.0), vShearForce()
{
	for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
		dUDV[u] = 0.0;
};

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond, const double& Kn,
     const njr::Vector3d& ShearForce, const double* UDVp       ):
    bContact(Contact), bBond(Bond),dKn(Kn), vShearForce(ShearForce)
{
	for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
		dUDV[u] = *(UDVp+u);
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
	memcpy(dUDV, is.dUDV, 4*uNumUDDImpactStatus*sizeof(double));
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
	if (u < uNumUDDImpactStatus)
	{
		dUDV[u+3*uNumUDDImpactStatus] = d;
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::SetUserDefinedValue(unsigned, double)" << std::endl;
		exit(-1);
	}
};

double ImpactStatus::GetUserDefinedValue(unsigned u) const
{
	if (u < uNumUDDImpactStatus)
	{
		return dUDV[u+3*uNumUDDImpactStatus];
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
	if (u < uNumUDDImpactStatus)
	{
		dUDV[u                    ] += d;
		dUDV[u+uNumUDDImpactStatus] += d;
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
		(&dUDV[2*uNumUDDImpactStatus]      ,
		 &dUDV[uNumUDDImpactStatus]        ,
		 uNumUDDImpactStatus*sizeof(double) );

	for(unsigned u=0; u<uNumUDDImpactStatus; u++)
		dUDV[u+uNumUDDImpactStatus] = 0.0;

	return &dUDV[2*uNumUDDImpactStatus];
};

const double* ImpactStatus::RetrieveAllUserDefinedValue() const
{
	return &dUDV[0];
};

void ImpactStatus::SetAllUserDefinedValue(const double* dp)
{
	for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
		dUDV[u] = *(dp+u);
};

void ImpactStatus::CleanAllUserDefinedValue()
{
	for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
		dUDV[u] = 0.0;
};

void ImpactStatus::CleanUserDefinedValue()
{
	for(unsigned u=0; u<uNumUDDImpactStatus; u++)
		dUDV[u+3*uNumUDDImpactStatus] = 0.0;
};

void ImpactStatus::CleanAccumulativeUserDefinedValue()
{
	for(unsigned u=0; u<3*uNumUDDImpactStatus; u++)
		dUDV[u] = 0.0;
};

void ImpactStatus::Clean()
{
	bContact = false;
	bBond    = false;
	dKn      = 0.0;
	CleanShearForce();

	for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
		dUDV[u] = 0.0;
};

};   // namespace vedo
