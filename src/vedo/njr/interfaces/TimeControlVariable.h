#ifndef _NJR_TIME_CONTROL_VARIABLE_H
#define _NJR_TIME_CONTROL_VARIABLE_H

#include <vedo/njr/interfaces/EFSTimeHistory.h>
#include <iostream>
#include <vector>

namespace njr
{

template <typename T>
class TimeControlVariable
{
public:

	TimeControlVariable()
	{
		_TimeHistory = 0;
	};

	TimeControlVariable
		(const T& NewValue, njr::EFSTimeHistory<double>* TimeHistory):
			_BaseValue(NewValue), Value(NewValue), _TimeHistory(TimeHistory)
	{
	};

	TimeControlVariable(const TimeControlVariable& tcv)
	{
		*this = tcv;
	};

	const TimeControlVariable& operator = (const TimeControlVariable& tcv)
	{
		Value        = tcv.Value;
		_BaseValue   = tcv._BaseValue;
		_TimeHistory = tcv._TimeHistory;
		return *this;
	};

	const TimeControlVariable& operator = (const T& tv)
	{
		_BaseValue = Value = tv;
		_TimeHistory = 0;
		return *this;
	};

	const TimeControlVariable& operator += (const T& tv)
	{
		_BaseValue += tv;
		Value      += tv;
		return *this;
	};

	const TimeControlVariable& operator -= (const T& tv)
	{
		_BaseValue -= tv;
		Value      -= tv;
		return *this;
	};

	const TimeControlVariable& operator *= (const T& tv)
	{
		_BaseValue *= tv;
		Value      *= tv;
		return *this;
	};

	~TimeControlVariable()
	{
	};

	T TimeValue(const double& dTime) const
	{
		if(_TimeHistory)
		{
			return (*_TimeHistory)(dTime) * _BaseValue;
		}
		else
		{
			return _BaseValue;
		}
	};

	void Update(const double& dTime)
	{
		if(_TimeHistory)
		{
			Value = (*_TimeHistory)(dTime) * _BaseValue;
		}
	};

	const T GetBaseValue() const
	{
		return _BaseValue;
	};

	const njr::EFSTimeHistory<double>* GetTimeHistory() const
	{
		return _TimeHistory;
	};

	T Value;

private:

	T _BaseValue;

	njr::EFSTimeHistory<double>* _TimeHistory;
};

};   // namespace njr



template <typename T>
std::ostream& operator << (std::ostream& os, const njr::TimeControlVariable<T> & tcv)
{
	os << tcv.GetBaseValue() << std::endl;
	if(tcv.GetTimeHistory())
	{
		os << "Time history (file: " << tcv.GetTimeHistory()->GetFileName() << ")" << std::endl;
	}
	return os;
};

#endif // _NJR_TIME_CONTROL_VARIABLE_H
