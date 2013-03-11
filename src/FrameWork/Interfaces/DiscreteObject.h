#ifndef _DISCRETE_OBJECT_H
#define _DISCRETE_OBJECT_H

#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/Boundary.h>
#include <FrameWork/Interfaces/DOWorld.h>

#include <string>

class DiscreteObject
{

public:

	virtual ~DiscreteObject();

	inline unsigned long GetCounter() const
	{
		return ulCounter;
	};

	inline double GetMass() const
	{
		return dMass;
	};

	inline double GetSudoMass() const
	{
		return dSudoMass;
	};

	inline NJRvector3d GetMassMomentInertia() const
	{
		return vMassMomentInertia;
	}

	inline void ModifyMass(double d)
	{
		dMass = d;
	}

	inline double GetVolume() const
	{
		return dVolume;
	};

	inline const DOModel* GetDOModel() const
	{
		return cpDOModel;
	};

	inline const DOStatus* GetDOStatus() const
	{
		return pDOStatus;
	};

	inline const NJRvector3d& GetImpact() const
	{
		return vImpact;
	};

	inline const NJRvector3d& GetAngularImpact() const
	{
		return vAngularImpact;
	};

	inline const double GetRange() const
	{
		return pDOStatus->GetRange();
	};

	inline void SetRange(double r)
	{
		pDOStatus->SetRange(r);
	};

	inline void SetVelocity(const NJRvector3d& vV)
	{
		pDOStatus->SetVelocity(vV);
	};

	inline void SetAngularVelocity(const NJRvector3d& vAV)
	{
		pDOStatus->SetAngularVelocity(vAV);
	};

	void ModifyVelocity(const NJRvector3d& vdv);

	void ModifyPosition(const NJRvector3d& vdp);

	void ClearImpact();

	virtual void AddImpact
		(const NJRvector3d& vImpact, const NJRvector3d& vAngularImpact = ZERO);

/*
	inline NJRvector3d GetFieldImpact() const
	{
		return pDOStatus->GetFieldImpact();
	};

	inline void SetFieldImpact(const NJRvector3d& vi)
	{
		pDOStatus->SetFieldImpact(vi);
	};
*/

	virtual void Response(double dt) = 0;

	void AddConstrainedImpact(double dt);

	// Cross area to surface ax+by+cz=d;
	inline virtual double CrossAreaToSurface
		(double& a, double& b, double& c, double& d) const = 0;

	inline virtual std::string type () const
	{
		return "DiscreteObject";
	};

	virtual inline double GetSudoContactRadius() = 0;

	void EnforcePeriodicBoundaryConditions(const Boundary& bc);

protected:

	const DOModel* cpDOModel;

	DOStatus* pDOStatus;

	NJRvector3d	vImpact;

	NJRvector3d	vAngularImpact;

	double dVolume;

	double dMass;

	double dSudoMass;

	NJRvector3d	vMassMomentInertia;

	DiscreteObject(const DOStatus* cpdos, const DOModel* cpdoml);

private:

	static unsigned long ulCounter;
	DiscreteObject();
	DiscreteObject& operator = (const DiscreteObject&);
	DiscreteObject(const DiscreteObject&);
};

#endif // _DISCRETE_OBJECT_H
