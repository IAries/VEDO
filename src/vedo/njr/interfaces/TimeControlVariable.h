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
	};

	TimeControlVariable
		(const T& NewValue, njr::EFSTimeHistory<double>* TimeHistory):
			_InitialValue(NewValue), Value(NewValue), _TimeHistory(TimeHistory)
	{
	};

	TimeControlVariable(const TimeControlVariable& tcv)
	{
		*this = tcv;
	};

	const TimeControlVariable& operator = (const TimeControlVariable& tcv)
	{
		_InitialValue = tcv._InitialValue;
		Value         = tcv.Value;

		if (tcv._TimeHistory != 0)
			_TimeHistory  = tcv._TimeHistory;

		return *this;
	};

	const TimeControlVariable& operator = (const T& tv)
	{
		Value = tv;
		return *this;
	};

	const TimeControlVariable& operator += (const T& tv)
	{
		Value += tv;
		return *this;
	};

	const TimeControlVariable& operator -= (const T& tv)
	{
		Value -= tv;
		return *this;
	};

	const TimeControlVariable& operator *= (const T& tv)
	{
		Value *= tv;
		return *this;
	};

	~TimeControlVariable()
	{
	};

	T TimeValue(const double& dTime)
	{
		if(_TimeHistory != 0)
		{
			return (*_TimeHistory)(dTime) * _InitialValue;
		}
		else
		{
			std::cerr
				<< "Error!! Code: T TimeControlVariable::TimeValue(const double&)" << std::endl
				<< "        Note: _TimeHistory == 0" << std::endl;
			exit(-1);
		}
	};

	T UpdateAndGetTimeValue(const double& dTime)
	{
		Update(dTime);
		return Value;
	};

	void Update(const double& dTime)
	{
		if(_TimeHistory != 0)
		{
			Value = (*_TimeHistory)(dTime) * _InitialValue;
		}
		else
		{
			std::cerr
				<< "Error!! Code: void TimeControlVariable::Update(const double& dTime)" << std::endl
				<< "        Note: _TimeHistory == 0" << std::endl;
			exit(-1);
		}
	};

	T Value;

private:

	T _InitialValue;

	njr::EFSTimeHistory<double>* _TimeHistory;
};

};   // namespace njr

#endif // _NJR_TIME_CONTROL_VARIABLE_H
