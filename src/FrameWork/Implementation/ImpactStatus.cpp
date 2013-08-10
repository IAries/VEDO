#include <Framework/Interfaces/ImpactStatus.h>
#include <cstdlib>
#include <cstring>

namespace vedo
{

ImpactStatus::ImpactStatus(): bContact(false), bBond(false), dKn(0.0), vShearForce()
{
	if(uNumUDDImpactStatus != 0)
	{
	    dpUDV = new double[4*uNumUDDImpactStatus];
        for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
            *(dpUDV+u) = 0.0;
	}
};

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond, const double& Kn,
     const njr::Vector3d& ShearForce, const double* UDVp       ):
    bContact(Contact), bBond(Bond),dKn(Kn), vShearForce(ShearForce)
{
	if(uNumUDDImpactStatus != 0)
	{
	    dpUDV = new double[4*uNumUDDImpactStatus];
        memcpy(dpUDV, UDVp, 4*uNumUDDImpactStatus*sizeof(double));
	}
};

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond, const double& Kn,
     const njr::Vector3d& ShearForce                         ):
    bContact(Contact), bBond(Bond),dKn(Kn), vShearForce(ShearForce)
{
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
	if(uNumUDDImpactStatus != 0)
	{
	    dpUDV = new double[uNumUDDImpactStatus*4];
        memcpy(dpUDV, is.dpUDV, 4*uNumUDDImpactStatus*sizeof(double));
	}
	return *this;
};

ImpactStatus::~ImpactStatus()
{
    delete dpUDV;
};

void ImpactStatus::CleanShearForce()
{
	vShearForce.Set(0.0, 0.0, 0.0);
};

void ImpactStatus::SetUserDefinedValue(unsigned u, double d)
{
	if (u < uNumUDDImpactStatus)
	{
		*(dpUDV+u+3*uNumUDDImpactStatus)= d;
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
		return *(dpUDV+u+3*uNumUDDImpactStatus);
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
		*(dpUDV+u)                     += d;
		*(dpUDV+u+uNumUDDImpactStatus) += d;
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
		(dpUDV+2*uNumUDDImpactStatus       ,
		 dpUDV+uNumUDDImpactStatus         ,
		 uNumUDDImpactStatus*sizeof(double) );

	for(unsigned u=0; u<uNumUDDImpactStatus; u++)
		*(dpUDV+u+uNumUDDImpactStatus) = 0.0;

	return dpUDV+2*uNumUDDImpactStatus;
};

const double* ImpactStatus::RetrieveAllUserDefinedValue() const
{
	return dpUDV;
};

void ImpactStatus::SetAllUserDefinedValue(const double* dp)
{
    memcpy(dpUDV, dp, 4*uNumUDDImpactStatus*sizeof(double));
};

void ImpactStatus::CleanAllUserDefinedValue()
{
    for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
        *(dpUDV+u) = 0.0;
};

void ImpactStatus::CleanUserDefinedValue()
{
    for(unsigned u=0; u<uNumUDDImpactStatus; u++)
		*(dpUDV+u+3*uNumUDDImpactStatus) = 0.0;
};

void ImpactStatus::CleanAccumulativeUserDefinedValue()
{
	for(unsigned u=0; u<3*uNumUDDImpactStatus; u++)
		*(dpUDV+u) = 0.0;
};

void ImpactStatus::Clean()
{
	bContact = false;
	bBond    = false;
	dKn      = 0.0;
	CleanShearForce();
    CleanAllUserDefinedValue();
};

};   // namespace vedo
