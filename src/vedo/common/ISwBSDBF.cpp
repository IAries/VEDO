#include <vedo/common/ISwBSDBF.h>
#include <cmath>

namespace vedo
{

ISwBSDBF::ISwBSDBF(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml):
	ImpactSolver(cpiactml)
{
	kn  = cpiactml->GetIactMechanism("NormalStiffness");
	ks  = cpiactml->GetIactMechanism("ShearStiffness");
	std::pair<_float_t, _float_t> CriticalDamping
		= ism->CriticalDamping(cpdoSlave->GetSudoMass(), cpdoMaster->GetSudoMass(), kn, ks);
	cn  = (cpiactml->GetIactMechanism("NormalDampingRatio")) * CriticalDamping.first;
	cs  = (cpiactml->GetIactMechanism("ShearDampingRatio")) * CriticalDamping.second;
	bn  = cpiactml->GetIactMechanism("NormalBondStrength");
	bs  = cpiactml->GetIactMechanism("ShearBondStrength");
	fc  = cpiactml->GetIactMechanism("FrictionCoefficient");
}

bool ISwBSDBF::InitialStep(const ContactDetector *CInfo, DiscreteObject *pdoSlave, DiscreteObject *pdoMaster)
{
	return true;
}

aries::Vector3df ISwBSDBF::NextStep(const ContactDetector* CInfo, DiscreteObject* A, DiscreteObject* B, _float_t dt)
{
	// CInfo->GetContactInfo() -> vImpactDirection represents direction of (Position B - Position A)
	const _float_t  dImpDepth     = CInfo->GetContactInfo()->dImpactDepth;
	const aries::Vector3df vImpDirection = CInfo->GetContactInfo()->vImpactDirection;

	if ((!(CInfo->GetContactInfo()->bActive)) && (!(ism->NormalBond(kn, cn, bn, dImpDepth))))
	{
		// Normal bond break
		//ImStatus.stage = 2;
		ImStatus.Clean();
		return aries::Vector3df();
	}

    ImStatus.SetContactInformation(CInfo->GetContactInfo());

	const DOStatus*     dosA = A->GetDOStatus();
	const DOStatus*     dosB = B->GetDOStatus();

	//const aries::Vector3df vIa  = A->GetMassMomentInertia();
	//const aries::Vector3df vIb  = B->GetMassMomentInertia();

	const aries::Vector3df vVa  = dosA->GetVelocity();
	const aries::Vector3df vVb  = dosB->GetVelocity();
	const aries::Vector3df vAVa = dosA->GetAngularVelocity();
	const aries::Vector3df vAVb = dosB->GetAngularVelocity();

	aries::Vector3df vImpPoint;
	aries::Vector3df vDepthRadiusA, vDepthRadiusB;
	aries::Vector3df vRelativeV;                         // The relative velocity std::vector of A respect to B in the contact point
	aries::Vector3df vRelativeVn, vRelativeVs;           // Normal and tangential component of vRelativeV
	aries::Vector3df vForceAn, vForceAs;                 // The force in normal/shear component of A
	aries::Vector3df vForceA, ImpactA;                   // The total "impact force" and "impact" on A
	aries::Vector3df vAngularImpactA, vAngularImpactB;   // Angular impact

	if (dImpDepth > 0.0)
	{
		// Normal bond inactive
		//ImStatus.stage = 2;
		vImpPoint     = CInfo->GetContactInfo()->vImpactPoint;

		vDepthRadiusA = vImpPoint - dosA->GetPosition();
		vDepthRadiusB = vImpPoint - dosB->GetPosition();
		if (this->pBC)
		{
			this->pBC->DifferenceBoundaryConditions(&vDepthRadiusA);
			this->pBC->DifferenceBoundaryConditions(&vDepthRadiusB);
		}

		vRelativeV    = vVa - vVb + vAVa.cross(vDepthRadiusA) - vAVb.cross(vDepthRadiusB);
		vRelativeVn   = vRelativeV.dot(vImpDirection) * vImpDirection;
		vRelativeVs   = vRelativeV - vRelativeVn;

		vForceAn      = ism->NormalForce(kn, cn, dImpDepth, vImpDirection, vRelativeVn);

		// Default model: contact-bond model
		vForceAs      = ism->ShearForceRotation(ImStatus.ShearForce(), vRelativeVs) - ks * dt * vRelativeVs;
		_float_t dForceAsMax = fc * kn * dImpDepth;
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
		vRelativeVn = vRelativeV.dot(vImpDirection) * vImpDirection;
		vRelativeVs = vRelativeV - vRelativeVn;
		vForceAn    = ism->NormalForce(kn, cn, bn, dImpDepth, vImpDirection, vRelativeVn);
		vForceAs.set(0.0, 0.0, 0.0);
		ImStatus.CleanContactStatusAndImpact();
	}

	// Calculate the Impact force
	vForceA = vForceAn + vForceAs;
	ImpactA = vForceA  * dt;

	// Calculate the Angular impact
	vAngularImpactA =  dt * vDepthRadiusA.cross(vForceA);
	vAngularImpactB = -dt * vDepthRadiusB.cross(vForceA);

    ImStatus.SetImpactInformation(-1.0 * ImpactA, vAngularImpactB);

	A->AddImpact(ImpactA, vAngularImpactA);
	B->AddImpact(-1.0 * ImpactA, vAngularImpactB);

	return ImpactA;
}

}   // namespace vedo
