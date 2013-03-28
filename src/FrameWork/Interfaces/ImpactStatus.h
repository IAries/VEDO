#ifndef _IMPACT_STATUS_H
#define _IMPACT_STATUS_H

#include <NJR/Interfaces/Vector3d.h>
#include <FrameWork/Interfaces/Constants.h>

namespace VEDO
{

class ImpactStatus
{

public:

	ImpactStatus();

	ImpactStatus
        (const bool& Contact, const bool& Bond, const double& Kn,
         const NJR::Vector3d& ShearForce, const double* UDVp       );

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

	inline NJR::Vector3d ShearForce() const
	{
		return vShearForce;
	};

	inline void SetShearForce(NJR::Vector3d& vS)
	{
		vShearForce = vS;
	};

	void CleanShearForce();

	void SetUserDefinedValue(unsigned u, double d);

	double GetUserDefinedValue(unsigned u) const;

	void AddAccumulativeUserDefinedValue(unsigned u, double d);

	const double* RetrieveUserDefinedValue();

	const double* RetrieveAllUserDefinedValue() const;

	void SetAllUserDefinedValue(const double*);

	void CleanAllUserDefinedValue();

	void CleanUserDefinedValue();

	void CleanAccumulativeUserDefinedValue();

	void Clean();

private:

	bool          bContact;                      // Contacted or not
	bool          bBond;                         // Bond exists or not
	double        dKn;                           // Stiffness of normal spring
	NJR::Vector3d vShearForce;                   // Shear force
	double        dUDV[uNumUDDImpactStatus*4];   // User-defined value
                                                 // 0 ~ uNumUDDImpactStatus: Accumulative user-defined value
                                                 // uNumUDDImpactStatus ~ 2*uNumUDDImpactStatus-1: Unsynchronized part of Accumulative user-defined value
                                                 // 2*uNumUDDImpactStatus ~ 3*uNumUDDImpactStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                                 // 3*uNumUDDImpactStatus ~ 4*uNumUDDImpactStatus-1: User-defined value
};

};   // namespace VEDO

#endif // _IMPACT_STATUS_H
