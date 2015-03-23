#ifndef _ARIES_TIME_CONTROL_VARIABLE_H
#define _ARIES_TIME_CONTROL_VARIABLE_H

#include <aries/utility/EFSTimeHistory.h>
#include <aries/utility/DataType.h>
#include <iostream>
#include <vector>

namespace aries
{

template <typename T>
class TimeControlVariable
{
public:

	TimeControlVariable()
	{
		_TimeHistory = 0;
	}

	TimeControlVariable(const T& NewValue, EFSTimeHistory<_float_t>* TimeHistory):
		_BaseValue(NewValue), Value(NewValue), _TimeHistory(TimeHistory)
	{
	}

	TimeControlVariable(const TimeControlVariable& tcv)
	{
		*this = tcv;
	}

	const TimeControlVariable& operator = (const TimeControlVariable& tcv)
	{
		Value        = tcv.Value;
		_BaseValue   = tcv._BaseValue;
		_TimeHistory = tcv._TimeHistory;
		return *this;
	}

	const TimeControlVariable& operator = (const T& tv)
	{
		_BaseValue = Value = tv;
		_TimeHistory = 0;
		return *this;
	}

	const TimeControlVariable& operator += (const T& tv)
	{
		_BaseValue += tv;
		Value      += tv;
		return *this;
	}

	const TimeControlVariable& operator -= (const T& tv)
	{
		_BaseValue -= tv;
		Value      -= tv;
		return *this;
	}

	const TimeControlVariable& operator *= (const T& tv)
	{
		_BaseValue *= tv;
		Value      *= tv;
		return *this;
	}

	~TimeControlVariable()
	{
	}

	T TimeValue(const _float_t& fTime) const
	{
		if (_TimeHistory)
		{
			return (*_TimeHistory)(fTime) * _BaseValue;
		}
		else
		{
			return _BaseValue;
		}
	}

	void Update(const _float_t& fTime)
	{
		if (_TimeHistory)
		{
			Value = (*_TimeHistory)(fTime) * _BaseValue;
		}
	}

	const T GetBaseValue() const
	{
		return _BaseValue;
	}

	const EFSTimeHistory<_float_t>* GetTimeHistory() const
	{
		return _TimeHistory;
	}

	T Value;

private:

	T _BaseValue;

	EFSTimeHistory<_float_t>* _TimeHistory;
};

}   // namespace aries



template <typename T>
std::ostream& operator << (std::ostream& os, const aries::TimeControlVariable<T> & tcv)
{
	os << tcv.GetBaseValue() << std::endl;
	if (tcv.GetTimeHistory())
	{
		os
			<< "Time history (file: " << tcv.GetTimeHistory()->FileName()
			<< ", note: " << tcv.GetTimeHistory()->FileNote() << ")" << std::endl;
	}
	return os;
}

#endif   // _ARIES_TIME_CONTROL_VARIABLE_H
