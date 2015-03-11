#include <vedo/njr/interfaces/Constants.h>
#include <vedo/framework/interfaces/ISwModels.h>
#include <cmath>

namespace vedo
{

vedo_float_t ISwModels::CriticalTimeStep(const vedo_float_t& dMeff, const vedo_float_t& dK)
{
	return njr::dDoublePI * sqrt(dMeff / dK);
}

vedo_float_t ISwModels::CriticalTimeStep(const vedo_float_t& dMeff, const vedo_float_t& dKn, const vedo_float_t& dKt)
{
	if (dKn == 0.0)
	{
		if (dKt == 0.0)
		{
			return -1.0;
		}
		else
		{
			return njr::dDoublePI * sqrt(dMeff / dKt);
		}
	}
	else
	{
		if (dKt == 0.0)
		{
			return njr::dDoublePI * sqrt(dMeff / dKn);
		}
		else
		{
			return njr::dDoublePI * sqrt(dMeff / std::max(dKn, dKt));
		}
	}
}

vedo_float_t ISwModels::CriticalDamping(const vedo_float_t& dMa, const vedo_float_t& dMb, const vedo_float_t& dKn)
{
	vedo_float_t dMe;   // Effective System Mass
	if (dMb == 0)
	{
		dMe = dMa;
	}
	else
	{
		if (dMa == 0)
		{
			dMe = dMb;
		}
		else
		{
			dMe = 0.5 * (dMa + dMb);
		}
	}
	return 2.0 * sqrt(dMe * dKn);
}

std::pair<vedo_float_t, vedo_float_t> ISwModels::CriticalDamping
	(const vedo_float_t& dMa, const vedo_float_t& dMb, const vedo_float_t& dKn, const vedo_float_t& dKs)
{
	vedo_float_t dMe;   // Effective System Mass
	if (dMb == 0)
	{
		dMe = dMa;
	}
	else
	{
		if (dMa == 0)
		{
			dMe = dMb;
		}
		else
		{
			dMe = 0.5 * (dMa + dMb);
		}
	}
	return std::make_pair(2.0 * sqrt(dMe * dKn), 2.0 * sqrt(dMe * dKs));
}

bool ISwModels::NormalBond
	(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dBn, const vedo_float_t& dImpactDepth)
{
//		&& (dCn != 0.0)
	if(    (dImpactDepth <= 0.0)
		&& (dKn != 0.0)
		&& (dBn != 0.0)
		&& (-dImpactDepth * dKn < dBn) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

njr::Vector3d ISwModels::NormalForceHertzSpring
	(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dImpactDepth,
	 const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity)
{
	return njr::Vector3d(- dKn * pow(dImpactDepth, 1.5) * vImpactDirection - dCn * vRelativeNormalVelocity);
}

njr::Vector3d ISwModels::NormalForce
	(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dImpactDepth,
	 const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity)
{
	return njr::Vector3d(- dKn * dImpactDepth * vImpactDirection - dCn * vRelativeNormalVelocity);
}

njr::Vector3d ISwModels::NormalForceNoTension
	(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dImpactDepth,
	 const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity)
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		njr::Vector3d vSpringForce = - dKn * dImpactDepth * vImpactDirection;
		njr::Vector3d vShearViscousDampingForce = - dCn * vRelativeNormalVelocity;
		if (vShearViscousDampingForce.length() > vSpringForce.length())
		{
			return njr::ZERO;
		}
		else
		{
			return vSpringForce + vShearViscousDampingForce;
		}
	}
	else
	{
		// Elements move colser to each others
		return njr::Vector3d(- dKn * dImpactDepth * vImpactDirection - dCn * vRelativeNormalVelocity);
	}
}

njr::Vector3d ISwModels::NormalForce
	(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dBn, const vedo_float_t& dImpactDepth,
	 const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity                         )
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		return njr::Vector3d(- dKn * dImpactDepth * vImpactDirection - dCn * vRelativeNormalVelocity);
	}
	else
	{
		// Elements move colser to each others
		return njr::Vector3d(- dCn * vRelativeNormalVelocity);
	}
}

