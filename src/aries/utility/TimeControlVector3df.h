#ifndef _ARIES_TIME_CONTROL_VECTOR3DF_H
#define _ARIES_TIME_CONTROL_VECTOR3DF_H

#include <aries/utility/EFSTimeHistory.h>
#include <aries/utility/DataType.h>
#include <aries/utility/Vector3df.h>
#include <iostream>
#include <vector>

namespace aries
{

class TimeControlVector3df
{
public:

	TimeControlVector3df();

	TimeControlVector3df
		(const Vector3df& NewValue,
		 EFSTimeHistory<_float_t>* TimeHistoryX,
		 EFSTimeHistory<_float_t>* TimeHistoryY,
		 EFSTimeHistory<_float_t>* TimeHistoryZ );

	TimeControlVector3df(const TimeControlVector3df&);

	const TimeControlVector3df& operator = (const TimeControlVector3df&);

	const TimeControlVector3df& operator = (const Vector3df&);

	const TimeControlVector3df& operator += (const Vector3df&);

	const TimeControlVector3df& operator -= (const Vector3df&);

	const TimeControlVector3df& operator *= (const _float_t&);

	~TimeControlVector3df();

	Vector3df TimeValue(const _float_t& fTime) const;

	void Update(const _float_t& fTime);

	inline const Vector3df GetBaseValue() const
	{
		return _BaseValue;
	}

	inline const EFSTimeHistory<_float_t>* GetTimeHistoryX() const
	{
		return _TimeHistoryX;
	}

	inline const EFSTimeHistory<_float_t>* GetTimeHistoryY() const
	{
		return _TimeHistoryY;
	}

	inline const EFSTimeHistory<_float_t>* GetTimeHistoryZ() const
	{
		return _TimeHistoryZ;
	}

	Vector3df Value;

private:

	Vector3df _BaseValue;

	EFSTimeHistory<_float_t>* _TimeHistoryX;

	EFSTimeHistory<_float_t>* _TimeHistoryY;

	EFSTimeHistory<_float_t>* _TimeHistoryZ;
};

}   // namespace aries



std::ostream& operator << (std::ostream&, const aries::TimeControlVector3df&);

#endif   // _ARIES_TIME_CONTROL_VECTOR3DF_H
