#ifndef _NJR_EXPLICIT_FUNCTION_SCATTER_TIME_HISTORY_H
#define _NJR_EXPLICIT_FUNCTION_SCATTER_TIME_HISTORY_H

#include <vedo/njr/interfaces/EFScatter.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace njr
{

template <typename TValue>
class EFSTimeHistory: public EFScatter<double, TValue>
{

public:

	EFSTimeHistory()
	{
	};

	EFSTimeHistory(const EFSTimeHistory& ef)
	{
		*this = ef;
	};

	const EFSTimeHistory& operator = (const EFSTimeHistory& ef)
	{
		(this->_sFileName) = ef._sFileName;
		(this->_KeyPoints) = ef._KeyPoints;
		return *this;
	};

	~EFSTimeHistory()
	{
	};

	EFSTimeHistory(const char* cFileName)
	{
		(this->_sFileName) = cFileName;
		double dTime;
		TValue tValue;
		std::ifstream iFile(cFileName, std::ios::in);

		// Count the size of time history
		unsigned long ulNumData = this->CountingInputFileSize(iFile);
		if(ulNumData != 0)
		{
			iFile >> dTime >> tValue;
			if(dTime >= 0.0)
			{
				(this->_KeyPoints).push_back(std::make_pair(dTime, tValue));
				double dTimeLast = dTime;

				for (unsigned long ul=1; ul<ulNumData; ul++)
				{
					iFile >> dTime >> tValue;
					if(dTime >= dTimeLast)
					{
						(this->_KeyPoints).push_back(std::make_pair(dTime, tValue));
						dTimeLast = dTime;
					}
					else
					{
						std::cerr
							<< "Error!! Code: EFSTimeHistory(const char*)" << std::endl
							<< "        Note: Time doesn't increase." << std::endl;
						exit(-1);
					}
				}
			}
			else
			{
				std::cerr
					<< "Error!! Code: EFSTimeHistory(const char*)" << std::endl
					<< "        Note: Time has negative value." << std::endl;
				exit(-1);
			}
		}
		iFile.close();
	};

	virtual const std::string GetName() const
	{
		return "EFSTimeHistory";
	};

	virtual bool AddValue(double dTime, TValue tValue)
	{
		if((this->_KeyPoints).size() != 0)
		{
			if (dTime >= (this->_KeyPoints)[(this->_KeyPoints).size()-1].first)
			{
				(this->_KeyPoints).push_back(std::make_pair(dTime, tValue));
				return true;
			}
			else
			{
				std::cout
					<< "Error!! Code: EFSTimeHistory::AddValue(double, TValue)" << std::endl
					<< "        Note: X doesn't increase." << std::endl;
				return false;
			}
		}
		else
		{
			(this->_KeyPoints).push_back(std::make_pair(dTime, tValue));
			return true;
		}
	};

	virtual TValue operator () (const double& dTime) const
	{
		unsigned long ulSize = (this->_KeyPoints).size();
		if (ulSize == 0)
		{
			return TValue();
		}
		else if (ulSize == 1)
		{
			return (this->_KeyPoints)[0].second;
		}
		else
		{
			double dTime1  = (this->_KeyPoints)[0       ].first;
			double dTime2  = (this->_KeyPoints)[ulSize-1].first;
			TValue tValue1 = (this->_KeyPoints)[0       ].second;
			TValue tValue2 = (this->_KeyPoints)[ulSize-1].second;
			if (dTime == dTime1)
			{
				return tValue1;
			}
			else if (dTime == dTime2)
			{
				return tValue2;
			}
			else if (dTime < dTime1)
			{
				return TValue();
			}
			else if (dTime > dTime2)
			{
				return TValue();
			}
			else
			{
				for (unsigned long ul=1; ul<ulSize; ul++)
				{
					if (dTime < ((this->_KeyPoints)[ul].first))
					{
						dTime1  = (this->_KeyPoints)[ul-1].first;
						tValue1 = (this->_KeyPoints)[ul-1].second;
						dTime2  = (this->_KeyPoints)[ul  ].first;
						tValue2 = (this->_KeyPoints)[ul  ].second;
						return (tValue1 + (dTime - dTime1) * (tValue2 - tValue1) / (dTime2 - dTime1));
					}
				}
			}

			std::cerr << "Error!! Code: TValue EFSTimeHistory::operator () (double) const" << std::endl;
			exit(-1);
		}
	};
};

};   // namespace njr

#endif   // _NJR_EXPLICIT_FUNCTION_SCATTER_TIME_HISTORY_H
