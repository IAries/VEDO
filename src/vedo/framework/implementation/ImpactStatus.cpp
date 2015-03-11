#include <vedo/framework/interfaces/ImpactStatus.h>
#include <cstdlib>
#include <cstring>

namespace vedo
{

ImpactStatus::ImpactStatus():
	bContact(false), bLastContact(true), bBond(false), bLastBond(true),
	dOverlap(0.0), dKn(0.0), dInitialVelocity(0.0),
	vShearForce(), vImpactPoint(), vImpactDirection(), vImpactToMaster(), vAngularImpactToMaster()
{
	if (uNumUDDImpactStatus != 0)
	{
	    dpUDV = new vedo_float_t[4*uNumUDDImpactStatus];
        for (vedo_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
        {
            *(dpUDV+u) = 0.0;
		}
	}
	else
	{
		dpUDV = 0;
	}
}

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond, const vedo_float_t& Kn,
     const vedo_float_t& InitialVelocity, const njr::Vector3d& ShearForce, const vedo_float_t* UDVp):
		bContact(Contact), bLastContact(Contact), bBond(Bond), bLastBond(Bond),
		dOverlap(0.0), dKn(Kn), dInitialVelocity(InitialVelocity),
		vShearForce(ShearForce), vImpactPoint(), vImpactDirection(), vImpactToMaster(), vAngularImpactToMaster()
{
	if (uNumUDDImpactStatus != 0)
	{
	    dpUDV = new vedo_float_t[4*uNumUDDImpactStatus];
        memcpy(dpUDV, UDVp, 4*uNumUDDImpactStatus*sizeof(vedo_float_t));
	}
	else
	{
		dpUDV = 0;
	}
}

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond,
     const vedo_float_t& Kn, const vedo_float_t& InitialVelocity, const njr::Vector3d& ShearForce):
    	bContact(Contact), bLastContact(Contact), bBond(Bond), bLastBond(Bond),
    	dOverlap(0.0), dKn(Kn), dInitialVelocity(InitialVelocity),
    	vShearForce(ShearForce), vImpactPoint(), vImpactDirection(), vImpactToMaster(), vAngularImpactToMaster()
{
	dpUDV = 0;
}

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond, const vedo_float_t& Kn,
	 const vedo_float_t& InitialVelocity, const njr::Vector3d& ShearForce,
	 const njr::Vector3d& ImpactPoint, const njr::Vector3d& ImpactDirection,
	 const njr::Vector3d& ImpactToMaster, const njr::Vector3d& AngularImpactToMaster,
     const vedo_float_t& Overlap, const vedo_float_t* UDVp                           ):
    	bContact(Contact), bLastContact(Contact), bBond(Bond), bLastBond(Bond),
    	dOverlap(Overlap), dKn(Kn), dInitialVelocity(InitialVelocity),
    	vShearForce(ShearForce), vImpactPoint(ImpactPoint), vImpactDirection(ImpactDirection),
    	vImpactToMaster(ImpactToMaster), vAngularImpactToMaster(AngularImpactToMaster)
{
	if (uNumUDDImpactStatus != 0)
	{
	    dpUDV = new vedo_float_t[4*uNumUDDImpactStatus];
        memcpy(dpUDV, UDVp, 4*uNumUDDImpactStatus*sizeof(vedo_float_t));
	}
	else
	{
		dpUDV = 0;
	}
}

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond, const vedo_float_t& Kn, const vedo_float_t& InitialVelocity,
	 const njr::Vector3d& ShearForce, const njr::Vector3d& ImpactPoint, const njr::Vector3d& ImpactDirection,
     const njr::Vector3d& ImpactToMaster, const njr::Vector3d& AngularImpactToMaster, const vedo_float_t& Overlap):
    	bContact(Contact), bLastContact(Contact), bBond(Bond), bLastBond(Bond),
    	dOverlap(Overlap), dKn(Kn), dInitialVelocity(InitialVelocity),
    	vShearForce(ShearForce), vImpactPoint(ImpactPoint), vImpactDirection(ImpactDirection),
    	vImpactToMaster(ImpactToMaster), vAngularImpactToMaster(AngularImpactToMaster)
{
	dpUDV = 0;
}

ImpactStatus::ImpactStatus(const ImpactStatus& is)
{
	*this = is;
}

const ImpactStatus& ImpactStatus::operator = (const ImpactStatus& is)
{
	bContact               = is.bContact;
	bLastContact           = is.bLastContact;
	bBond                  = is.bBond;
	bLastBond              = is.bLastBond;
	dKn                    = is.dKn;
	dInitialVelocity       = is.dInitialVelocity;
	vShearForce            = is.vShearForce;
	vImpactPoint           = is.vImpactPoint;
	vImpactDirection       = is.vImpactDirection;
	vImpactToMaster        = is.vImpactToMaster;
	vAngularImpactToMaster = is.vAngularImpactToMaster;
	dOverlap               = is.dOverlap;
	if (uNumUDDImpactStatus != 0)
	{
		dpUDV = new vedo_float_t[uNumUDDImpactStatus*4];
		memcpy(dpUDV, is.dpUDV, 4*uNumUDDImpactStatus*sizeof(vedo_float_t));
	}
	else
	{
		dpUDV = 0;
	}
	return *this;
}

