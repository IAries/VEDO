#ifndef _IMPACT_STATUS_H
#define _IMPACT_STATUS_H

#include <FrameWork/Interfaces/Constants.h>
#include <NJR/Interfaces/vector3d.h>

class ImpactStatus
{

public:

	ImpactStatus();

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

	inline NJRvector3d ShearForce() const
	{
		return vShearForce;
	};

	inline void SetShearForce(NJRvector3d& vS)
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

	bool bContact;             // Contacted or not

	bool bBond;                // Bond exists or not

	double dKn;                // Stiffness of normal spring

	NJRvector3d vShearForce;   // Shear force

	double dUDV[uNumUserDefinedData*4];   // User-defined value
									      // 0 ~ uNumUserDefinedData: Accumulative user-defined value
									      // uNumUserDefinedData ~ 2*uNumUserDefinedData-1: Unsynchronized part of Accumulative user-defined value
									      // 2*uNumUserDefinedData ~ 3*uNumUserDefinedData-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
									      // 3*uNumUserDefinedData ~ 4*uNumUserDefinedData-1: User-defined value
};

#endif // _IMPACT_STATUS_H
