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
	Vector3df vImpactDirection;
	Vector3df vSlideDirection;
	Vector3df vCenterToCenter;
	Vector3df vImpactPoint;
	_float_t  dImpactDepth;
	_float_t  dDistance;
	_float_t  dOverlapArea;
	bool      bActive;
	bool      bUnBalance;
	_uint_t   uShapeTypeMaster;
	_uint_t   uShapeTypeSlave;
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
