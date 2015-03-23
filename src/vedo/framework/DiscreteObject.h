#ifndef _DISCRETE_OBJECT_H
#define _DISCRETE_OBJECT_H

#include <vedo/framework/Boundary.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/DataType.h>

#include <string>

namespace vedo
{

class DiscreteObject
{

public:

	virtual ~DiscreteObject();

	inline _uint_t GetCounter() const
	{
		return ulCounter;
	}

	inline _float_t GetMass() const
	{
		return dMass;
	}

	inline _float_t GetSudoMass() const
	{
		return dSudoMass;
	}

	inline aries::Vector3df GetMassMomentInertia() const
	{
		return vMassMomentInertia;
	}

	inline void ModifyMass(_float_t d)
	{
		dMass = d;
	}

	inline _float_t GetVolume() const
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

	inline const aries::Vector3df& GetImpact() const
	{
		return vImpact;
	}

	inline const aries::Vector3df& GetAngularImpact() const
	{
		return vAngularImpact;
	}

	inline void SetVelocity(const aries::Vector3df& vV)
	{
		pDOStatus->SetVelocity(vV);
	}

	inline void SetAngularVelocity(const aries::Vector3df& vAV)
	{
		pDOStatus->SetAngularVelocity(vAV);
	}

	void ModifyVelocity(const aries::Vector3df& vdv);

	void ModifyPosition(const aries::Vector3df& vdp);

	void ClearImpact();

	virtual void AddImpact(const aries::Vector3df& vImpact, const aries::Vector3df& vAngularImpact);

	virtual void AddImpact(const aries::Vector3df& vImpact);

/*
	inline aries::Vector3df GetFieldImpact() const
	{
		return pDOStatus->GetFieldImpact();
	}

	inline void SetFieldImpact(const aries::Vector3df& vi)
	{
		pDOStatus->SetFieldImpact(vi);
	}
*/

	virtual void Response(_float_t dt) = 0;

	void AddConstrainedImpact(_float_t dt);

	// cross area to surface ax+by+cz=d;
	inline virtual _float_t CrossAreaToSurface
		(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const = 0;

	inline virtual std::string type () const
	{
		return "DiscreteObject";
	}

	virtual inline _float_t GetSudoContactRadius() = 0;

	void EnforcePeriodicBoundaryConditions(const Boundary& bc);

protected:

	const DOModel* cpDOModel;

	DOStatus*      pDOStatus;

	aries::Vector3df  vImpact;

	aries::Vector3df  vAngularImpact;

	_float_t   dVolume;

	_float_t   dMass;

	_float_t   dSudoMass;

	aries::Vector3df  vMassMomentInertia;

	DiscreteObject(const DOStatus* cpdos, const DOModel* cpdoml);

private:

	static _uint_t ulCounter;

	DiscreteObject();

	DiscreteObject& operator = (const DiscreteObject&);

	DiscreteObject(const DiscreteObject&);
};

}   // namespace vedo

#endif // _DISCRETE_OBJECT_H
