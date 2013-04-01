#ifndef _NJR_TIME_HISTORY_H
#define _NJR_TIME_HISTORY_H

#include <iostream>
#include <string>
#include <vector>

namespace njr
{

class TimeHistory
{

public:

	TimeHistory(const char* cFileName);

	TimeHistory(const TimeHistory&);

	const TimeHistory& operator = (const TimeHistory&);

	~TimeHistory();

	inline unsigned GetNumHistory() const
	{
		return _vddTimeAndValue.size();
	};

	std::pair<double, double> GetHistory(unsigned uSerialNumber) const;

	double GetValue(double dTime) const;

	inline std::string GetFileName() const
	{
		return _sFileName;
	};

protected:

	TimeHistory();

	std::string _sFileName;

	std::vector<std::pair<double, double> > _vddTimeAndValue;
};

};   // namespace njr

std::ostream& operator << (std::ostream& os, const njr::TimeHistory& th);

#endif // _NJR_TIME_HISTORY_H
