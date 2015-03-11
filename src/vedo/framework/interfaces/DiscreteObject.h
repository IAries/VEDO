#ifndef _DISCRETE_OBJECT_H
#define _DISCRETE_OBJECT_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/Boundary.h>
#include <vedo/framework/interfaces/DOWorld.h>

#include <string>

namespace vedo
{

class DiscreteObject
{

public:

	virtual ~DiscreteObject();

	inline vedo::vedo_uint_t GetCounter() const
	{
		return ulCounter;
	}

	inline vedo_float_t GetMass() const
	{
		return dMass;
	}

	inline vedo_float_t GetSudoMass() const
	{
		return dSudoMass;
	}

	inline njr::Vector3d GetMassMomentInertia() const
	{
		return vMassMomentInertia;
	}

	inline void ModifyMass(vedo_float_t d)
	{
		dMass = d;
	}

	inline vedo_float_t GetVolume() const
	{
		return dVolume;
	}

	inline const DOModel* GetDOModel() const
	{
		return cpDOModel;
	}

	inline const DOStatus* GetDOStatus() const
	{
		return pDOStatus;
	}

	inline const njr::Vector3d& GetImpact() const
	{
		return vImpact;
	}

	inline const njr::Vector3d& GetAngularImpact() const
	{
		return vAngularImpact;
	}

	inline void SetVelocity(const njr::Vector3d& vV)
	{
		pDOStatus->SetVelocity(vV);
	}

	inline void SetAngularVelocity(const njr::Vector3d& vAV)
	{
		pDOStatus->SetAngularVelocity(vAV);
	}

	void ModifyVelocity(const njr::Vector3d& vdv);

	void ModifyPosition(const njr::Vector3d& vdp);

	void ClearImpact();

	virtual void AddImpact(const njr::Vector3d& vImpact, const njr::Vector3d& vAngularImpact = njr::ZERO);

/*
	inline njr::Vector3d GetFieldImpact() const
	{
		return pDOStatus->GetFieldImpact();
	}

	inline void SetFieldImpact(const njr::Vector3d& vi)
	{
		pDOStatus->SetFieldImpact(vi);
	}
*/

	virtual void Response(vedo_float_t dt) = 0;

	void AddConstrainedImpact(vedo_float_t dt);

	// Cross area to surface ax+by+cz=d;
	inline virtual vedo_float_t CrossAreaToSurface
		(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const = 0;

	inline virtual std::string type () const
	{
		return "DiscreteObject";
	}

	virtual inline vedo_float_t GetSudoContactRadius() = 0;

	void EnforcePeriodicBoundaryConditions(const Boundary& bc);

protected:

	const DOModel* cpDOModel;

	DOStatus*      pDOStatus;

	njr::Vector3d  vImpact;

	njr::Vector3d  vAngularImpact;

	vedo_float_t   dVolume;

	vedo_float_t   dMass;

	vedo_float_t   dSudoMass;

	njr::Vector3d  vMassMomentInertia;

	DiscreteObject(const DOStatus* cpdos, const DOModel* cpdoml);

private:

	static vedo::vedo_uint_t ulCounter;

	DiscreteObject();

	DiscreteObject& operator = (const DiscreteObject&);

	DiscreteObject(const DiscreteObject&);
};

}   // namespace vedo

#endif // _DISCRETE_OBJECT_H
