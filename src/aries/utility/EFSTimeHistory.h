#ifndef _ARIES_EXPLICIT_FUNCTION_SCATTER_TIME_HISTORY_H
#define _ARIES_EXPLICIT_FUNCTION_SCATTER_TIME_HISTORY_H

#include <aries/utility/EFScatter.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aries
{

template <typename TValue>
class EFSTimeHistory: public EFScatter<_float_t, TValue>
{

public:

	EFSTimeHistory()
	{
	}

	EFSTimeHistory(const EFSTimeHistory& ef)
	{
		*this = ef;
	}

	const EFSTimeHistory& operator = (const EFSTimeHistory& ef)
	{
		(this->_sFileName) = ef._sFileName;
		(this->_sFileNote) = ef._sFileNote;
		(this->_KeyPoints) = ef._KeyPoints;
		return *this;
	}

	~EFSTimeHistory()
	{
	}

	EFSTimeHistory(const std::string& filenote, const std::string& filename)
	{
		(this->_sFileName) = filename;
		(this->_sFileNote) = filenote;
		_float_t fTime;
		TValue tValue;
		std::ifstream iFile(filename.c_str(), std::ios::in);

		// Count the size of time history
		_uint_t uNumData = this->CountingInputFileSize(iFile);
		if (uNumData != 0)
		{
			iFile >> fTime >> tValue;
			if (fTime >= 0.0)
			{
				(this->_KeyPoints).push_back(std::make_pair(fTime, tValue));
				_float_t dTimeLast = fTime;

				for (_uint_t ul=1; ul<uNumData; ul++)
				{
					iFile >> fTime >> tValue;
					if (fTime >= dTimeLast)
					{
						(this->_KeyPoints).push_back(std::make_pair(fTime, tValue));
						dTimeLast = fTime;
					}
					else
					{
						std::cerr
							<< "Error!! Code: aries::EFSTimeHistory(const std::string&, const std::string&)" << std::endl
							<< "        Note: Time doesn't increase."                                        << std::endl;
						exit(-1);
					}
				}
			}
			else
			{
				std::cerr
					<< "Error!! Code: aries::EFSTimeHistory(const std::string&, const std::string&)" << std::endl
					<< "        Note: Time has negative value."                                      << std::endl;
				exit(-1);
			}
		}
		iFile.close();
	}

	virtual const std::string GetName() const
	{
		return "EFSTimeHistory";
	}

	virtual bool AddValue(_float_t fTime, TValue tValue)
	{
		if ((this->_KeyPoints).size() != 0)
		{
			if (fTime >= (this->_KeyPoints)[(this->_KeyPoints).size()-1].first)
			{
				(this->_KeyPoints).push_back(std::make_pair(fTime, tValue));
				return true;
			}
			else
			{
				std::cout
					<< "Error!! Code: aries::EFSTimeHistory::AddValue(_float_t, TValue)" << std::endl
					<< "        Note: Time doesn't increase."                            << std::endl;
				return false;
			}
		}
		else
		{
			(this->_KeyPoints).push_back(std::make_pair(fTime, tValue));
			return true;
		}
	}

	virtual TValue operator () (const _float_t& fTime) const
	{
		_uint_t uSize = (this->_KeyPoints).size();
		if (uSize == 0)
		{
			return TValue();
		}
		else if (uSize == 1)
		{
			return (this->_KeyPoints)[0].second;
		}
		else
		{
			_float_t fTime1  = (this->_KeyPoints)[0      ].first;
			_float_t fTime2  = (this->_KeyPoints)[uSize-1].first;
			TValue   tValue1 = (this->_KeyPoints)[0      ].second;
			TValue   tValue2 = (this->_KeyPoints)[uSize-1].second;
			if (fTime == fTime1)
			{
				return tValue1;
			}
			else if (fTime == fTime2)
			{
				return tValue2;
			}
			else if (fTime < fTime1)
			{
				return TValue();
			}
			else if (fTime > fTime2)
			{
				return TValue();
			}
			else
			{
				for (_uint_t ul=1; ul<uSize; ul++)
				{
					if (fTime < ((this->_KeyPoints)[ul].first))
					{
						fTime1  = (this->_KeyPoints)[ul-1].first;
						tValue1 = (this->_KeyPoints)[ul-1].second;
						fTime2  = (this->_KeyPoints)[ul  ].first;
						tValue2 = (this->_KeyPoints)[ul  ].second;
						return (tValue1 + (fTime - fTime1) * (tValue2 - tValue1) / (fTime2 - fTime1));
					}
				}
			}

			std::cerr << "Error!! Code: TValue aries::EFSTimeHistory::operator () (_float_t) const" << std::endl;
			exit(-1);
		}
	}
};

}   // namespace aries

#endif   // _ARIES_EXPLICIT_FUNCTION_SCATTER_TIME_HISTORY_H
