#include <vedo/constants/interfaces/Constants.h>
#include <vedo/common/interfaces/ISwH.h>
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
		dCriticalTimeStep = Constants::dDoublePI * sqrt(Meff / kn);
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

njr::Vector3d ISwH::NextStep(const ContactDetector* CInfo, DiscreteObject* A, DiscreteObject* B, vedo_float_t dt)
{
	// CInfo->GetContactInfo() -> vImpactDirection represents direction of (Position B - Position A)
	const vedo_float_t  dImpDepth     = CInfo->GetContactInfo()->dImpactDepth;
	const njr::Vector3d vImpDirection = CInfo->GetContactInfo()->vImpactDirection;

	if (!(CInfo->GetContactInfo()->bActive))
	{
		ImStatus.Clean();
		return njr::ZERO;
	}

	ImStatus.SetContactInformation(CInfo->GetContactInfo());

	const DOStatus*     dosA = A->GetDOStatus();
	const DOStatus*     dosB = B->GetDOStatus();

	//const njr::Vector3d vIa  = A->GetMassMomentInertia();
	//const njr::Vector3d vIb  = B->GetMassMomentInertia();

	const njr::Vector3d vVa  = dosA->GetVelocity();
	const njr::Vector3d vVb  = dosB->GetVelocity();
	const njr::Vector3d vAVa = dosA->GetAngularVelocity();
	const njr::Vector3d vAVb = dosB->GetAngularVelocity();

	njr::Vector3d vImpPoint;
	njr::Vector3d vDepthRadiusA, vDepthRadiusB;
	njr::Vector3d vRelativeV;                         // The relative velocity std::vector of A respect to B in the contact point
	njr::Vector3d vRelativeVn, vRelativeVs;           // Normal and tangential component of vRelativeV
	njr::Vector3d vForceAn, vForceAs;                 // The force in normal/shear component of A
	njr::Vector3d vForceA, ImpactA;                   // The total "impact force" and "impact" on A
	njr::Vector3d vAngularImpactA, vAngularImpactB;   // Angular impact

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

		vRelativeV    = vVa - vVb + vAVa.Cross(vDepthRadiusA) - vAVb.Cross(vDepthRadiusB);
		vRelativeVn   = (vRelativeV % vImpDirection) * vImpDirection;
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

		vedo_float_t NormalCriticalDamping = 2.0 * sqrt(Meff * kn);
		vedo_float_t ks = ksc1 * pow(ksc2 * fabs(kn * dImpDepth), 1.0/3.0);

/*
		dCriticalTimeStep = ism->CriticalTimeStep(Meff, kn, ks);
		#ifdef _VEDO_DEBUG
			std::cout << "Critical time step = " << dCriticalTimeStep << std::endl;
		#endif   // _VEDO_DEBUG
*/

		vForceAn = ism->NormalForce(kn, NormalCriticalDamping*betan, dImpDepth, vImpDirection, vRelativeVn);
		vedo_float_t FrictionForce = fk * kn * dImpDepth;

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
		vForceAs.Set(0.0, 0.0, 0.0);
		ImStatus.CleanContactStatusAndImpact();
	}

	// Calculate the Impact force
	vForceA         = vForceAn + vForceAs;
	ImpactA         = vForceA  * dt;
	// Calculate the Angular impact
	vAngularImpactA =  dt * vDepthRadiusA.Cross(vForceA);
	vAngularImpactB = -dt * vDepthRadiusB.Cross(vForceA);

    ImStatus.SetImpactInformation(-1.0 * ImpactA, vAngularImpactB);

	A->AddImpact(ImpactA, vAngularImpactA);
	B->AddImpact(-1.0 * ImpactA, vAngularImpactB);

	return ImpactA;
}

}   // namespace vedo
