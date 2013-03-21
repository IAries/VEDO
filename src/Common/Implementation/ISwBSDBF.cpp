#include <Common/Interfaces/ISwBSDBF.h>
#include <cmath>

namespace VEDO
{

ISwBSDBF::ISwBSDBF
	(const DiscreteObject* cpdoSlave,
	const DiscreteObject* cpdoMaster,
	const IactModel* cpiactml) : ImpactSolver(cpiactml)
{
	kn = cpiactml->GetIactMechanism("NormalStiffness");
	ks = cpiactml->GetIactMechanism("ShearStiffness");
	std::pair<double, double> CriticalDamping
		= ism
			->CriticalDamping
				(cpdoSlave->GetSudoMass(), cpdoMaster->GetSudoMass(), kn, ks);
	cn = (cpiactml->GetIactMechanism("NormalDampingRatio")) * CriticalDamping.first;
	cs = (cpiactml->GetIactMechanism("ShearDampingRatio")) * CriticalDamping.second;
	bn = cpiactml->GetIactMechanism("NormalBondStrength");
	bs = cpiactml->GetIactMechanism("ShearBondStrength");
	fc = cpiactml->GetIactMechanism("FrictionCoefficient");
};

bool ISwBSDBF::InitialStep
	(const ContactDetector *CInfo,
	DiscreteObject *pdoSlave,
	DiscreteObject *pdoMaster)
{
	return true;
};

NJR::NJRvector3d ISwBSDBF::NextStep
	(const ContactDetector* CInfo,
	DiscreteObject* A,
	DiscreteObject* B,
	double dt                     )
{
	/**********************************************************************
	 * CInfo->GetContactInfo() -> vImpactDirection
	 * represents direction of (Position B - Position A)
	 **********************************************************************/
	const double      dImpDepth     = CInfo->GetContactInfo()->dImpactDepth;
	const NJR::NJRvector3d vImpDirection = CInfo->GetContactInfo()->vImpactDirection;

	if (   (!(CInfo->GetContactInfo()->bActive))
		&& (!(ism->NormalBond(kn, cn, bn, dImpDepth))) )
	{
		// Normal bond break
		//ImStatus.stage = 2;
		ImStatus.Clean();
		return NJR::NJRvector3d(NJRDXF::ZERO);
	}

	const DOStatus*   dosA = A->GetDOStatus();
	const DOStatus*   dosB = B->GetDOStatus();

	const NJR::NJRvector3d vIa  = A->GetMassMomentInertia();
	const NJR::NJRvector3d vIb  = B->GetMassMomentInertia();

	const NJR::NJRvector3d vVa  = dosA->GetVelocity();
	const NJR::NJRvector3d vVb  = dosB->GetVelocity();
	const NJR::NJRvector3d vAVa = dosA->GetAngularVelocity();
	const NJR::NJRvector3d vAVb = dosB->GetAngularVelocity();

	NJR::NJRvector3d vImpPoint;
	NJR::NJRvector3d vDepthRadiusA, vDepthRadiusB;
	NJR::NJRvector3d vRelativeV;                 // The relative velocity std::vector of A respect to B in the contact point
	NJR::NJRvector3d vRelativeVn, vRelativeVs;   // Normal and tangential component of vRelativeV
	NJR::NJRvector3d vForceAn, vForceAs;         // The force in normal/shear component of A
	NJR::NJRvector3d vForceA, ImpactA;           // The total "impact force" and "impact" on A
	NJR::NJRvector3d vAngMomentA, vAngMomentB;   // Angular impact

	if (dImpDepth > 0.0)
	{
		// Normal bond inactive
		//ImStatus.stage = 2;
		vImpPoint     = CInfo->GetContactInfo()->vImpactPoint;
		vDepthRadiusA = vImpPoint - dosA->GetPosition();
		vDepthRadiusB = vImpPoint - dosB->GetPosition();
		vRelativeV
			= vVa - vVb + vAVa.Cross(vDepthRadiusA) - vAVb.Cross(vDepthRadiusB);
		vRelativeVn   = (vRelativeV % vImpDirection) * vImpDirection;
		vRelativeVs   = vRelativeV - vRelativeVn;

		vForceAn
			= ism->NormalForce(kn, cn, dImpDepth, vImpDirection, vRelativeVn);

		// Default model: contact-bond model
		vForceAs
			= ism->ShearForceRotation(ImStatus.ShearForce(), vRelativeVs)
			- ks * dt * vRelativeVs;
		double dForceAsMax = fc * kn * dImpDepth;
		if (bs == 0.0)
		{
			if (vForceAs.length() > dForceAsMax)
			{
				// Sliding (slip model)
				vForceAs = dForceAsMax * vForceAs.direction();
				ImStatus.SetShearForce(vForceAs);
			}
			else
			{
				ImStatus.SetShearForce(vForceAs);
				vForceAs -= cs * vRelativeVs;
			}
		}
		else
		{
			if (vForceAs.length() > bs)
			{
				// Shear bond breaks, switch to slip model (sliding)
				vForceAs = dForceAsMax * vForceAs.direction();
				ImStatus.SetShearForce(vForceAs);
			}
			else
			{
				ImStatus.SetShearForce(vForceAs);
				vForceAs -= cs * vRelativeVs;
			}
		}

		bool bContact = true;
		ImStatus.SetContact(bContact);
	}
	else
	{
		// Normal bond active
		//ImStatus.stage = 1;
		vRelativeV  = vVa - vVb;
		vRelativeVn = (vRelativeV % vImpDirection) * vImpDirection;
		vRelativeVs = vRelativeV - vRelativeVn;
		vForceAn    = ism->NormalForce(kn, cn, bn, dImpDepth, vImpDirection, vRelativeVn);
		vForceAs.Set(0.0, 0.0, 0.0);
		ImStatus.Clean();
	};

	// Calculate the Impact force
	vForceA = vForceAn + vForceAs;
	ImpactA = vForceA  * dt;

	// Calculate the Angular impact
	vAngMomentA =  vDepthRadiusA.Cross(vForceA);
	vAngMomentB = -vDepthRadiusB.Cross(vForceA);

	A->AddImpact(ImpactA, vAngMomentA * dt);
	B->AddImpact(-1.0 * ImpactA, vAngMomentB * dt);

	return ImpactA;
};

};   // namespace VEDO
