#include <aries/utility/Constants.h>
#include <vedo/common/ISwH.h>
#include <cmath>

namespace vedo
{

ISwH::ISwH(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml): ImpactSolver(cpiactml)
{
	knHertz = cpiactml->GetIactMechanism("HertzNormalStiffness");
	kn      = ImStatus.Kn();
	Meff    = cpiactml->GetIactMechanism("EquivalentMass");
/*
	if (kn != 0.0)
	{
		dCriticalTimeStep = Constants::fDoublePI * sqrt(Meff / kn);
	}
*/
	ksc1    = cpiactml->GetIactMechanism("ShearStiffnessCoefficient1");
	ksc2    = cpiactml->GetIactMechanism("ShearStiffnessCoefficient2");
	betan   = cpiactml->GetIactMechanism("DampingRatio");
	fk      = cpiactml->GetIactMechanism("KineticFrictionCoefficient");
}

bool ISwH::InitialStep(const ContactDetector *CInfo, DiscreteObject *pdoSlave, DiscreteObject *pdoMaster)
{
	return true;
}

aries::Vector3df ISwH::NextStep(const ContactDetector* CInfo, DiscreteObject* A, DiscreteObject* B, _float_t dt)
{
	// CInfo->GetContactInfo() -> vImpactDirection represents direction of (Position B - Position A)
	const _float_t  dImpDepth     = CInfo->GetContactInfo()->dImpactDepth;
	const aries::Vector3df vImpDirection = CInfo->GetContactInfo()->vImpactDirection;

	if (!(CInfo->GetContactInfo()->bActive))
	{
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
		vImpPoint     = CInfo->GetContactInfo()->vImpactPoint;

		vDepthRadiusA = vImpPoint - dosA->GetPosition();
		vDepthRadiusB = vImpPoint - dosB->GetPosition();
		if(this->pBC)
		{
			this->pBC->DifferenceBoundaryConditions(&vDepthRadiusA);
			this->pBC->DifferenceBoundaryConditions(&vDepthRadiusB);
		}

		vRelativeV    = vVa - vVb + vAVa.cross(vDepthRadiusA) - vAVb.cross(vDepthRadiusB);
		vRelativeVn   = vRelativeV.dot(vImpDirection) * vImpDirection;
		vRelativeVs   = vRelativeV - vRelativeVn;

		if (!ImStatus.Contact())
		{
			if(vRelativeVn.length() == 0.0)
			{
				kn = knHertz;
			}
			else
			{
//				kn = 0.957832746294131 * pow(Meff, 0.2) * pow(knHertz, 0.8) * pow(vRelativeVn.length(), 0.4);
				if (betan < 0.99)
				{
					kn
						= 0.957832746294131
						/ (1.0 - betan * betan)
						* pow(Meff, 0.2)
						* pow(knHertz, 0.8)
						* pow(vRelativeVn.length(), 0.4);
				}
				else
				{
					kn
						= 479.155951122633
						* pow(Meff, 0.2)
						* pow(knHertz, 0.8)
						* pow(vRelativeVn.length(), 0.4);
				}
			}

			ImStatus.SetKn(kn);
			bool bContact = true;
			ImStatus.SetContact(bContact);
		}
		else
		{
			kn = ImStatus.Kn();
		}

		_float_t NormalCriticalDamping = 2.0 * sqrt(Meff * kn);
		_float_t ks = ksc1 * pow(ksc2 * fabs(kn * dImpDepth), 1.0/3.0);

/*
		dCriticalTimeStep = ism->CriticalTimeStep(Meff, kn, ks);
		#ifdef _DEBUG
			std::cout << "Critical time step = " << dCriticalTimeStep << std::endl;
		#endif   // _DEBUG
*/

		vForceAn = ism->NormalForce(kn, NormalCriticalDamping*betan, dImpDepth, vImpDirection, vRelativeVn);
		_float_t FrictionForce = fk * kn * dImpDepth;

		vForceAs = ism->ShearForceRotation(ImStatus.ShearForce(), vRelativeVs) - ks * dt * vRelativeVs;
		ImStatus.SetShearForce(vForceAs);
		if (vForceAs.length() >= FrictionForce)
		{
			// Sliding
			vForceAs = FrictionForce * vForceAs.direction();
		}
	}
	else
	{
		vForceAs.set(0.0, 0.0, 0.0);
		ImStatus.CleanContactStatusAndImpact();
	}

	// Calculate the Impact force
	vForceA         = vForceAn + vForceAs;
	ImpactA         = vForceA  * dt;
	// Calculate the Angular impact
	vAngularImpactA =  dt * vDepthRadiusA.cross(vForceA);
	vAngularImpactB = -dt * vDepthRadiusB.cross(vForceA);

    ImStatus.SetImpactInformation(-1.0 * ImpactA, vAngularImpactB);

	A->AddImpact(ImpactA, vAngularImpactA);
	B->AddImpact(-1.0 * ImpactA, vAngularImpactB);

	return ImpactA;
}

}   // namespace vedo
