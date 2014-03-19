#include <vedo/framework/interfaces/ImpactStatus.h>
#include <cstdlib>
#include <cstring>

namespace vedo
{

ImpactStatus::ImpactStatus():
	bContact(false),
	bBond(false),
	dKn(0.0),
	dInitialVelocity(0.0),
	vShearForce(),
	vImpactPoint(),
	vImpactDirection(),
	vImpactToMaster(),
	vAngularImpactToMaster(),
	dOverlap(0.0)
{
	if(uNumUDDImpactStatus != 0)
	{
	    dpUDV = new double[4*uNumUDDImpactStatus];
        for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
            *(dpUDV+u) = 0.0;
	}
	else
	{
		dpUDV = 0;
	}
};

ImpactStatus::ImpactStatus
    (const bool& Contact,
	 const bool& Bond,
	 const double& Kn,
	 const double& InitialVelocity,
	 const njr::Vector3d& ShearForce,
	 const double* UDVp              ):
    bContact(Contact),
    bBond(Bond),
    dKn(Kn),
    dInitialVelocity(InitialVelocity),
    vShearForce(ShearForce),
	vImpactPoint(),
	vImpactDirection(),
	vImpactToMaster(),
	vAngularImpactToMaster(),
	dOverlap(0.0)
{
	if(uNumUDDImpactStatus != 0)
	{
	    dpUDV = new double[4*uNumUDDImpactStatus];
        memcpy(dpUDV, UDVp, 4*uNumUDDImpactStatus*sizeof(double));
	}
	else
	{
		dpUDV = 0;
	}
};

ImpactStatus::ImpactStatus
    (const bool& Contact,
	 const bool& Bond,
	 const double& Kn,
	 const double& InitialVelocity,
     const njr::Vector3d& ShearForce):
    bContact(Contact),
    bBond(Bond),
    dKn(Kn),
    dInitialVelocity(InitialVelocity),
    vShearForce(ShearForce),
	vImpactPoint(),
	vImpactDirection(),
	vImpactToMaster(),
	vAngularImpactToMaster(),
	dOverlap(0.0)
{
	dpUDV = 0;
};

ImpactStatus::ImpactStatus
    (const bool& Contact,
	 const bool& Bond,
	 const double& Kn,
	 const double& InitialVelocity,
	 const njr::Vector3d& ShearForce,
     const njr::Vector3d& ImpactPoint,
     const njr::Vector3d& ImpactDirection,
     const njr::Vector3d& ImpactToMaster,
     const njr::Vector3d& AngularImpactToMaster,
     const double& Overlap,
	 const double* UDVp                          ):
    bContact(Contact),
    bBond(Bond),
    dKn(Kn),
    dInitialVelocity(InitialVelocity),
    vShearForce(ShearForce),
	vImpactPoint(ImpactPoint),
	vImpactDirection(ImpactDirection),
	vImpactToMaster(ImpactToMaster),
	vAngularImpactToMaster(AngularImpactToMaster),
	dOverlap(Overlap)
{
	if(uNumUDDImpactStatus != 0)
	{
	    dpUDV = new double[4*uNumUDDImpactStatus];
        memcpy(dpUDV, UDVp, 4*uNumUDDImpactStatus*sizeof(double));
	}
	else
	{
		dpUDV = 0;
	}
};

ImpactStatus::ImpactStatus
    (const bool& Contact,
	 const bool& Bond,
	 const double& Kn,
	 const double& InitialVelocity,
     const njr::Vector3d& ShearForce,
     const njr::Vector3d& ImpactPoint,
     const njr::Vector3d& ImpactDirection,
     const njr::Vector3d& ImpactToMaster,
     const njr::Vector3d& AngularImpactToMaster,
     const double& Overlap                      ):
    bContact(Contact),
    bBond(Bond),
    dKn(Kn),
    dInitialVelocity(InitialVelocity),
    vShearForce(ShearForce),
	vImpactPoint(ImpactPoint),
	vImpactDirection(ImpactDirection),
	vImpactToMaster(ImpactToMaster),
	vAngularImpactToMaster(AngularImpactToMaster),
	dOverlap(Overlap)
{
	dpUDV = 0;
};

ImpactStatus::ImpactStatus(const ImpactStatus& is)
{
	*this = is;
};

const ImpactStatus& ImpactStatus::operator = (const ImpactStatus& is)
{
	bContact               = is.bContact;
	bBond                  = is.bBond;
	dKn                    = is.dKn;
	dInitialVelocity       = is.dInitialVelocity;
	vShearForce            = is.vShearForce;
	vImpactPoint           = is.vImpactPoint;
	vImpactDirection       = is.vImpactDirection;
	vImpactToMaster        = is.vImpactToMaster;
	vAngularImpactToMaster = is.vAngularImpactToMaster;
	dOverlap               = is.dOverlap;
	if(uNumUDDImpactStatus != 0)
	{
	    dpUDV = new double[uNumUDDImpactStatus*4];
        memcpy(dpUDV, is.dpUDV, 4*uNumUDDImpactStatus*sizeof(double));
	}
	else
	{
		dpUDV = 0;
	}
	return *this;
};

ImpactStatus::~ImpactStatus()
{
	if(uNumUDDImpactStatus != 0)
	{
		delete dpUDV;
	}
};

void ImpactStatus::SetContactInformation(const ContactInfo* cInfo)
{
    vImpactPoint     = cInfo->vImpactPoint;
    vImpactDirection = cInfo->vImpactDirection;
    dOverlap         = cInfo->dImpactDepth;
};

void ImpactStatus::SetImpactInformation
    (const njr::Vector3d& ImpactToMaster,
     const njr::Vector3d& AngularImpactToMaster)
{
    vImpactToMaster        = ImpactToMaster;
    vAngularImpactToMaster = AngularImpactToMaster;
};

void ImpactStatus::CleanShearForce()
{
	vShearForce.Set(0.0, 0.0, 0.0);
};

void ImpactStatus::CleanImpactPoint()
{
	vImpactPoint.Set(0.0, 0.0, 0.0);
};

void ImpactStatus::CleanImpactDirection()
{
	vImpactDirection.Set(0.0, 0.0, 0.0);
};

void ImpactStatus::CleanImpactToMaster()
{
	vImpactToMaster.Set(0.0, 0.0, 0.0);
};

void ImpactStatus::CleanAngularImpactToMaster()
{
	vAngularImpactToMaster.Set(0.0, 0.0, 0.0);
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

void ImpactStatus::CleanContactStatusAndImpact()
{
	bContact = false;
	CleanShearForce();
	CleanImpactToMaster();
	CleanAngularImpactToMaster();
    CleanAllUserDefinedValue();
};

void ImpactStatus::Clean()
{
	bContact         = false;
	bBond            = false;
	dKn              = 0.0;
	dInitialVelocity = 0.0;
    dOverlap         = 0.0;
	CleanShearForce();
	CleanImpactPoint();
	CleanImpactDirection();
	CleanImpactToMaster();
	CleanAngularImpactToMaster();
    CleanAllUserDefinedValue();
};

};   // namespace vedo
