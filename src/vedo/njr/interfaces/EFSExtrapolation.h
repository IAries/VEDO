#ifndef _NJR_EXPLICIT_FUNCTION_SCATTER_EXTRAPOLATION_H
#define _NJR_EXPLICIT_FUNCTION_SCATTER_EXTRAPOLATION_H

#include <vedo/njr/interfaces/EFScatter.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace njr
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
		(this->_KeyPoints) = ef._KeyPoints;
		return *this;
	}

	~EFSExtrapolation()
	{
	}

	EFSExtrapolation(const char* cFileName)
	{
		(this->_sFileName) = cFileName;
		TX tX;
		TY tY;
		std::ifstream iFile(cFileName, std::ios::in);

		// Count the size of time history
		vedo::vedo_uint_t ulNumData = this->CountingInputFileSize(iFile);
		if (ulNumData != 0)
		{
			iFile >> tX >> tY;
			(this->_KeyPoints).push_back(std::make_pair(tX, tY));
			TX tXLast = tX;
			for (vedo::vedo_uint_t ul=1; ul<ulNumData; ul++)
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
						<< "Error!! Code: EFSExtrapolation(const char*)" << std::endl
						<< "        Note: X doesn't increase." << std::endl;
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
				<< "Error!! Code: EFSExtrapolation::AddValue(TX, TY)" << std::endl
				<< "        Note: X doesn't increase."                << std::endl;
			return false;
		}
	}

	virtual TY operator () (const TX& tX)
	{
		vedo::vedo_uint_t ulSize = (this->_KeyPoints).size();
		if (ulSize == 0)
		{
			return TY();
		}
		else if (ulSize == 1)
		{
			return (this->_KeyPoints)[0].second;
		}
		else
		{
			TX tX1 = (this->_KeyPoints)[0       ].first;
			TX tX2 = (this->_KeyPoints)[ulSize-1].first;
			TY tY1 = (this->_KeyPoints)[0       ].second;
			TY tY2 = (this->_KeyPoints)[ulSize-1].second;
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
				tX1 = (this->_KeyPoints)[ulSize-2].first;
				tY1 = (this->_KeyPoints)[ulSize-2].second;
				return (tY2 + (tX - tX2) * (tY2 - tY1) / (tX2 - tX1));
			}
			else
			{
				for (vedo::vedo_uint_t ul=1; ul<ulSize; ul++)
				{
					if (tX < ((this->_KeyPoints)[ul].first))
					{
						tX1 = (this->_KeyPoints)[ul-1].first;
						tY1 = (this->_KeyPoints)[ul-1].second;
						tX2 = (this->_KeyPoints)[ul  ].first;
						tY2 = (this->_KeyPoints)[ul  ].second;
						return (tY1 + (tX - tX1) * (tY2 - tY1) / (tX2 - tX1));
					}
				}
			}

			std::cerr << "Error!! Code: TY EFSExtrapolation::GetValue(TX) const" << std::endl;
			exit(-1);
		}
	}
};

}   // namespace njr

#endif   // _NJR_EXPLICIT_FUNCTION_SCATTER_EXTRAPOLATION_H
