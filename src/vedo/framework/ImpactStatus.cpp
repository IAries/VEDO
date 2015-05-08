#include <vedo/framework/ImpactStatus.h>
#include <cstdlib>
#include <cstring>

namespace vedo
{

ImpactStatus::ImpactStatus():
	bContact(false), bLastContact(true), bBond(false), bLastBond(true),
	dOverlap(0.0), dKn(0.0), dInitialVelocity(0.0),
	vShearForce(), vImpactPoint(), vImpactDirection(), vImpactToMaster(), vAngularImpactToMaster()
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	if (uNumUDDImpactStatus != 0)
	{
	    dpUDV = new _float_t[4*uNumUDDImpactStatus];
        for (_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
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
    (const bool& Contact, const bool& Bond, const _float_t& Kn,
     const _float_t& InitialVelocity, const Vector3df& ShearForce, const _float_t* UDVp):
		bContact(Contact), bLastContact(Contact), bBond(Bond), bLastBond(Bond),
		dOverlap(0.0), dKn(Kn), dInitialVelocity(InitialVelocity),
		vShearForce(ShearForce), vImpactPoint(), vImpactDirection(), vImpactToMaster(), vAngularImpactToMaster()
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	if (uNumUDDImpactStatus != 0)
	{
	    dpUDV = new _float_t[4*uNumUDDImpactStatus];
        memcpy(dpUDV, UDVp, 4*uNumUDDImpactStatus*sizeof(_float_t));
	}
	else
	{
		dpUDV = 0;
	}
}

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond,
     const _float_t& Kn, const _float_t& InitialVelocity, const Vector3df& ShearForce):
    	bContact(Contact), bLastContact(Contact), bBond(Bond), bLastBond(Bond),
    	dOverlap(0.0), dKn(Kn), dInitialVelocity(InitialVelocity),
    	vShearForce(ShearForce), vImpactPoint(), vImpactDirection(), vImpactToMaster(), vAngularImpactToMaster()
{
	dpUDV = 0;
}

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond, const _float_t& Kn,
	 const _float_t& InitialVelocity, const Vector3df& ShearForce,
	 const Vector3df& ImpactPoint, const Vector3df& ImpactDirection,
	 const Vector3df& ImpactToMaster, const Vector3df& AngularImpactToMaster,
     const _float_t& Overlap, const _float_t* UDVp                           ):
    	bContact(Contact), bLastContact(Contact), bBond(Bond), bLastBond(Bond),
    	dOverlap(Overlap), dKn(Kn), dInitialVelocity(InitialVelocity),
    	vShearForce(ShearForce), vImpactPoint(ImpactPoint), vImpactDirection(ImpactDirection),
    	vImpactToMaster(ImpactToMaster), vAngularImpactToMaster(AngularImpactToMaster)
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	if (uNumUDDImpactStatus != 0)
	{
	    dpUDV = new _float_t[4*uNumUDDImpactStatus];
        memcpy(dpUDV, UDVp, 4*uNumUDDImpactStatus*sizeof(_float_t));
	}
	else
	{
		dpUDV = 0;
	}
}

ImpactStatus::ImpactStatus
    (const bool& Contact, const bool& Bond, const _float_t& Kn, const _float_t& InitialVelocity,
	 const Vector3df& ShearForce, const Vector3df& ImpactPoint, const Vector3df& ImpactDirection,
     const Vector3df& ImpactToMaster, const Vector3df& AngularImpactToMaster, const _float_t& Overlap):
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

	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
	if (uNumUDDImpactStatus != 0)
	{
		dpUDV = new _float_t[uNumUDDImpactStatus*4];
		memcpy(dpUDV, is.dpUDV, 4*uNumUDDImpactStatus*sizeof(_float_t));
	}
	else
	{
		dpUDV = 0;
	}
	return *this;
}

ImpactStatus::~ImpactStatus()
{
	if (aries::Constants::Instance()->NumUDDImpactStatus() != 0)
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

void ImpactStatus::SetImpactInformation (const Vector3df& ImpactToMaster, const Vector3df& AngularImpactToMaster)
{
	vImpactToMaster        = ImpactToMaster;
	vAngularImpactToMaster = AngularImpactToMaster;
}

void ImpactStatus::CleanShearForce()
{
	vShearForce.set(0.0, 0.0, 0.0);
}

void ImpactStatus::CleanImpactPoint()
{
	vImpactPoint.set(0.0, 0.0, 0.0);
}

void ImpactStatus::CleanImpactDirection()
{
	vImpactDirection.set(0.0, 0.0, 0.0);
}

void ImpactStatus::CleanImpactToMaster()
{
	vImpactToMaster.set(0.0, 0.0, 0.0);
}

void ImpactStatus::CleanAngularImpactToMaster()
{
	vAngularImpactToMaster.set(0.0, 0.0, 0.0);
}

void ImpactStatus::SetUserDefinedValue(_uint_t u, _float_t d)
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
	if (u < uNumUDDImpactStatus)
	{
		*(dpUDV+u+3*uNumUDDImpactStatus)= d;
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::SetUserDefinedValue(_uint_t, _float_t)" << std::endl;
		exit(-1);
	}
}

_float_t ImpactStatus::GetUserDefinedValue(_uint_t u) const
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
	if (u < uNumUDDImpactStatus)
	{
		return *(dpUDV+u+3*uNumUDDImpactStatus);
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::GetUserDefinedValue(_uint_t)" << std::endl;
		exit(-1);
	}
}

void ImpactStatus::AddAccumulativeUserDefinedValue(_uint_t u, _float_t d)
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
	if (u < uNumUDDImpactStatus)
	{
		*(dpUDV+u)                     += d;
		*(dpUDV+u+uNumUDDImpactStatus) += d;
	}
	else
	{
		std::cerr << "Error!! Code: ImpactStatus::AddAccumulativeUserDefinedValue(_uint_t, _float_t)" << std::endl;
		exit(-1);
	}
}

const _float_t* ImpactStatus::RetrieveUserDefinedValue()
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	memcpy(dpUDV+2*uNumUDDImpactStatus, dpUDV+uNumUDDImpactStatus, uNumUDDImpactStatus*sizeof(_float_t));

	for(_uint_t u=0; u<uNumUDDImpactStatus; u++)
	{
		*(dpUDV+u+uNumUDDImpactStatus) = 0.0;
	}

	return dpUDV+2*uNumUDDImpactStatus;
}

const _float_t* ImpactStatus::RetrieveAllUserDefinedValue() const
{
	return dpUDV;
}

void ImpactStatus::SetAllUserDefinedValue(const _float_t* dp)
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
    memcpy(dpUDV, dp, 4*uNumUDDImpactStatus*sizeof(_float_t));
}

void ImpactStatus::CleanAllUserDefinedValue()
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
    for (_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
    {
        *(dpUDV+u) = 0.0;
    }
}

void ImpactStatus::CleanUserDefinedValue()
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
    for (_uint_t u=0; u<uNumUDDImpactStatus; u++)
    {
		*(dpUDV+u+3*uNumUDDImpactStatus) = 0.0;
	}
}

void ImpactStatus::CleanAccumulativeUserDefinedValue()
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();
	for (_uint_t u=0; u<3*uNumUDDImpactStatus; u++)
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
