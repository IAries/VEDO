#ifndef _ISWMODELS_H
#define _ISWMODELS_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <algorithm>

namespace vedo
{

class ISwModels
{

public:

	vedo_float_t CriticalTimeStep(const vedo_float_t& dMeff, const vedo_float_t& dK);

	vedo_float_t CriticalTimeStep(const vedo_float_t& dMeff, const vedo_float_t& dKn, const vedo_float_t& dKt);

	vedo_float_t CriticalDamping(const vedo_float_t& dMa, const vedo_float_t& dMb, const vedo_float_t& dKn);

	std::pair<vedo_float_t, vedo_float_t> CriticalDamping
		(const vedo_float_t& dMa, const vedo_float_t& dMb, const vedo_float_t& dKn, const vedo_float_t& dKs);

	bool NormalBond
		(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dBn, const vedo_float_t& dImpactDepth);

	njr::Vector3d NormalForceHertzSpring
		(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dImpactDepth,
		 const njr::Vector3d& vImpactDirection, const njr::Vector3d& vInitialRelativeNormalVelocity);

	njr::Vector3d NormalForce
		(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dImpactDepth,
		 const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity);

	njr::Vector3d NormalForceNoTension
		(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dImpactDepth,
		 const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity);

	njr::Vector3d NormalForce
		(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dBn,
		 const vedo_float_t& dImpactDepth, const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity);

	njr::Vector3d NormalForceNoTension
		(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dBn,
		 const vedo_float_t& dImpactDepth, const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity);

	vedo_float_t ElasticModules2EquivalentStiffness
		(const vedo_float_t& dEa, const vedo_float_t& dEb, const vedo_float_t& dRa, const vedo_float_t& dRb,
		 const vedo_float_t& dOverlapArea                                                                   );

	std::pair<bool, std::pair<vedo_float_t, vedo_float_t> > FrictionForce
		(const vedo_float_t& dStaticFrictionCoefficient,
		 const vedo_float_t& dKineticFrictionCoefficient,
		 const vedo_float_t& dFrictionSwitch,
		 const vedo_float_t& dRelativeShearVelocity,
		 const vedo_float_t& dNormalForce                );

	std::pair<bool, std::pair<vedo_float_t, vedo_float_t> > FrictionForce
		(const vedo_float_t& dStaticFrictionCoefficient,
		 const vedo_float_t& dKineticFrictionCoefficient,
		 const vedo_float_t& dRelativeShearVelocity,
		 const vedo_float_t& dNormalForce                );

	njr::Vector3d ShearForceRotation(const njr::Vector3d&, const njr::Vector3d&);

	vedo_float_t WetDampingRatioLegendre2006(vedo_float_t dDryRestitutionCoefficient, vedo_float_t dBinaryStokesNumber);
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
