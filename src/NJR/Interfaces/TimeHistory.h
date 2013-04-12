#ifndef _NJR_TIME_HISTORY_H
#define _NJR_TIME_HISTORY_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace njr
{

template <typename T>
class TimeHistory
{

public:

	TimeHistory()
	{
		_sFileName = "No Name";
	};

	TimeHistory(const char* cFileName)
	{
		_sFileName = cFileName;
		double dTime;
		T      tValue;
		std::ifstream iFile(cFileName, std::ios::in);

		// Count the size of time history
		unsigned long ulNumData = 0;
		std::string sTempLine;
		while(getline(iFile, sTempLine))
		{
			if(sTempLine.size() != 0)
				ulNumData++;
		}

		iFile.clear();
		iFile.seekg(0, std::ios::beg);
		for (unsigned u=0; u<ulNumData; u++)
		{
			iFile >> dTime >> tValue;
			_vdtTimeAndValue.push_back(std::make_pair(dTime, tValue));
		}
		iFile.close();
	};

	TimeHistory(const TimeHistory& t)
	{
		*this = t;
	};

	const TimeHistory& operator = (const TimeHistory& t)
	{
		_sFileName = t._sFileName;
		_vdtTimeAndValue.clear();
		for (unsigned u=0; u<t._vdtTimeAndValue.size(); u++)
			_vdtTimeAndValue.push_back(t._vdtTimeAndValue[u]);

		return *this;
	};

	~TimeHistory()
	{
	};

	inline unsigned GetNumHistory() const
	{
		return _vdtTimeAndValue.size();
	};

	std::pair<double, T> GetHistory(unsigned uSerialNumber) const
	{
		if (uSerialNumber < _vdtTimeAndValue.size())
		{
			return _vdtTimeAndValue[uSerialNumber];
		}
		else
		{
			return std::make_pair(0.0, T());
		}
	};

	T AddValue(double dTime, T tValue)
	{
		_vdtTimeAndValue.push_back(std::make_pair(dTime, tValue));
	};

	T GetValue(double dTime) const
	{
		if (_vdtTimeAndValue.size() == 0)
			return 0.0;

		if (_vdtTimeAndValue.size() == 1)
			return _vdtTimeAndValue[0].second;

		double tValue;
		double dTime1, dTime2, tValue1, tValue2;

		if (dTime <= (_vdtTimeAndValue[0].first))
		{
			dTime1  = _vdtTimeAndValue[0].first;
			tValue1 = _vdtTimeAndValue[0].second;
			dTime2  = _vdtTimeAndValue[1].first;
			tValue2 = _vdtTimeAndValue[1].second;
			tValue
				= tValue1
				- (dTime1 - dTime) * (tValue2 - tValue1) / (dTime2 - dTime1);
		}
		else
		{
			bool bNotFound = true;
			unsigned u = 0;
			while (bNotFound)
			{
				if (dTime < (_vdtTimeAndValue[u].first))
				{
					bNotFound = false;
					dTime2    = (_vdtTimeAndValue[u].first);
					tValue2   = (_vdtTimeAndValue[u].second);
					dTime1    = (_vdtTimeAndValue[u-1].first);
					tValue1   = (_vdtTimeAndValue[u-1].second);
					tValue
						= tValue1
						+ (dTime - dTime1) * (tValue2 - tValue1) / (dTime2 - dTime1);
				}
				u++;
				if (u == _vdtTimeAndValue.size())
				{
					u--;
					dTime2  = (_vdtTimeAndValue[u].first);
					tValue2 = (_vdtTimeAndValue[u].second);
					dTime1  = (_vdtTimeAndValue[u-1].first);
					tValue1 = (_vdtTimeAndValue[u-1].second);
					tValue
						= tValue2
						+ (dTime - dTime2) * (tValue2 - tValue1) / (dTime2 - dTime1);
					bNotFound = false;
				}
			}
		}

		return tValue;
	};

	inline std::string GetFileName() const
	{
		return _sFileName;
	};

protected:

	std::string _sFileName;

	std::vector<std::pair<double, T> > _vdtTimeAndValue;
};

};   // namespace njr



template <typename T>
std::ostream& operator << (std::ostream& os, const njr::TimeHistory<T>& t)
{
	os << "File name: " << t.GetFileName() << std::endl;
	for (unsigned u=0; u<t.GetNumHistory(); u++)
	{
		os << t.GetHistory(u).first << ", " << t.GetHistory(u).second << std::endl;
	}
	return os;
};

#endif // _NJR_TIME_HISTORY_H
