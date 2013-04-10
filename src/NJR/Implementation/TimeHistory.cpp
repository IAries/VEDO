#include <NJR/Interfaces/TimeHistory.h>
#include <fstream>
#include <iostream>
#include <string>

namespace njr
{

TimeHistory::TimeHistory()
{
	_sFileName = "No Name";
};

TimeHistory::TimeHistory(const TimeHistory& t)
{
	*this = t;
};

const TimeHistory& TimeHistory::operator = (const TimeHistory& t)
{
	_sFileName = t._sFileName;
	_vddTimeAndValue.clear();
	for (unsigned u=0; u<t._vddTimeAndValue.size(); u++)
		_vddTimeAndValue.push_back(t._vddTimeAndValue[u]);

	return *this;
};

TimeHistory::TimeHistory(const char* cFileName)
{
	_sFileName = cFileName;
	double dTime, dValue;
	std::ifstream iFile(cFileName, std::ios::in);

    // Count the size of time history
	unsigned long ulNumData = 0;
    std::string sTempLine;
    while(getline(iFile, sTempLine))
    {
        if(sTempLine.size() != 0)
            ulNumData++;
    }

//  iFile.close();
//	iFile.open(cFileName, std::ios::in);
    iFile.clear();
    iFile.seekg(0, std::ios::beg);
	for (unsigned u=0; u<ulNumData; u++)
	{
		iFile >> dTime >> dValue;
		_vddTimeAndValue.push_back(std::make_pair(dTime, dValue));
	}
	iFile.close();
};

TimeHistory::~TimeHistory()
{
};

std::pair<double, double> TimeHistory::GetHistory(unsigned uSerialNumber) const
{
	if (uSerialNumber < _vddTimeAndValue.size())
	{
		return _vddTimeAndValue[uSerialNumber];
	}
	else
	{
		return std::make_pair(0.0, 0.0);;
	}
};

double TimeHistory::GetValue(double dTime) const
{
	if (_vddTimeAndValue.size() == 0)
		return 0.0;

	if (_vddTimeAndValue.size() == 1)
		return _vddTimeAndValue[0].second;

	double dValue;
	double dTime1, dTime2, dValue1, dValue2;

	if (dTime <= (_vddTimeAndValue[0].first))
	{
		dTime1  = _vddTimeAndValue[0].first;
		dValue1 = _vddTimeAndValue[0].second;
		dTime2  = _vddTimeAndValue[1].first;
		dValue2 = _vddTimeAndValue[1].second;
		dValue
			= dValue1
			- (dTime1 - dTime) * (dValue2 - dValue1) / (dTime2 - dTime1);
	}
	else
	{
		bool bNotFound = true;
		unsigned u = 0;
		while (bNotFound)
		{
			if (dTime < (_vddTimeAndValue[u].first))
			{
				bNotFound = false;
				dTime2    = (_vddTimeAndValue[u].first);
				dValue2   = (_vddTimeAndValue[u].second);
				dTime1    = (_vddTimeAndValue[u-1].first);
				dValue1   = (_vddTimeAndValue[u-1].second);
				dValue
					= dValue1
					+ (dTime - dTime1) * (dValue2 - dValue1) / (dTime2 - dTime1);
			}
			u++;
			if (u == _vddTimeAndValue.size())
			{
				u--;
				dTime2  = (_vddTimeAndValue[u].first);
				dValue2 = (_vddTimeAndValue[u].second);
				dTime1  = (_vddTimeAndValue[u-1].first);
				dValue1 = (_vddTimeAndValue[u-1].second);
				dValue
					= dValue2
					+ (dTime - dTime2) * (dValue2 - dValue1) / (dTime2 - dTime1);
				bNotFound = false;
			}
		}
	}

	return dValue;
};

};   // namespace NJR

std::ostream& operator << (std::ostream& os, const njr::TimeHistory& t)
{
	os << "File name: " << t.GetFileName() << std::endl;
	for (unsigned u=0; u<t.GetNumHistory(); u++)
	{
		os << t.GetHistory(u).first << ", " << t.GetHistory(u).second << std::endl;
	}
	return os;
};
