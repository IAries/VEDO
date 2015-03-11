#ifndef _IMPACT_STATUS_H
#define _IMPACT_STATUS_H

#include <vedo/constants/interfaces/Constants.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/ContactDetector.h>

namespace vedo
{

class ImpactStatus
{

public:

	ImpactStatus();

	ImpactStatus
        (const bool& Contact, const bool& Bond, const vedo_float_t& Kn, const vedo_float_t& InitialVelocity,
         const njr::Vector3d& ShearForce, const vedo_float_t* UDVp                                          );

	ImpactStatus
        (const bool& Contact, const bool& Bond, const vedo_float_t& Kn, const vedo_float_t& InitialVelocity,
		 const njr::Vector3d& ShearForce                                                                    );

	ImpactStatus
        (const bool& Contact, const bool& Bond, const vedo_float_t& Kn, const vedo_float_t& InitialVelocity,
         const njr::Vector3d& ShearForce, const njr::Vector3d& ImpactPoint, const njr::Vector3d& ImpactDirection,
         const njr::Vector3d& ImpactToMaster, const njr::Vector3d& AngularImpactToMaster,
         const vedo_float_t& Overlap, const vedo_float_t* UDVp                                                   );

	ImpactStatus
        (const bool& Contact, const bool& Bond, const vedo_float_t& Kn, const vedo_float_t& InitialVelocity,
		 const njr::Vector3d& ShearForce, const njr::Vector3d& ImpactPoint, const njr::Vector3d& ImpactDirection,
         const njr::Vector3d& ImpactToMaster, const njr::Vector3d& AngularImpactToMaster,
         const vedo_float_t& Overlap                                                                             );

	ImpactStatus(const ImpactStatus&);

	const ImpactStatus& operator = (const ImpactStatus&);

	~ImpactStatus();

	inline bool Contact() const
	{
		return bContact;
	}

	inline bool LastContact() const
	{
		return bLastContact;
	}

	inline void SetContact(bool& b)
	{
		bContact = b;
	}

	inline void SetLastContact(bool& b)
	{
		bLastContact = b;
	}

	inline bool Bond() const
	{
		return bBond;
	}

	inline bool LastBond() const
	{
		return bLastBond;
	}

	inline void SetBond(bool& b)
	{
		bBond = b;
	}

	inline void SetLastBond(bool& b)
	{
		bLastBond = b;
	}

	inline vedo_float_t Kn() const
	{
		return dKn;
	}

	inline void SetKn(vedo_float_t& dK)
	{
		dKn = dK;
	}

	inline vedo_float_t InitialVelocity() const
	{
		return dInitialVelocity;
	}

	inline void SetInitialVelocity(vedo_float_t& dV)
	{
		dInitialVelocity = dV;
	}

	inline njr::Vector3d ShearForce() const
	{
		return vShearForce;
	}

	inline void SetShearForce(njr::Vector3d& vS)
	{
		vShearForce = vS;
	}

	void CleanShearForce();

	inline njr::Vector3d ImpactPoint() const
	{
		return vImpactPoint;
	}

	inline void SetImpactPoint(njr::Vector3d& vI)
	{
		vImpactPoint = vI;
	}

	void CleanImpactPoint();

	inline njr::Vector3d ImpactDirection() const
	{
		return vImpactDirection;
	}

	inline void SetImpactDirection(njr::Vector3d& vI)
	{
		vImpactDirection = vI;
	}

	void CleanImpactDirection();

	inline njr::Vector3d ImpactToMaster() const
	{
		return vImpactToMaster;
	}

	inline void SetImpactToMaster(njr::Vector3d& vI)
	{
		vImpactToMaster = vI;
	}

	void CleanImpactToMaster();

	inline njr::Vector3d AngularImpactToMaster() const
	{
		return vAngularImpactToMaster;
	}

	inline void SetAngularImpactToMaster(njr::Vector3d& vA)
	{
		vAngularImpactToMaster = vA;
	}

	void CleanAngularImpactToMaster();

	inline vedo_float_t Overlap() const
	{
		return dOverlap;
	}

	inline void SetOverlap(vedo_float_t& dO)
	{
		dOverlap = dO;
	}

	inline void Backup()
	{
		bLastContact = bContact;
		bLastBond    = bBond;
	}

    void SetContactInformation(const ContactInfo*);

    void SetImpactInformation(const njr::Vector3d& ImpactToMaster, const njr::Vector3d& AngularImpactToMaster);

	void SetUserDefinedValue(vedo_uint_t u, vedo_float_t d);

	vedo_float_t GetUserDefinedValue(vedo_uint_t u) const;

	void AddAccumulativeUserDefinedValue(vedo_uint_t u, vedo_float_t d);

	const vedo_float_t* RetrieveUserDefinedValue();

	const vedo_float_t* RetrieveAllUserDefinedValue() const;

	void SetAllUserDefinedValue(const vedo_float_t*);

	void CleanAllUserDefinedValue();

	void CleanUserDefinedValue();

	void CleanAccumulativeUserDefinedValue();

	void CleanContactStatusAndImpact();

	void Clean();

private:

	bool          bContact;                 // Contacted or not

	bool          bLastContact;             // Contacted or not at last step

	bool          bBond;                    // Bond exists or not at last step

	bool          bLastBond;                // Bond exists or not

	vedo_float_t  dKn;                      // Stiffness of normal spring

	vedo_float_t  dInitialVelocity;         // Initial velocity

	njr::Vector3d vShearForce;              // Shear force

	njr::Vector3d vImpactPoint;             // Impact point

	njr::Vector3d vImpactDirection;         // Impact direction

	njr::Vector3d vImpactToMaster;          // Impact to master

	njr::Vector3d vAngularImpactToMaster;   // Angular impact to master

    vedo_float_t  dOverlap;                 // Overlap

	vedo_float_t* dpUDV;                    // User-defined value

//	vedo_float_t        dUDV[uNumUDDImpactStatus*4];   // User-defined value
                                                 // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value
                                                 // uNumUDDImpactStatus ~ 2*uNumUDDImpactStatus-1: Unsynchronized part of Accumulative user-defined value
                                                 // 2*uNumUDDImpactStatus ~ 3*uNumUDDImpactStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                                 // 3*uNumUDDImpactStatus ~ 4*uNumUDDImpactStatus-1: User-defined value
};

}   // namespace vedo

#endif // _IMPACT_STATUS_H
