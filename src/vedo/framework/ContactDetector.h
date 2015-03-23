#ifndef _CONTACT_DETECTOR_H
#define _CONTACT_DETECTOR_H

#include <vedo/framework/Boundary.h>
#include <vedo/framework/DiscreteObject.h>
#include <vedo/framework/IactModel.h>
#include <vedo/framework/DataType.h>

namespace vedo
{

struct ContactInfo
{
	aries::Vector3df vImpactDirection;
	aries::Vector3df vSlideDirection;
	aries::Vector3df vCenterToCenter;
	aries::Vector3df vImpactPoint;
	_float_t  dImpactDepth;
	_float_t  dDistance;
	_float_t  dOverlapArea;
	bool          bActive;
	bool          bUnBalance;
};



class ContactDetector
{

public:

	ContactDetector();

	virtual ~ContactDetector()
	{
	}

	inline const ContactInfo* GetContactInfo() const
	{
		return &cInfo;
	}

	inline void SetPeriodicBoundaryConditions(const Boundary* bc)
	{
		pBC = bc;
	}

	virtual void Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster) = 0;

	virtual void CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster) = 0;

	virtual std::string type () const
	{
		return "ContactDetector";
	}

protected:

	const Boundary* pBC;

	ContactInfo cInfo;
};

}   // namespace vedo

#endif // _CONTACT_DETECTOR_H
