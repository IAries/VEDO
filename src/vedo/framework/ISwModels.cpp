#include <aries/utility/Constants.h>
#include <vedo/framework/ISwModels.h>
#include <cmath>

namespace vedo
{

_float_t ISwModels::CriticalTimeStep(const _float_t& dMeff, const _float_t& dK)
{
	return aries::math::_DoublePI * sqrt(dMeff / dK);
}

_float_t ISwModels::CriticalTimeStep(const _float_t& dMeff, const _float_t& dKn, const _float_t& dKt)
{
	if (dKn == 0.0)
	{
		if (dKt == 0.0)
		{
			return -1.0;
		}
		else
		{
			return aries::math::_DoublePI * sqrt(dMeff / dKt);
		}
	}
	else
	{
		if (dKt == 0.0)
		{
			return aries::math::_DoublePI * sqrt(dMeff / dKn);
		}
		else
		{
			return aries::math::_DoublePI * sqrt(dMeff / std::max(dKn, dKt));
		}
	}
}

_float_t ISwModels::CriticalDamping(const _float_t& dMa, const _float_t& dMb, const _float_t& dKn)
{
	_float_t dMe;   // Effective System Mass
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

/*
_float_t ISwModels::EffectiveRadius(const _float_t& fRadiusA, const _float_t& fRadiusB)
{
	if (fRadiusA == 0.0)
	{
		if (fRadiusB == 0.0)
		{
			return 0.0;
		}
		else
		{
			return fRadiusB;
		}
	}
	else
	{
		if (fRadiusB == 0.0)
		{
			return fRadiusA;
		}
		else
		{
			return (fRadiusA * fRadiusB / (fRadiusA + fRadiusB));
		}
	}
}
*/

std::pair<_float_t, _float_t> ISwModels::CriticalDamping
	(const _float_t& dMa, const _float_t& dMb, const _float_t& dKn, const _float_t& dKs)
{
	_float_t dMe;   // Effective System Mass
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
	(const _float_t& dKn, const _float_t& dCn, const _float_t& dBn, const _float_t& dImpactDepth)
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

Vector3df ISwModels::NormalForceHertzSpring
	(const _float_t& dKn, const _float_t& dCn, const _float_t& dImpactDepth,
	 const Vector3df& vImpactDirection, const Vector3df& vRelativeNormalVelocity)
{
	return Vector3df(- dKn * pow(dImpactDepth, 1.5) * vImpactDirection - dCn * vRelativeNormalVelocity);
}

Vector3df ISwModels::NormalForce
	(const _float_t& dKn, const _float_t& dCn, const _float_t& dImpactDepth,
	 const Vector3df& vImpactDirection, const Vector3df& vRelativeNormalVelocity)
{
	return Vector3df(- dKn * dImpactDepth * vImpactDirection - dCn * vRelativeNormalVelocity);
}

Vector3df ISwModels::NormalForceNoTension
	(const _float_t& dKn, const _float_t& dCn, const _float_t& dImpactDepth,
	 const Vector3df& vImpactDirection, const Vector3df& vRelativeNormalVelocity)
{
	if ((vRelativeNormalVelocity.dot(vImpactDirection)) < 0.0)
	{
		// Elements move far away from each others
		Vector3df vSpringForce = - dKn * dImpactDepth * vImpactDirection;
		Vector3df vShearViscousDampingForce = - dCn * vRelativeNormalVelocity;
		if (vShearViscousDampingForce.length() > vSpringForce.length())
		{
			return Vector3df();
		}
		else
		{
			return vSpringForce + vShearViscousDampingForce;
		}
	}
	else
	{
		// Elements move colser to each others
		return Vector3df(- dKn * dImpactDepth * vImpactDirection - dCn * vRelativeNormalVelocity);
	}
}

Vector3df ISwModels::NormalForce
	(const _float_t& dKn, const _float_t& dCn, const _float_t& dBn, const _float_t& dImpactDepth,
	 const Vector3df& vImpactDirection, const Vector3df& vRelativeNormalVelocity                         )
{
	if ((vRelativeNormalVelocity.dot(vImpactDirection)) < 0.0)
	{
		// Elements move far away from each others
		return Vector3df(- dKn * dImpactDepth * vImpactDirection - dCn * vRelativeNormalVelocity);
	}
	else
	{
		// Elements move colser to each others
		return Vector3df(- dCn * vRelativeNormalVelocity);
	}
}

Vector3df ISwModels::NormalForceNoTension
	(const _float_t& dKn, const _float_t& dCn, const _float_t& dBn, const _float_t& dImpactDepth,
	 const Vector3df& vImpactDirection, const Vector3df& vRelativeNormalVelocity                         )
{
	if ((vRelativeNormalVelocity.dot(vImpactDirection)) < 0.0)
	{
		// Elements move far away from each others
		Vector3df vSpringForce = - dKn * dImpactDepth * vImpactDirection;
		Vector3df vShearViscousDampingForce = - dCn * vRelativeNormalVelocity;
		if (vShearViscousDampingForce.length() > vSpringForce.length())
		{
			return Vector3df();
		}
		else
		{
			return vSpringForce + vShearViscousDampingForce;
		}
	}
	else
	{
		// Elements move colser to each others
		return Vector3df(- dCn * vRelativeNormalVelocity);
	}
}

_float_t ISwModels::ElasticModules2EquivalentStiffness
	(const _float_t& dEa, const _float_t& dEb,
	 const _float_t& dRa, const _float_t& dRb,
	 const _float_t& dOverlapArea                 )
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

std::pair<bool, std::pair<_float_t, _float_t> > ISwModels::FrictionForce
	(const _float_t& dStaticFrictionCoefficient,
	 const _float_t& dKineticFrictionCoefficient,
	 const _float_t& dFrictionSwitch,
	 const _float_t& dRelativeShearVelocity,
	 const _float_t& dNormalForce                )
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

std::pair<bool, std::pair<_float_t, _float_t> > ISwModels::FrictionForce
	(const _float_t& dStaticFrictionCoefficient,
	 const _float_t& dKineticFrictionCoefficient,
	 const _float_t& dRelativeShearVelocity,
	 const _float_t& dNormalForce                )
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

Vector3df ISwModels::ShearForceRotation(const Vector3df& vShearForce, const Vector3df& vNewDirection)
{
	if (vShearForce.dot(vNewDirection) <= 0.0)
	{
		return Vector3df(-vShearForce.length() * vNewDirection.direction());
	}
	else
	{
		return Vector3df(vShearForce.length() * vNewDirection.direction());
	}
}

_float_t ISwModels::WetDampingRatioLegendre2006(_float_t dDryRestitutionCoefficient, _float_t dBinaryStokesNumber)
{
	if ((dBinaryStokesNumber == 0.0) || (dDryRestitutionCoefficient == 0.0))
	{
		return 1.0;
	}

	_float_t dWetRestitutionCoefficient = dDryRestitutionCoefficient * exp(-35.0/dBinaryStokesNumber);

	if (dWetRestitutionCoefficient == 0.0)
	{
		return 1.0;
	}

	_float_t beta_c = log(dWetRestitutionCoefficient) / aries::math::_PI;
	return (-beta_c / sqrt(1.0 + beta_c * beta_c));
}

}   // namespace vedo
