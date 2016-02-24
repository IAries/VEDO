#ifndef _IMPACT_STATUS_H
#define _IMPACT_STATUS_H

#include <aries/utility/Constants.h>
#include <vedo/framework/ContactDetector.h>
#include <vedo/framework/DataType.h>

namespace vedo
{

class ImpactStatus
{

public:

	ImpactStatus();

	ImpactStatus
        (const bool& Contact, const bool& Bond, const _float_t& Kn, const _float_t& InitialVelocity,
         const Vector3df& ShearForce, const _float_t* UDVp                                          );

	ImpactStatus
        (const bool& Contact, const bool& Bond, const _float_t& Kn, const _float_t& InitialVelocity,
		 const Vector3df& ShearForce                                                                    );

	ImpactStatus
        (const bool& Contact, const bool& Bond, const _float_t& Kn, const _float_t& InitialVelocity,
         const Vector3df& ShearForce, const Vector3df& ImpactPoint, const Vector3df& ImpactDirection,
         const Vector3df& ImpactToMaster, const Vector3df& AngularImpactToMaster,
         const _float_t& Overlap, const _float_t* UDVp                                                   );

	ImpactStatus
        (const bool& Contact, const bool& Bond, const _float_t& Kn, const _float_t& InitialVelocity,
		 const Vector3df& ShearForce, const Vector3df& ImpactPoint, const Vector3df& ImpactDirection,
         const Vector3df& ImpactToMaster, const Vector3df& AngularImpactToMaster,
         const _float_t& Overlap                                                                             );

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

	inline _float_t Kn() const
	{
		return dKn;
	}

	inline void SetKn(_float_t& dK)
	{
		dKn = dK;
	}

	inline _float_t InitialVelocity() const
	{
		return dInitialVelocity;
	}

	inline void SetInitialVelocity(_float_t& dV)
	{
		dInitialVelocity = dV;
	}

	inline Vector3df ShearForce() const
	{
		return vShearForce;
	}

	inline void SetShearForce(Vector3df& vS)
	{
		vShearForce = vS;
	}

	void CleanShearForce();

	inline Vector3df ImpactPoint() const
	{
		return vImpactPoint;
	}

	inline void SetImpactPoint(Vector3df& vI)
	{
		vImpactPoint = vI;
	}

	void CleanImpactPoint();

	inline Vector3df ImpactDirection() const
	{
		return vImpactDirection;
	}

	inline void SetImpactDirection(Vector3df& vI)
	{
		vImpactDirection = vI;
	}

	void CleanImpactDirection();

	inline Vector3df ImpactToMaster() const
	{
		return vImpactToMaster;
	}

	inline void SetImpactToMaster(Vector3df& vI)
	{
		vImpactToMaster = vI;
	}

	void CleanImpactToMaster();

	inline Vector3df AngularImpactToMaster() const
	{
		return vAngularImpactToMaster;
	}

	inline void SetAngularImpactToMaster(Vector3df& vA)
	{
		vAngularImpactToMaster = vA;
	}

	void CleanAngularImpactToMaster();

	inline _float_t Overlap() const
	{
		return dOverlap;
	}

	inline void SetOverlap(_float_t& dO)
	{
		dOverlap = dO;
	}

	inline void Backup()
	{
		bLastContact = bContact;
		bLastBond    = bBond;
	}

    void SetContactInformation(const ContactInfo*);

    void SetImpactInformation(const Vector3df& ImpactToMaster, const Vector3df& AngularImpactToMaster);

	void SetUserDefinedValue(_uint_t u, _float_t d);

	_float_t GetUserDefinedValue(_uint_t u) const;

	void AddAccumulativeUserDefinedValue(_uint_t u, _float_t d);

	const _float_t* RetrieveUserDefinedValue();

	const _float_t* RetrieveAllUserDefinedValue() const;

	void SetAllUserDefinedValue(const _float_t*);

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

	_float_t  dKn;                      // Stiffness of normal spring

	_float_t  dInitialVelocity;         // Initial velocity

	Vector3df vShearForce;              // Shear force

	Vector3df vImpactPoint;             // Impact point

	Vector3df vImpactDirection;         // Impact direction

	Vector3df vImpactToMaster;          // Impact to master

	Vector3df vAngularImpactToMaster;   // Angular impact to master

    _float_t  dOverlap;                 // Overlap

	_float_t* dpUDV;                    // User-defined value

//	_float_t        dUDV[uNumUDDImpactStatus*4];   // User-defined value
                                                 // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value
                                                 // uNumUDDImpactStatus ~ 2*uNumUDDImpactStatus-1: Unsynchronized part of Accumulative user-defined value
                                                 // 2*uNumUDDImpactStatus ~ 3*uNumUDDImpactStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                                 // 3*uNumUDDImpactStatus ~ 4*uNumUDDImpactStatus-1: User-defined value
};

}   // namespace vedo

#endif // _IMPACT_STATUS_H