njr::Vector3d ISwModels::NormalForceNoTension
	(const vedo_float_t& dKn, const vedo_float_t& dCn, const vedo_float_t& dBn, const vedo_float_t& dImpactDepth,
	 const njr::Vector3d& vImpactDirection, const njr::Vector3d& vRelativeNormalVelocity                         )
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		njr::Vector3d vSpringForce = - dKn * dImpactDepth * vImpactDirection;
		njr::Vector3d vShearViscousDampingForce = - dCn * vRelativeNormalVelocity;
		if (vShearViscousDampingForce.length() > vSpringForce.length())
		{
			return njr::ZERO;
		}
		else
		{
			return vSpringForce + vShearViscousDampingForce;
		}
	}
	else
	{
		// Elements move colser to each others
		return njr::Vector3d(- dCn * vRelativeNormalVelocity);
	}
}

vedo_float_t ISwModels::ElasticModules2EquivalentStiffness
	(const vedo_float_t& dEa, const vedo_float_t& dEb,
	 const vedo_float_t& dRa, const vedo_float_t& dRb,
	 const vedo_float_t& dOverlapArea                 )
{
	if ((dRa == 0.0) && (dRb == 0.0))
	{
		return 0.0;
	}
	else
	{
		return dOverlapArea * dEa * dEb / (dEa * dRb + dEb * dRa);
	}
}

std::pair<bool, std::pair<vedo_float_t, vedo_float_t> > ISwModels::FrictionForce
	(const vedo_float_t& dStaticFrictionCoefficient,
	 const vedo_float_t& dKineticFrictionCoefficient,
	 const vedo_float_t& dFrictionSwitch,
	 const vedo_float_t& dRelativeShearVelocity,
	 const vedo_float_t& dNormalForce                )
{
	if (dRelativeShearVelocity <= dFrictionSwitch)
	{
		return
			std::make_pair
				(false, std::make_pair(dStaticFrictionCoefficient * dNormalForce, dKineticFrictionCoefficient * dNormalForce));
	}
	else
	{
		return
			std::make_pair
				(true, std::make_pair(dStaticFrictionCoefficient * dNormalForce, dKineticFrictionCoefficient * dNormalForce));
	}
}

std::pair<bool, std::pair<vedo_float_t, vedo_float_t> > ISwModels::FrictionForce
	(const vedo_float_t& dStaticFrictionCoefficient,
	 const vedo_float_t& dKineticFrictionCoefficient,
	 const vedo_float_t& dRelativeShearVelocity,
	 const vedo_float_t& dNormalForce                )
{
	if (dRelativeShearVelocity == 0.0)
	{
		return
			std::make_pair
				(false, std::make_pair (dStaticFrictionCoefficient * dNormalForce, dKineticFrictionCoefficient * dNormalForce));
	}
	else
	{
		return
			std::make_pair
				(true, std::make_pair (dStaticFrictionCoefficient * dNormalForce, dKineticFrictionCoefficient * dNormalForce));
	}
}

njr::Vector3d ISwModels::ShearForceRotation(const njr::Vector3d& vShearForce, const njr::Vector3d& vNewDirection)
{
	if (vShearForce.Dot(vNewDirection) <= 0.0)
	{
		return njr::Vector3d(-vShearForce.length() * vNewDirection.direction());
	}
	else
	{
		return njr::Vector3d(vShearForce.length() * vNewDirection.direction());
	}
}

vedo_float_t ISwModels::WetDampingRatioLegendre2006(vedo_float_t dDryRestitutionCoefficient, vedo_float_t dBinaryStokesNumber)
{
	if ((dBinaryStokesNumber == 0.0) || (dDryRestitutionCoefficient == 0.0))
	{
		return 1.0;
	}

	vedo_float_t dWetRestitutionCoefficient = dDryRestitutionCoefficient * exp(-35.0/dBinaryStokesNumber);

	if (dWetRestitutionCoefficient == 0.0)
	{
		return 1.0;
	}

	vedo_float_t beta_c = log(dWetRestitutionCoefficient) / njr::dPI;
	return (-beta_c / sqrt(1.0 + beta_c * beta_c));
}

}   // namespace vedo
