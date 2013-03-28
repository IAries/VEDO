#ifndef _CONTACT_DETECTOR_H
#define _CONTACT_DETECTOR_H

#include <FrameWork/Interfaces/Boundary.h>
#include <FrameWork/Interfaces/DiscreteObject.h>
#include <FrameWork/Interfaces/IactModel.h>

namespace VEDO
{

struct ContactInfo
{
	NJR::Vector3d vImpactDirection;
	NJR::Vector3d vSlideDirection;
	NJR::Vector3d vCenterToCenter;
	NJR::Vector3d vImpactPoint;
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

};   // namespace VEDO

#endif // _CONTACT_DETECTOR_H
