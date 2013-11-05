#ifndef _INTERACTION_H
#define _INTERACTION_H

#include <Framework/Interfaces/Boundary.h>
#include <Framework/Interfaces/ContactDetector.h>
#include <Framework/Interfaces/DiscreteObject.h>
#include <Framework/Interfaces/ImpactSolver.h>

namespace vedo
{

class Interaction
{

public:

	Interaction
		(DiscreteObject* doslave,
		DiscreteObject* domaster,
		ContactDetector* cd,
		ImpactSolver* is);

	~Interaction ();

	inline const DiscreteObject* GetMaster() const
	{
		return pdoMaster;
	};

	inline const DiscreteObject* GetSlave() const
	{
		return pdoSlave;
	};

	inline const ContactDetector* GetDetector() const
	{
		return pcd;
	};

	inline const ImpactSolver* GetSolver() const
	{
		return pis;
	};

	inline void CleanSolverStatus()
	{
		pis->CleanStatus();
	};

	inline const ContactInfo* GetContactInfo() const
	{
		return pcd->GetContactInfo();
	};

	inline bool IsActive() const
	{
		return pcd->GetContactInfo()->bActive;
	};

	void DetectContact();

	void SolveImpact(const double dt);

	void ControlError();

	virtual std::string type () const
	{
		return "Interaction";
	};

	inline const ImpactStatus* GetImpactStatus() const
	{
		return pis->GetImpactStatus();
	};

	inline void SetImpactStatus(const ImpactStatus* status)
	{
		pis->SetImpactStatus(status);
	};

	inline const double* RetrieveUserDefinedValue()
	{
		return pis->RetrieveUserDefinedValue();
	};

	inline void SetPeriodicBoundaryConditions(const Boundary* pPBC)
	{
		pcd->SetPeriodicBoundaryConditions(pPBC);
		pis->SetPeriodicBoundaryConditions(pPBC);
	};

private:

	DiscreteObject*  pdoMaster;
	DiscreteObject*  pdoSlave;
	ContactDetector* pcd;
	ImpactSolver*    pis;

	Interaction(const Interaction&);
	Interaction& operator = (const Interaction&);
	Interaction();
};

};   // namespace vedo

#endif // _INTERACTION_H
