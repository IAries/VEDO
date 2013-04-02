#ifndef _DISCRETE_OBJECT_H
#define _DISCRETE_OBJECT_H

#include <NJR/Interfaces/Vector3d.h>
#include <Framework/Interfaces/Boundary.h>
#include <Framework/Interfaces/DOWorld.h>

#include <string>

namespace vedo
{

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

	inline njr::Vector3d GetMassMomentInertia() const
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

	inline const njr::Vector3d& GetImpact() const
	{
		return vImpact;
	};

	inline const njr::Vector3d& GetAngularImpact() const
	{
		return vAngularImpact;
	};

	inline void SetVelocity(const njr::Vector3d& vV)
	{
		pDOStatus->SetVelocity(vV);
	};

	inline void SetAngularVelocity(const njr::Vector3d& vAV)
	{
		pDOStatus->SetAngularVelocity(vAV);
	};

	void ModifyVelocity(const njr::Vector3d& vdv);

	void ModifyPosition(const njr::Vector3d& vdp);

	void ClearImpact();

	virtual void AddImpact
		(const njr::Vector3d& vImpact, const njr::Vector3d& vAngularImpact = njrdxf::ZERO);

/*
	inline njr::Vector3d GetFieldImpact() const
	{
		return pDOStatus->GetFieldImpact();
	};

	inline void SetFieldImpact(const njr::Vector3d& vi)
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

	njr::Vector3d	vImpact;

	njr::Vector3d	vAngularImpact;

	double dVolume;

	double dMass;

	double dSudoMass;

	njr::Vector3d	vMassMomentInertia;

	DiscreteObject(const DOStatus* cpdos, const DOModel* cpdoml);

private:

	static unsigned long ulCounter;
	DiscreteObject();
	DiscreteObject& operator = (const DiscreteObject&);
	DiscreteObject(const DiscreteObject&);
};

};   // namespace vedo

#endif // _DISCRETE_OBJECT_H
