#include <FrameWork/Interfaces/ISwModels.h>
#include <cmath>

using namespace std;

double ISwModels::CriticalTimeStep(const double& dMeff, const double& dK)
{
	return 6.28318530717959 * sqrt(dMeff / dK);
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
			return 6.28318530717959 * sqrt(dMeff / dKt);
		}
	}
	else
	{
		if (dKt == 0.0)
		{
			return 6.28318530717959 * sqrt(dMeff / dKn);
		}
		else
		{
			return 6.28318530717959 * sqrt(dMeff / max(dKn, dKt));
		}
	}
};

pair<double, double> ISwModels::CriticalDamping
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
	return make_pair(2.0 * sqrt(dMe * dKn), 2.0 * sqrt(dMe * dKs));
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

NJRvector3d ISwModels::NormalForceHertzSpring
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dImpactDepth,
	 const NJRvector3d& vImpactDirection,
	 const NJRvector3d& vRelativeNormalVelocity)
{
	return NJRvector3d
		(- dKn * pow(dImpactDepth, 1.5) * vImpactDirection
		 - dCn * vRelativeNormalVelocity                       );
};

NJRvector3d ISwModels::NormalForce
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dImpactDepth,
	 const NJRvector3d& vImpactDirection,
	 const NJRvector3d& vRelativeNormalVelocity)
{
	return NJRvector3d
		(- dKn * dImpactDepth * vImpactDirection
		 - dCn * vRelativeNormalVelocity        );
};

NJRvector3d ISwModels::NormalForceNoTension
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dImpactDepth,
	 const NJRvector3d& vImpactDirection,
	 const NJRvector3d& vRelativeNormalVelocity)
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		NJRvector3d vSpringForce = - dKn * dImpactDepth * vImpactDirection;
		NJRvector3d vShearViscousDampingForce = - dCn * vRelativeNormalVelocity;
		if(vShearViscousDampingForce.length() > vSpringForce.length())
			return NJRvector3d(ZERO);
		else
			return vSpringForce + vShearViscousDampingForce;
	}
	else
	{
		// Elements move colser to each others
		return NJRvector3d
			(- dKn * dImpactDepth * vImpactDirection
			 - dCn * vRelativeNormalVelocity        );
	}
};

NJRvector3d ISwModels::NormalForce
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dBn,
	 const double&      dImpactDepth,
	 const NJRvector3d& vImpactDirection,
	 const NJRvector3d& vRelativeNormalVelocity)
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		return NJRvector3d
			(- dKn * dImpactDepth * vImpactDirection
			 - dCn                * vRelativeNormalVelocity);
	}
	else
	{
		// Elements move colser to each others
		return NJRvector3d(- dCn * vRelativeNormalVelocity);
	}
};

NJRvector3d ISwModels::NormalForceNoTension
	(const double&      dKn,
	 const double&      dCn,
	 const double&      dBn,
	 const double&      dImpactDepth,
	 const NJRvector3d& vImpactDirection,
	 const NJRvector3d& vRelativeNormalVelocity)
{
	if ((vRelativeNormalVelocity % vImpactDirection) < 0.0)
	{
		// Elements move far away from each others
		NJRvector3d vSpringForce = - dKn * dImpactDepth * vImpactDirection;
		NJRvector3d vShearViscousDampingForce = - dCn * vRelativeNormalVelocity;
		if(vShearViscousDampingForce.length() > vSpringForce.length())
			return NJRvector3d(ZERO);
		else
			return vSpringForce + vShearViscousDampingForce;
	}
	else
	{
		// Elements move colser to each others
		return NJRvector3d(- dCn * vRelativeNormalVelocity);
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

pair<bool, pair<double, double> > ISwModels::FrictionForce
	(const double& dStaticFrictionCoefficient,
	 const double& dKineticFrictionCoefficient,
	 const double& dFrictionSwitch,
	 const double& dRelativeShearVelocity,
	 const double& dNormalForce                )
{
	if (dRelativeShearVelocity <= dFrictionSwitch)
	{
		return
			make_pair
			(false,
			make_pair
				(dStaticFrictionCoefficient  * dNormalForce,
				 dKineticFrictionCoefficient * dNormalForce ));
	}
	else
	{
		return
			make_pair
			(true,
			make_pair
				(dStaticFrictionCoefficient  * dNormalForce,
				 dKineticFrictionCoefficient * dNormalForce ));
	}
};

pair<bool, pair<double, double> > ISwModels::FrictionForce
	(const double& dStaticFrictionCoefficient,
	 const double& dKineticFrictionCoefficient,
	 const double& dRelativeShearVelocity,
	 const double& dNormalForce                )
{
	if (dRelativeShearVelocity == 0.0)
	{
		return
			make_pair
			(false,
			make_pair
				(dStaticFrictionCoefficient  * dNormalForce,
				 dKineticFrictionCoefficient * dNormalForce ));
	}
	else
	{
		return
			make_pair
			(true,
			make_pair
				(dStaticFrictionCoefficient  * dNormalForce,
				 dKineticFrictionCoefficient * dNormalForce ));
	}
};

NJRvector3d ISwModels::ShearForceRotation
	(const NJRvector3d& vShearForce, const NJRvector3d& vNewDirection)
{
	if(vShearForce.Dot(vNewDirection) <= 0.0)
	{
		return NJRvector3d(-vShearForce.length() * vNewDirection.direction());
	}
	else
	{
		return NJRvector3d(vShearForce.length() * vNewDirection.direction());
	}
};