ImpactStatus::~ImpactStatus()
{
	if (uNumUDDImpactStatus != 0)
	{
		delete dpUDV;
	}
}

void ImpactStatus::SetContactInformation(const ContactInfo* cInfo)
{
    vImpactPoint     = cInfo->vImpactPoint;
    vImpactDirection = cInfo->vImpactDirection;
    dOverlap         = cInfo->dImpactDepth;
}

void ImpactStatus::SetImpactInformation (const njr::Vector3d& ImpactToMaster, const njr::Vector3d& AngularImpactToMaster)
{
	vImpactToMaster        = ImpactToMaster;
	vAngularImpactToMaster = AngularImpactToMaster;
}

void ImpactStatus::CleanShearForce()
{
	vShearForce.Set(0.0, 0.0, 0.0);
}

void ImpactStatus::CleanImpactPoint()
{
	vImpactPoint.Set(0.0, 0.0, 0.0);
}

void ImpactStatus::CleanImpactDirection()
{
	vImpactDirection.Set(0.0, 0.0, 0.0);
}

void ImpactStatus::CleanImpactToMaster()
{
	vImpactToMaster.Set(0.0, 0.0, 0.0);
}

void ImpactStatus::CleanAngularImpactToMaster()
{
	vAngularImpactToMaster.Set(0.0, 0.0, 0.0);
}

void ImpactStatus::SetUserDefinedValue(vedo_uint_t u, vedo_float_t d)
{
	if (u < uNumUDDImpactStatus)
	{
		*(dpUDV+u+3*uNumUDDImpactStatus)= d;
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::SetUserDefinedValue(vedo_uint_t, vedo_float_t)" << std::endl;
		exit(-1);
	}
}

vedo_float_t ImpactStatus::GetUserDefinedValue(vedo_uint_t u) const
{
	if (u < uNumUDDImpactStatus)
	{
		return *(dpUDV+u+3*uNumUDDImpactStatus);
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::GetUserDefinedValue(vedo_uint_t)" << std::endl;
		exit(-1);
	}
}

void ImpactStatus::AddAccumulativeUserDefinedValue(vedo_uint_t u, vedo_float_t d)
{
	if (u < uNumUDDImpactStatus)
	{
		*(dpUDV+u)                     += d;
		*(dpUDV+u+uNumUDDImpactStatus) += d;
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::AddAccumulativeUserDefinedValue(vedo_uint_t, vedo_float_t)" << std::endl;
		exit(-1);
	}
}

const vedo_float_t* ImpactStatus::RetrieveUserDefinedValue()
{
	memcpy(dpUDV+2*uNumUDDImpactStatus, dpUDV+uNumUDDImpactStatus, uNumUDDImpactStatus*sizeof(vedo_float_t));

	for(vedo_uint_t u=0; u<uNumUDDImpactStatus; u++)
	{
		*(dpUDV+u+uNumUDDImpactStatus) = 0.0;
	}

	return dpUDV+2*uNumUDDImpactStatus;
}

const vedo_float_t* ImpactStatus::RetrieveAllUserDefinedValue() const
{
	return dpUDV;
}

void ImpactStatus::SetAllUserDefinedValue(const vedo_float_t* dp)
{
    memcpy(dpUDV, dp, 4*uNumUDDImpactStatus*sizeof(vedo_float_t));
}

void ImpactStatus::CleanAllUserDefinedValue()
{
    for (vedo_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
    {
        *(dpUDV+u) = 0.0;
    }
}

void ImpactStatus::CleanUserDefinedValue()
{
    for (vedo_uint_t u=0; u<uNumUDDImpactStatus; u++)
    {
		*(dpUDV+u+3*uNumUDDImpactStatus) = 0.0;
	}
}

void ImpactStatus::CleanAccumulativeUserDefinedValue()
{
	for (vedo_uint_t u=0; u<3*uNumUDDImpactStatus; u++)
	{
		*(dpUDV+u) = 0.0;
	}
}

void ImpactStatus::CleanContactStatusAndImpact()
{
	bLastContact = bContact;
	bContact     = false;
	bLastBond    = bBond;
	bBond        = false;
	CleanShearForce();
	CleanImpactToMaster();
	CleanAngularImpactToMaster();
    CleanAllUserDefinedValue();
}

void ImpactStatus::Clean()
{
	bLastContact     = false;
	bContact         = false;
	bLastBond        = false;
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
}

}   // namespace vedo
