#include <aries/utility/Constants.h>
#include <vedo/common/ISwHertz.h>
#include <cmath>

namespace vedo
{

ISwHertz::ISwHertz(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml):
	ImpactSolver(cpiactml)
{
	kn = cpiactml->GetIactMechanism("NormalStiffness");
	cn = cpiactml->GetIactMechanism("DampingCoefficient");
}

bool ISwHertz::InitialStep(const ContactDetector *CInfo, DiscreteObject *pdoSlave, DiscreteObject *pdoMaster)
{
	return true;
}

Vector3df ISwHertz::NextStep(const ContactDetector* CInfo, DiscreteObject* A, DiscreteObject* B, _float_t dt)
{
	// CInfo->GetContactInfo() -> vImpactDirection represents direction of (Position B - Position A)
	const _float_t  dImpDepth     = CInfo->GetContactInfo()->dImpactDepth;
	const Vector3df vImpDirection = CInfo->GetContactInfo()->vImpactDirection;

	if (!(CInfo->GetContactInfo()->bActive))
	{
		ImStatus.Clean();
		return Vector3df();
	}

    ImStatus.SetContactInformation(CInfo->GetContactInfo());

	const DOStatus*     dosA = A->GetDOStatus();
	const DOStatus*     dosB = B->GetDOStatus();

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
		vImpPoint     = CInfo->GetContactInfo()->vImpactPoint;

		vDepthRadiusA = vImpPoint - dosA->GetPosition();
		vDepthRadiusB = vImpPoint - dosB->GetPosition();
		if(this->pBC)
		{
			this->pBC->DifferenceBoundaryConditions(&vDepthRadiusA);
			this->pBC->DifferenceBoundaryConditions(&vDepthRadiusB);
		}

		vRelativeV  = vVa - vVb + vAVa.cross(vDepthRadiusA) - vAVb.cross(vDepthRadiusB);
		vRelativeVn = vRelativeV.dot(vImpDirection) * vImpDirection;
		vRelativeVs = vRelativeV - vRelativeVn;

		if (!ImStatus.Contact())
		{
			bool bContact = true;
			ImStatus.SetContact(bContact);
			_float_t dInitialVelocity = vRelativeVn.length();
			ImStatus.SetInitialVelocity(dInitialVelocity);
		}

		vForceAn = ism->NormalForceHertzSpring(kn, cn, dImpDepth, vImpDirection, vRelativeVn);
	}
	else
	{
		vForceAs.set(0.0, 0.0, 0.0);
		ImStatus.CleanContactStatusAndImpact();
	}

	// Calculate the Impact force
	ImpactA = vForceAn * dt;

    ImStatus.SetImpactInformation(-ImpactA, Vector3df());

	A->AddImpact( ImpactA);
	B->AddImpact(-ImpactA);

	return ImpactA;
}

}   // namespace vedo
