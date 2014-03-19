#ifndef _IMPACT_STATUS_H
#define _IMPACT_STATUS_H

#include <vedo/Constants.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/ContactDetector.h>

namespace vedo
{

class ImpactStatus
{

public:

	ImpactStatus();

	ImpactStatus
        (const bool& Contact,
		 const bool& Bond,
		 const double& Kn,
		 const double& InitialVelocity,
         const njr::Vector3d& ShearForce,
         const double* UDVp              );

	ImpactStatus
        (const bool& Contact,
		 const bool& Bond,
		 const double& Kn,
		 const double& InitialVelocity,
		 const njr::Vector3d& ShearForce);

	ImpactStatus
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
         const double* UDVp                         );

	ImpactStatus
        (const bool& Contact,
		 const bool& Bond,
		 const double& Kn,
		 const double& InitialVelocity,
		 const njr::Vector3d& ShearForce,
         const njr::Vector3d& ImpactPoint,
         const njr::Vector3d& ImpactDirection,
         const njr::Vector3d& ImpactToMaster,
         const njr::Vector3d& AngularImpactToMaster,
         const double& Overlap                      );

	ImpactStatus(const ImpactStatus&);

	const ImpactStatus& operator = (const ImpactStatus&);

	~ImpactStatus();

	inline bool Contact() const
	{
		return bContact;
	};

	inline void SetContact(bool& b)
	{
		bContact = b;
	};

	inline bool Bond() const
	{
		return bBond;
	};

	inline void SetBond(bool& b)
	{
		bBond = b;
	};

	inline double Kn() const
	{
		return dKn;
	};

	inline void SetKn(double& dK)
	{
		dKn = dK;
	};

	inline double InitialVelocity() const
	{
		return dInitialVelocity;
	};

	inline void SetInitialVelocity(double& dV)
	{
		dInitialVelocity = dV;
	};

	inline njr::Vector3d ShearForce() const
	{
		return vShearForce;
	};

	inline void SetShearForce(njr::Vector3d& vS)
	{
		vShearForce = vS;
	};

	void CleanShearForce();

	inline njr::Vector3d ImpactPoint() const
	{
		return vImpactPoint;
	};

	inline void SetImpactPoint(njr::Vector3d& vI)
	{
		vImpactPoint = vI;
	};

	void CleanImpactPoint();

	inline njr::Vector3d ImpactDirection() const
	{
		return vImpactDirection;
	};

	inline void SetImpactDirection(njr::Vector3d& vI)
	{
		vImpactDirection = vI;
	};

	void CleanImpactDirection();

	inline njr::Vector3d ImpactToMaster() const
	{
		return vImpactToMaster;
	};

	inline void SetImpactToMaster(njr::Vector3d& vI)
	{
		vImpactToMaster = vI;
	};

	void CleanImpactToMaster();

	inline njr::Vector3d AngularImpactToMaster() const
	{
		return vAngularImpactToMaster;
	};

	inline void SetAngularImpactToMaster(njr::Vector3d& vA)
	{
		vAngularImpactToMaster = vA;
	};

	void CleanAngularImpactToMaster();

	inline double Overlap() const
	{
		return dOverlap;
	};

	inline void SetOverlap(double& dO)
	{
		dOverlap = dO;
	};

    void SetContactInformation(const ContactInfo*);

    void SetImpactInformation
        (const njr::Vector3d& ImpactToMaster, const njr::Vector3d& AngularImpactToMaster);

	void SetUserDefinedValue(unsigned u, double d);

	double GetUserDefinedValue(unsigned u) const;

	void AddAccumulativeUserDefinedValue(unsigned u, double d);

	const double* RetrieveUserDefinedValue();

	const double* RetrieveAllUserDefinedValue() const;

	void SetAllUserDefinedValue(const double*);

	void CleanAllUserDefinedValue();

	void CleanUserDefinedValue();

	void CleanAccumulativeUserDefinedValue();

	void CleanContactStatusAndImpact();

	void Clean();

private:

	bool          bContact;                 // Contacted or not
	bool          bBond;                    // Bond exists or not
	double        dKn;                      // Stiffness of normal spring
	double        dInitialVelocity;         // Initial velocity
	njr::Vector3d vShearForce;              // Shear force
	njr::Vector3d vImpactPoint;             // Impact point
	njr::Vector3d vImpactDirection;         // Impact direction
	njr::Vector3d vImpactToMaster;          // Impact to master
	njr::Vector3d vAngularImpactToMaster;   // Angular impact to master
    double        dOverlap;                 // Overlap
	double*       dpUDV;                    // User-defined value
//	double        dUDV[uNumUDDImpactStatus*4];   // User-defined value
                                                 // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value
                                                 // uNumUDDImpactStatus ~ 2*uNumUDDImpactStatus-1: Unsynchronized part of Accumulative user-defined value
                                                 // 2*uNumUDDImpactStatus ~ 3*uNumUDDImpactStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                                 // 3*uNumUDDImpactStatus ~ 4*uNumUDDImpactStatus-1: User-defined value
};

};   // namespace vedo

#endif // _IMPACT_STATUS_H
