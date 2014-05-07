#include <vedo/Constants.h>
#include <vedo/common/interfaces/ISwHertz.h>
#include <cmath>

namespace vedo
{

ISwHertz::ISwHertz
	(const DiscreteObject* cpdoSlave,
	const DiscreteObject* cpdoMaster,
	const IactModel* cpiactml) : ImpactSolver(cpiactml)
{
	kn      = cpiactml->GetIactMechanism("NormalStiffness");
	cn      = cpiactml->GetIactMechanism("DampingCoefficient");
}

bool ISwHertz::InitialStep
	(const ContactDetector *CInfo,
	DiscreteObject *pdoSlave,
	DiscreteObject *pdoMaster)
{
	return true;
}

njr::Vector3d ISwHertz::NextStep
	(const ContactDetector* CInfo,
	DiscreteObject* A,
	DiscreteObject* B,
	double dt                     )
{
	/**********************************************************************
	 * CInfo->GetContactInfo() -> vImpactDirection
	 * represents direction of (Position B - Position A)
	 **********************************************************************/
	const double        dImpDepth     = CInfo->GetContactInfo()->dImpactDepth;
	const njr::Vector3d vImpDirection = CInfo->GetContactInfo()->vImpactDirection;

	if (!(CInfo->GetContactInfo()->bActive))
	{
		ImStatus.Clean();
		return njr::ZERO;
	}

    ImStatus.SetContactInformation(CInfo->GetContactInfo());

	const DOStatus*   dosA = A->GetDOStatus();
	const DOStatus*   dosB = B->GetDOStatus();

	const njr::Vector3d vIa  = A->GetMassMomentInertia();
	const njr::Vector3d vIb  = B->GetMassMomentInertia();

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

		vRelativeV
			= vVa - vVb + vAVa.Cross(vDepthRadiusA) - vAVb.Cross(vDepthRadiusB);
		vRelativeVn = (vRelativeV % vImpDirection) * vImpDirection;
		vRelativeVs = vRelativeV - vRelativeVn;

		if (!ImStatus.Contact())
		{
			bool bContact = true;
			ImStatus.SetContact(bContact);
			double dInitialVelocity = vRelativeVn.length();
			ImStatus.SetInitialVelocity(dInitialVelocity);
		}

		vForceAn
			= ism->NormalForceHertzSpring
				(kn, cn, dImpDepth, vImpDirection, vRelativeVn);
	}
	else
	{
		vForceAs.Set(0.0, 0.0, 0.0);
		ImStatus.CleanContactStatusAndImpact();
	}

	// Calculate the Impact force
	ImpactA = vForceAn  * dt;

    ImStatus.SetImpactInformation(-1.0 * ImpactA, njr::Vector3d(njr::ZERO));

	A->AddImpact(ImpactA, njr::Vector3d(njr::ZERO));
	B->AddImpact(-1.0 * ImpactA, njr::Vector3d(njr::ZERO));

	return ImpactA;
};

};   // namespace vedo
