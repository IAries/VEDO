#ifndef _CDQUASICYLINDER_2_QUASIPLATE_H
#define _CDQUASICYLINDER_2_QUASIPLATE_H

#include <FrameWork/Interfaces/ContactDetector.h>

class CDQuasiCylinder_QuasiPlate : public ContactDetector
{

public:

	void CalDistance
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	void Detect
		(const DiscreteObject* pdoslave, const DiscreteObject* pdomaster);

	virtual std::string type () const
	{
		return "CDQuasiCylinder_QuasiPlate";
	};
};

#endif // _CDQUASICYLINDER_2_QUASIPLATE_H
