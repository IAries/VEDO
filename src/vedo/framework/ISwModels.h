#ifndef _ISWMODELS_H
#define _ISWMODELS_H

#include <vedo/framework/DataType.h>
#include <algorithm>

namespace vedo
{

class ISwModels
{

public:

	_float_t CriticalTimeStep(const _float_t& dMeff, const _float_t& dK);

	_float_t CriticalTimeStep(const _float_t& dMeff, const _float_t& dKn, const _float_t& dKt);

	_float_t CriticalDamping(const _float_t& dMa, const _float_t& dMb, const _float_t& dKn);

	std::pair<_float_t, _float_t> CriticalDamping
		(const _float_t& dMa, const _float_t& dMb, const _float_t& dKn, const _float_t& dKs);

	bool NormalBond
		(const _float_t& dKn, const _float_t& dCn, const _float_t& dBn, const _float_t& dImpactDepth);

	aries::Vector3df NormalForceHertzSpring
		(const _float_t& dKn, const _float_t& dCn, const _float_t& dImpactDepth,
		 const aries::Vector3df& vImpactDirection, const aries::Vector3df& vInitialRelativeNormalVelocity);

	aries::Vector3df NormalForce
		(const _float_t& dKn, const _float_t& dCn, const _float_t& dImpactDepth,
		 const aries::Vector3df& vImpactDirection, const aries::Vector3df& vRelativeNormalVelocity);

	aries::Vector3df NormalForceNoTension
		(const _float_t& dKn, const _float_t& dCn, const _float_t& dImpactDepth,
		 const aries::Vector3df& vImpactDirection, const aries::Vector3df& vRelativeNormalVelocity);

	aries::Vector3df NormalForce
		(const _float_t& dKn, const _float_t& dCn, const _float_t& dBn,
		 const _float_t& dImpactDepth, const aries::Vector3df& vImpactDirection, const aries::Vector3df& vRelativeNormalVelocity);

	aries::Vector3df NormalForceNoTension
		(const _float_t& dKn, const _float_t& dCn, const _float_t& dBn,
		 const _float_t& dImpactDepth, const aries::Vector3df& vImpactDirection, const aries::Vector3df& vRelativeNormalVelocity);

	_float_t ElasticModules2EquivalentStiffness
		(const _float_t& dEa, const _float_t& dEb, const _float_t& dRa, const _float_t& dRb,
		 const _float_t& dOverlapArea                                                                   );

	std::pair<bool, std::pair<_float_t, _float_t> > FrictionForce
		(const _float_t& dStaticFrictionCoefficient,
		 const _float_t& dKineticFrictionCoefficient,
		 const _float_t& dFrictionSwitch,
		 const _float_t& dRelativeShearVelocity,
		 const _float_t& dNormalForce                );

	std::pair<bool, std::pair<_float_t, _float_t> > FrictionForce
		(const _float_t& dStaticFrictionCoefficient,
		 const _float_t& dKineticFrictionCoefficient,
		 const _float_t& dRelativeShearVelocity,
		 const _float_t& dNormalForce                );

	aries::Vector3df ShearForceRotation(const aries::Vector3df&, const aries::Vector3df&);

	_float_t WetDampingRatioLegendre2006(_float_t dDryRestitutionCoefficient, _float_t dBinaryStokesNumber);
/*
private:

	inline ISwModels()
	{
	}

	inline ~ISwModels()
	{
	}

	inline ISwModels(const ISwModels&)
	{
	}
*/
};

}   // namespace vedo

#endif // ISWMODELS_H
