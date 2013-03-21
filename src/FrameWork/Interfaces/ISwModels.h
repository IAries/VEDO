#ifndef _ISWMODELS_H
#define _ISWMODELS_H

#include <NJR/Interfaces/vector3d.h>
#include <algorithm>

namespace VEDO
{

class ISwModels
{

public:

	double CriticalTimeStep(const double& dMeff, const double& dK);

	double CriticalTimeStep(const double& dMeff, const double& dKn,  const double& dKt);

	std::pair<double, double> CriticalDamping
		(const double& dMa, const double& dMb,
		 const double& dKn, const double& dKs );

	bool NormalBond
		(const double& dKn,
		 const double& dCn,
		 const double& dBn,
		 const double& dImpactDepth);

	NJR::NJRvector3d NormalForceHertzSpring
		(const double&      dKn,
		 const double&      dCn,
		 const double&      dImpactDepth,
		 const NJR::NJRvector3d& vImpactDirection,
		 const NJR::NJRvector3d& vRelativeNormalVelocity);

	NJR::NJRvector3d NormalForce
		(const double&      dKn,
		 const double&      dCn,
		 const double&      dImpactDepth,
		 const NJR::NJRvector3d& vImpactDirection,
		 const NJR::NJRvector3d& vRelativeNormalVelocity);

	NJR::NJRvector3d NormalForceNoTension
		(const double&      dKn,
		 const double&      dCn,
		 const double&      dImpactDepth,
		 const NJR::NJRvector3d& vImpactDirection,
		 const NJR::NJRvector3d& vRelativeNormalVelocity);

	NJR::NJRvector3d NormalForce
		(const double&      dKn,
		 const double&      dCn,
		 const double&      dBn,
		 const double&      dImpactDepth,
		 const NJR::NJRvector3d& vImpactDirection,
		 const NJR::NJRvector3d& vRelativeNormalVelocity);

	NJR::NJRvector3d NormalForceNoTension
		(const double&      dKn,
		 const double&      dCn,
		 const double&      dBn,
		 const double&      dImpactDepth,
		 const NJR::NJRvector3d& vImpactDirection,
		 const NJR::NJRvector3d& vRelativeNormalVelocity);

	double ElasticModules2EquivalentStiffness
		(const double& dEa,
		 const double& dEb,
		 const double& dRa,
		 const double& dRb,
		 const double& dOverlapArea);

	std::pair<bool, std::pair<double, double> > FrictionForce
		(const double& dStaticFrictionCoefficient,
		 const double& dKineticFrictionCoefficient,
		 const double& dFrictionSwitch,
		 const double& dRelativeShearVelocity,
		 const double& dNormalForce                );

	std::pair<bool, std::pair<double, double> > FrictionForce
		(const double& dStaticFrictionCoefficient,
		 const double& dKineticFrictionCoefficient,
		 const double& dRelativeShearVelocity,
		 const double& dNormalForce                );

	NJR::NJRvector3d ShearForceRotation(const NJR::NJRvector3d&, const NJR::NJRvector3d&);

private:

	inline ISwModels()
	{
	};

	inline ~ISwModels()
	{
	};

	inline ISwModels(const ISwModels&)
	{
	};

};

};   // namespace VEDO

#endif // ISWMODELS_H
