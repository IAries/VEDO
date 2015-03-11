#ifndef _NJR_EXPLICIT_FUNCTION_SCATTER_H
#define _NJR_EXPLICIT_FUNCTION_SCATTER_H

#include <vedo/njr/interfaces/ExplicitFunction.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace njr
{

template <typename TX, typename TY>
class EFScatter: public ExplicitFunction<TX, TY>
{

public:

	EFScatter()
	{
	}

	EFScatter(const EFScatter& ef)
	{
		*this = ef;
	}

	const EFScatter& operator = (const EFScatter& ef)
	{
		_sFileName = ef._sFileName;
		_KeyPoints = ef._KeyPoints;
		return *this;
	}

	~EFScatter()
	{
	}

	EFScatter(const char* cFileName)
	{
		(this->_sFileName) = cFileName;
		TX tX;
		TY tY;
		std::ifstream iFile(cFileName, std::ios::in);

		// Count the size of time history
		vedo::vedo_uint_t ulNumData = CountingInputFileSize(iFile);
		for (vedo::vedo_uint_t ul=0; ul<ulNumData; ul++)
		{
			iFile >> tX >> tY;
			_KeyPoints.push_back(std::make_pair(tX, tY));
		}
		iFile.close();
	}

	vedo::vedo_uint_t CountingInputFileSize(std::ifstream& iFile) const
	{
		iFile.clear();
		iFile.seekg(0, std::ios::beg);
		vedo::vedo_uint_t ulNumData = 0;
		std::string sTempLine;
		while (getline(iFile, sTempLine))
		{
			if (sTempLine.size() != 0)
			{
				ulNumData++;
			}
		}
		iFile.clear();
		iFile.seekg(0, std::ios::beg);
		return ulNumData;
	}

	inline vedo_uint_t Size() const
	{
		return _KeyPoints.size();
	}

	inline std::string GetFileName() const
	{
		return _sFileName;
	}

	std::pair<TX, TY> GetValue(vedo::vedo_uint_t ulIndex) const
	{
		if (ulIndex < _KeyPoints.size())
		{
			return _KeyPoints[ulIndex];
		}
		else
		{
			return std::make_pair(TX(), TY());
		}
	}

	virtual const std::string GetName() const
	{
		return "EFScatter";
	}

	virtual inline const bool PathDependent() const
	{
		return false;
	}

	void Clear()
	{
		_KeyPoints.clear();
	}

	virtual bool AddValue(TX tX, TY tY)
	{
		_KeyPoints.push_back(std::make_pair(tX, tY));
		return true;
	}

	virtual TY operator () (const TX& tx) const = 0;

protected:

	std::string _sFileName;

	std::vector<std::pair<TX, TY> > _KeyPoints;
};



template <typename TX, typename TY>
std::ostream& operator << (std::ostream& os, const njr::EFScatter<TX, TY>& t)
{
	if (t.GetFileName() != "")
	{
		os << "File name: " << t.GetFileName() << std::endl;
	}
	else
	{
		os << "File name: (Null)" << std::endl;
	}

	if (t.Size() > 0)
	{
		os << "Key points:" << std::endl;
		for (vedo::vedo_uint_t ul=0; ul<t.Size(); ul++)
		{
			os << t.GetValue(ul).first << ", " << t.GetValue(ul).second << std::endl;
		}
	}
	return os;
}

}   // namespace njr

#endif   // _NJR_EXPLICIT_FUNCTION_SCATTER_H

