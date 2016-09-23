#include <aries/utility/TimeControlVector3df.h>
#include <aries/utility/DataType.h>
#include <aries/utility/Vector3df.h>
#include <iostream>
#include <vector>

namespace aries
{

TimeControlVector3df::TimeControlVector3df()
{
	_TimeHistoryX = _TimeHistoryY = _TimeHistoryZ = 0;
}

TimeControlVector3df::TimeControlVector3df
	(const Vector3df& NewValue,
	 EFSTimeHistory<_float_t>* TimeHistoryX,
	 EFSTimeHistory<_float_t>* TimeHistoryY,
	 EFSTimeHistory<_float_t>* TimeHistoryZ ):
	_BaseValue(NewValue), Value(NewValue),
	_TimeHistoryX(TimeHistoryX), _TimeHistoryY(TimeHistoryY), _TimeHistoryZ(TimeHistoryZ)
{
}

TimeControlVector3df::TimeControlVector3df(const TimeControlVector3df& tcv)
{
	*this = tcv;
}

const TimeControlVector3df& TimeControlVector3df::operator = (const TimeControlVector3df& tcv)
{
	Value         = tcv.Value;
	_BaseValue    = tcv._BaseValue;
	_TimeHistoryX = tcv._TimeHistoryX;
	_TimeHistoryY = tcv._TimeHistoryY;
	_TimeHistoryZ = tcv._TimeHistoryZ;
	return *this;
}

const TimeControlVector3df& TimeControlVector3df::operator = (const Vector3df& v)
{
	_BaseValue = Value = v;
	_TimeHistoryX = _TimeHistoryY = _TimeHistoryZ = 0;
	return *this;
}

const TimeControlVector3df& TimeControlVector3df::operator += (const Vector3df& v)
{
	_BaseValue += v;
	Value      += v;
	return *this;
}

const TimeControlVector3df& TimeControlVector3df::operator -= (const Vector3df& v)
{
	_BaseValue -= v;
	Value      -= v;
	return *this;
}

const TimeControlVector3df& TimeControlVector3df::operator *= (const _float_t& f)
{
	_BaseValue = f * _BaseValue;
	Value      = f * Value;
	return *this;
}

TimeControlVector3df::~TimeControlVector3df()
{
}

Vector3df TimeControlVector3df::TimeValue(const _float_t& fTime) const
{
	Vector3df NewValue(_BaseValue);
	if (_TimeHistoryX)
	{
		NewValue.SetX((*_TimeHistoryX)(fTime) * _BaseValue.x());
	}
	if (_TimeHistoryY)
	{
		NewValue.SetY((*_TimeHistoryY)(fTime) * _BaseValue.y());
	}
	if (_TimeHistoryZ)
	{
		NewValue.SetZ((*_TimeHistoryZ)(fTime) * _BaseValue.z());
	}
	return NewValue;
}

void TimeControlVector3df::Update(const _float_t& fTime)
{
	if (_TimeHistoryX)
	{
		Value.SetX((*_TimeHistoryX)(fTime) * _BaseValue.x());
	}
	if (_TimeHistoryY)
	{
		Value.SetY((*_TimeHistoryY)(fTime) * _BaseValue.y());
	}
	if (_TimeHistoryZ)
	{
		Value.SetZ((*_TimeHistoryZ)(fTime) * _BaseValue.z());
	}
}

}   // namespace aries



std::ostream& operator << (std::ostream& os, const aries::TimeControlVector3df& tcv)
{
	os << tcv.GetBaseValue() << std::endl;
	if (tcv.GetTimeHistoryX())
	{
		os << "Time history of X (file: " << tcv.GetTimeHistoryX()->FileName() << ", note: " << tcv.GetTimeHistoryX()->FileNote() << ")" << std::endl;
	}
	if (tcv.GetTimeHistoryY())
	{
		os << "Time history of Y (file: " << tcv.GetTimeHistoryY()->FileName() << ", note: " << tcv.GetTimeHistoryY()->FileNote() << ")" << std::endl;
	}
	if (tcv.GetTimeHistoryZ())
	{
		os << "Time history of Z (file: " << tcv.GetTimeHistoryZ()->FileName() << ", note: " << tcv.GetTimeHistoryZ()->FileNote() << ")" << std::endl;
	}
	return os;
}
