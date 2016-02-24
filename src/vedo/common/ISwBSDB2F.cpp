#include <vedo/common/ISwBSDB2F.h>
#include <cmath>

namespace vedo
{

ISwBSDB2F::ISwBSDB2F(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml):
	ImpactSolver(cpiactml)
{
	kn = cpiactml->GetIactMechanism("NormalStiffness");
	ks = cpiactml->GetIactMechanism("ShearStiffness");
	std::pair<_float_t, _float_t> CriticalDamping
		= ism->CriticalDamping(cpdoSlave->GetSudoMass(), cpdoMaster->GetSudoMass(), kn, ks);
	cn = (cpiactml->GetIactMechanism("NormalDampingRatio")) * CriticalDamping.first;
	cs = (cpiactml->GetIactMechanism("ShearDampingRatio")) * CriticalDamping.second;
	bn = cpiactml->GetIactMechanism("NormalBondStrength");
	bs = cpiactml->GetIactMechanism("ShearBondStrength");
	fc = cpiactml->GetIactMechanism("FrictionCoefficient");
}

bool ISwBSDB2F::InitialStep(const ContactDetector *CInfo, DiscreteObject *pdoSlave, DiscreteObject *pdoMaster)
{
	return true;
}

Vector3df ISwBSDB2F::NextStep(const ContactDetector* CInfo, DiscreteObject* A, DiscreteObject* B, _float_t dt)
{
	/*****************************************************
	 * CInfo->GetContactInfo() -> vImpactDirection
	 * represents direction of (Position B - Position A)
	 ****************************************************/
	const _float_t  dImpDepth     = CInfo->GetContactInfo()->dImpactDepth;
	const Vector3df vImpDirection = CInfo->GetContactInfo()->vImpactDirection;

	if ((!(CInfo->GetContactInfo()->bActive)) && (!(ism->NormalBond(kn, cn, bn, dImpDepth))))
	{
		// Normal bond break
		//ImStatus.stage = 2;
		ImStatus.Clean();
		return Vector3df();
	}

    ImStatus.SetContactInformation(CInfo->GetContactInfo());

	const DOStatus*   dosA = A->GetDOStatus();
	const DOStatus*   dosB = B->GetDOStatus();

	//const Vector3df vIa  = A->GetMassMomentInertia();
	//const Vector3df vIb  = B->GetMassMomentInertia();

	const Vector3df vVa  = dosA->GetVelocity();
	const Vector3df vVb  = dosB->GetVelocity();
	const Vector3df vAVa = dosA->GetAngularVelocity();
	const Vector3df vAVb = dosB->GetAngularVelocity();

	Vector3df vImpPoint;
	Vector3df vDepthRadiusA, vDepthRadiusB;
	Vector3df vRelativeV;                         // The relative velocity std::vector of A respect to B in the contact point
	Vector3df vRelativeVn, vRelativeVs;           // Normal and tangential component of vRelativeV
	Vector3df vForceAn, vForceAs;                 // The force in normal/shear component of A
	Vector3df vForceA, ImpactA;                   // The total "impact force" and "impact" on A
	Vector3df vAngularImpactA, vAngularImpactB;   // Angular impact

	if (dImpDepth > 0.0)
	{
		// Normal bond inactive
		//ImStatus.stage = 2;
		vImpPoint     = CInfo->GetContactInfo()->vImpactPoint;

		vDepthRadiusA = vImpPoint - dosA->GetPosition();
		vDepthRadiusB = vImpPoint - dosB->GetPosition();
		if(this->pBC)
		{
			this->pBC->DifferenceBoundaryConditions(&vDepthRadiusA);
			this->pBC->DifferenceBoundaryConditions(&vDepthRadiusB);
		}

		vRelativeV
			= vVa - vVb + vAVa.cross(vDepthRadiusA) - vAVb.cross(vDepthRadiusB);
		vRelativeVn   = vRelativeV.dot(vImpDirection) * vImpDirection;
		vRelativeVs   = vRelativeV - vRelativeVn;

		vForceAn
			= ism->NormalForce(kn, cn, dImpDepth, vImpDirection, vRelativeVn);

		// Default model: contact-bond model
		vForceAs
			= ism->ShearForceRotation(ImStatus.ShearForce(), vRelativeVs)
			- ks * dt * vRelativeVs;
		_float_t dForceAsMax = fc * kn * dImpDepth;
		if (bs == 0.0)
		{
			if (vForceAs.length() > dForceAsMax)
			{
				// Sliding (slip model)
				vForceAs = dForceAsMax * vForceAs.direction();
			}
			else
			{
				vForceAs -= cs * vRelativeVs;
			}
		}
		else
		{
			if (vForceAs.length() > bs)
			{
				// Shear bond breaks, switch to slip model (sliding)
				vForceAs = dForceAsMax * vForceAs.direction();
			}
			else
			{
				vForceAs -= cs * vRelativeVs;
			}
		}
		ImStatus.SetShearForce(vForceAs);

		bool bContact = true;
		ImStatus.SetContact(bContact);
	}
	else
	{
	    if(ImStatus.LastBond())
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
