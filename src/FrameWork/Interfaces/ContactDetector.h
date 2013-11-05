#ifndef _CONTACT_DETECTOR_H
#define _CONTACT_DETECTOR_H

#include <Framework/Interfaces/Boundary.h>
#include <Framework/Interfaces/DiscreteObject.h>
#include <Framework/Interfaces/IactModel.h>

namespace vedo
{

struct ContactInfo
{
	njr::Vector3d vImpactDirection;
	njr::Vector3d vSlideDirection;
	njr::Vector3d vCenterToCenter;
	njr::Vector3d vImpactPoint;
	double      dImpactDepth;
	double      dDistance;
	double      dOverlapArea;
	bool        bActive;
	bool        bUnBalance;
};



class ContactDetector
{

public:

	ContactDetector();

	virtual ~ContactDetector()
	{
	};

	inline const ContactInfo* GetContactInfo() const
	{
		return &cInfo;
	};

	inline void SetPeriodicBoundaryConditions(const Boundary* bc)
	{
		pBC = bc;
	};

	virtual void Detect
		(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster) = 0;

	virtual void CalDistance
		(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster) = 0;

	virtual std::string type () const
	{
		return "ContactDetector";
	};

protected:

	const Boundary* pBC;

	ContactInfo cInfo;
};

};   // namespace vedo

#endif // _CONTACT_DETECTOR_H
