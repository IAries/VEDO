#ifndef _ARIES_EXPLICIT_FUNCTION_SCATTER_EXTRAPOLATION_H
#define _ARIES_EXPLICIT_FUNCTION_SCATTER_EXTRAPOLATION_H

#include <aries/utility/EFScatter.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aries
{

template <typename TX, typename TY>
class EFSExtrapolation: public EFScatter<TX, TY>
{

public:

	EFSExtrapolation()
	{
	}

	EFSExtrapolation(const EFSExtrapolation& ef)
	{
		*this = ef;
	}

	const EFSExtrapolation& operator = (const EFSExtrapolation& ef)
	{
		(this->_sFileName) = ef._sFileName;
		(this->_sFileNote) = ef._sFileNote;
		(this->_KeyPoints) = ef._KeyPoints;
		return *this;
	}

	~EFSExtrapolation()
	{
	}

	EFSExtrapolation(const std::string& filenote, const std::string& filename)
	{
		(this->_sFileName) = filename;
		(this->_sFileNote) = filenote;
		TX tX;
		TY tY;
		std::ifstream iFile(filename.c_str(), std::ios::in);

		// Count the size of time history
		_uint_t uNumData = this->CountingInputFileSize(iFile);
		if (uNumData != 0)
		{
			iFile >> tX >> tY;
			(this->_KeyPoints).push_back(std::make_pair(tX, tY));
			TX tXLast = tX;
			for (_uint_t u=1; u<uNumData; u++)
			{
				iFile >> tX >> tY;
				if (tX >= tXLast)
				{
					(this->_KeyPoints).push_back(std::make_pair(tX, tY));
					tXLast = tX;
				}
				else
				{
					std::cerr
						<< "Error!! Code: aries::EFSExtrapolation(const std::string&, const std::string&)" << std::endl
						<< "        Note: X doesn't increase."                                             << std::endl;
					exit(-1);
				}
			}
		}
		iFile.close();
	}

	virtual const std::string GetName() const
	{
		return "EFSExtrapolation";
	}

	virtual bool AddValue(TX tX, TY tY)
	{
		if (tX >= (this->_KeyPoints)[(this->_KeyPoints).size()-1].first)
		{
			(this->_KeyPoints).push_back(std::make_pair(tX, tY));
			return true;
		}
		else
		{
			std::cout
				<< "Error!! Code: aries::EFSExtrapolation::AddValue(TX, TY)" << std::endl
				<< "        Note: X doesn't increase."                       << std::endl;
			return false;
		}
	}

	virtual TY operator () (const TX& tX)
	{
		_uint_t uSize = (this->_KeyPoints).size();
		if (uSize == 0)
		{
			return TY();
		}
		else if (uSize == 1)
		{
			return (this->_KeyPoints)[0].second;
		}
		else
		{
			TX tX1 = (this->_KeyPoints)[0      ].first;
			TX tX2 = (this->_KeyPoints)[uSize-1].first;
			TY tY1 = (this->_KeyPoints)[0      ].second;
			TY tY2 = (this->_KeyPoints)[uSize-1].second;
			if (tX == tX1)
			{
				return tY1;
			}
			else if (tX == tX2)
			{
				return tY2;
			}
			else if (tX < tX1)
			{
				tX2 = (this->_KeyPoints)[1].first;
				tY2 = (this->_KeyPoints)[1].second;
				return (tY1 + (tX - tX1) * (tY2 - tY1) / (tX2 - tX1));
			}
			else if (tX > tX2)
			{
				tX1 = (this->_KeyPoints)[uSize-2].first;
				tY1 = (this->_KeyPoints)[uSize-2].second;
				return (tY2 + (tX - tX2) * (tY2 - tY1) / (tX2 - tX1));
			}
			else
			{
				for (_uint_t u=1; u<uSize; u++)
				{
					if (tX < ((this->_KeyPoints)[u].first))
					{
						tX1 = (this->_KeyPoints)[u-1].first;
						tY1 = (this->_KeyPoints)[u-1].second;
						tX2 = (this->_KeyPoints)[u  ].first;
						tY2 = (this->_KeyPoints)[u  ].second;
						return (tY1 + (tX - tX1) * (tY2 - tY1) / (tX2 - tX1));
					}
				}
			}

			std::cerr << "Error!! Code: TY aries::EFSExtrapolation::GetValue(TX) const" << std::endl;
			exit(-1);
		}
	}
};

}   // namespace aries

#endif   // _ARIES_EXPLICIT_FUNCTION_SCATTER_EXTRAPOLATION_H
