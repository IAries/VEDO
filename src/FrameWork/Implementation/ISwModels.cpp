#include <Framework/Interfaces/Constants.h>
#include <Framework/Interfaces/ISwModels.h>
#include <cmath>

namespace vedo
{

double ISwModels::CriticalTimeStep(const double& dMeff, const double& dK)
{
	return njr::dDoublePI * sqrt(dMeff / dK);
};

double ISwModels::CriticalTimeStep
	(const double& dMeff, const double& dKn, const double& dKt)
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
};

std::pair<double, double> ISwModels::CriticalDamping
	(const double& dMa, const double& dMb,
	 const double& dKn, const double& dKs )
{
	double dMe;   // Effective System Mass
	if(dMb == 0)
	{
		dMe = dMa;
	}
	else
	{
		if(dMa == 0)
		{
			dMe = dMb;
		}
		else
		{
			dMe = 0.5 * (dMa + dMb);
		}
	}
	return std::make_pair(2.0 * sqrt(dMe * dKn), 2.0 * sqrt(dMe * dKs));
};

bool ISwModels::NormalBond
	(const double& dKn,
	 const double& dCn,
	 const double& dBn,
	 const double& dImpactDepth)
{
//		&& (dCn != 0.0)
	if (   (dImpactDepth <= 0.0)
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
};

njr::Vector3d ISwModels::NormalForceHertzSpring
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dImpactDepth,
	 const njr::Vector3d& vImpactDirection,
	 const njr::Vector3d& vRelativeNormalVelocity)
{
	return njr::Vector3d
		(- dKn * pow(dImpactDepth, 1.5) * vImpactDirection
		 - dCn * vRelativeNormalVelocity                       );
};

njr::Vector3d ISwModels::NormalForce
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dImpactDepth,
	 const njr::Vector3d& vImpactDirection,
	 const njr::Vector3d& vRelativeNormalVelocity)
{
	return njr::Vector3d
		(- dKn * dImpactDepth * vImpactDirection
		 - dCn * vRelativeNormalVelocity        );
};

njr::Vector3d ISwModels::NormalForceNoTension
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dImpactDepth,
	 const njr::Vector3d& vImpactDirection,
	 const njr::Vector3d& vRelativeNormalVelocity)
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		njr::Vector3d vSpringForce = - dKn * dImpactDepth * vImpactDirection;
		njr::Vector3d vShearViscousDampingForce = - dCn * vRelativeNormalVelocity;
		if(vShearViscousDampingForce.length() > vSpringForce.length())
			return njr::ZERO;
		else
			return vSpringForce + vShearViscousDampingForce;
	}
	else
	{
		// Elements move colser to each others
		return njr::Vector3d
			(- dKn * dImpactDepth * vImpactDirection
			 - dCn * vRelativeNormalVelocity        );
	}
};

njr::Vector3d ISwModels::NormalForce
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dBn,
	 const double&      dImpactDepth,
	 const njr::Vector3d& vImpactDirection,
	 const njr::Vector3d& vRelativeNormalVelocity)
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		return njr::Vector3d
			(- dKn * dImpactDepth * vImpactDirection
			 - dCn                * vRelativeNormalVelocity);
	}
	else
	{
		// Elements move colser to each others
		return njr::Vector3d(- dCn * vRelativeNormalVelocity);
	}
};

njr::Vector3d ISwModels::NormalForceNoTension
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dBn,
	 const double&      dImpactDepth,
	 const njr::Vector3d& vImpactDirection,
	 const njr::Vector3d& vRelativeNormalVelocity)
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		njr::Vector3d vSpringForce = - dKn * dImpactDepth * vImpactDirection;
		njr::Vector3d vShearViscousDampingForce = - dCn * vRelativeNormalVelocity;
		if(vShearViscousDampingForce.length() > vSpringForce.length())
			return njr::ZERO;
		else
			return vSpringForce + vShearViscousDampingForce;
	}
	else
	{
		// Elements move colser to each others
		return njr::Vector3d(- dCn * vRelativeNormalVelocity);
	}
};

double ISwModels::ElasticModules2EquivalentStiffness
	(const double& dEa,
	 const double& dEb,
	 const double& dRa,
	 const double& dRb,
	 const double& dOverlapArea)
{
	if ((dRa == 0.0) && (dRb == 0.0))
	{
		return 0.0;
	}
	else
	{
		return dOverlapArea * dEa * dEb / (dEa * dRb + dEb * dRa);
	}
};

std::pair<bool, std::pair<double, double> > ISwModels::FrictionForce
	(const double& dStaticFrictionCoefficient,
	 const double& dKineticFrictionCoefficient,
	 const double& dFrictionSwitch,
	 const double& dRelativeShearVelocity,
	 const double& dNormalForce                )
{
	if (dRelativeShearVelocity <= dFrictionSwitch)
	{
		return
			std::make_pair
			(false,
			std::make_pair
				(dStaticFrictionCoefficient  * dNormalForce,
				 dKineticFrictionCoefficient * dNormalForce ));
	}
	else
	{
		return
			std::make_pair
			(true,
			std::make_pair
				(dStaticFrictionCoefficient  * dNormalForce,
				 dKineticFrictionCoefficient * dNormalForce ));
	}
};

std::pair<bool, std::pair<double, double> > ISwModels::FrictionForce
	(const double& dStaticFrictionCoefficient,
	 const double& dKineticFrictionCoefficient,
	 const double& dRelativeShearVelocity,
	 const double& dNormalForce                )
{
	if (dRelativeShearVelocity == 0.0)
	{
		return
			std::make_pair
			(false,
			std::make_pair
				(dStaticFrictionCoefficient  * dNormalForce,
				 dKineticFrictionCoefficient * dNormalForce ));
	}
	else
	{
		return
			std::make_pair
			(true,
			std::make_pair
				(dStaticFrictionCoefficient  * dNormalForce,
				 dKineticFrictionCoefficient * dNormalForce ));
	}
};

njr::Vector3d ISwModels::ShearForceRotation
	(const njr::Vector3d& vShearForce, const njr::Vector3d& vNewDirection)
{
	if(vShearForce.Dot(vNewDirection) <= 0.0)
	{
		return njr::Vector3d(-vShearForce.length() * vNewDirection.direction());
	}
	else
	{
		return njr::Vector3d(vShearForce.length() * vNewDirection.direction());
	}
};

double ISwModels::WetDampingRatioLegendre2006
	(double dDryRestitutionCoefficient, double dBinaryStokesNumber)
{
	if((dBinaryStokesNumber == 0.0) || (dDryRestitutionCoefficient == 0.0))
		return 1.0;

	double dWetRestitutionCoefficient
		= dDryRestitutionCoefficient * exp(-35.0/dBinaryStokesNumber);

	if (dWetRestitutionCoefficient == 0.0)
		return 1.0;

	double beta_c = log(dWetRestitutionCoefficient) / njr::dPI;
	return (-beta_c / sqrt(1.0 + beta_c * beta_c));
};

};   // namespace vedo
