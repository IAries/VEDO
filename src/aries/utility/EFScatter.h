#ifndef _ARIES_EXPLICIT_FUNCTION_SCATTER_H
#define _ARIES_EXPLICIT_FUNCTION_SCATTER_H

#include <aries/utility/ExplicitFunction.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aries
{

template <typename TX, typename TY>
class EFScatter: public ExplicitFunction<TX, TY>
{

public:

	EFScatter(): _pScaleX(0), _pScaleY(0)
	{
	}

	EFScatter(const EFScatter& ef)
	{
		*this = ef;
	}

	const EFScatter& operator = (const EFScatter& ef)
	{
		_sFileName = ef._sFileName;
		_sFileNote = ef._sFileNote;

		if (_pScaleX == 0)
		{
			_pScaleX = new TX;
		}

		if (ef._pScaleX)
		{
			*_pScaleX = *(ef._pScaleX);
		}
		else
		{
			_pScaleX = 0;
		}

		if (_pScaleY == 0)
		{
			_pScaleY = new TY;
		}

		if (ef._pScaleY)
		{
			*_pScaleY = *(ef._pScaleY);
		}
		else
		{
			_pScaleY = 0;
		}

		_KeyPoints = ef._KeyPoints;
		return *this;
	}

	~EFScatter()
	{
		delete _pScaleX;
		delete _pScaleY;
	}

	EFScatter(const std::string& filenote, const std::string& filename): _pScaleX(0), _pScaleY(0)
	{
		_sFileName = filename;
		_sFileNote = filenote;
		TX tX;
		TY tY;
		std::ifstream iFile(_sFileName.c_str(), std::ios::in);

		// Count the size of time history
		_uint_t ulNumData = CountingInputFileSize(iFile);
		for (_uint_t ul=0; ul<ulNumData; ul++)
		{
			iFile >> tX >> tY;
			_KeyPoints.push_back(std::make_pair(tX, tY));
		}
		iFile.close();
	}

	EFScatter(const std::string& filenote, const std::string& filename, const TX& ScaleX, const TY& ScaleY)
	{
		_sFileName = filename;
		_sFileNote = filenote;

		_pScaleX   = new TX;
		*_pScaleX  = ScaleX;
		_pScaleY   = new TY;
		*_pScaleY  = ScaleY;
		TX tX;
		TY tY;
		std::ifstream iFile(_sFileName.c_str(), std::ios::in);

		// Count the size of time history
		_uint_t ulNumData = CountingInputFileSize(iFile);
		for (_uint_t ul=0; ul<ulNumData; ul++)
		{
			iFile >> tX >> tY;
			_KeyPoints.push_back(std::make_pair(ScaleX * tX, ScaleY * tY));
		}
		iFile.close();
	}

	inline _uint_t size() const
	{
		return (_uint_t)_KeyPoints.size();
	}

	inline _uint_t Size() const
	{
		return (_uint_t)_KeyPoints.size();
	}

	inline std::string FileNote() const
	{
		return _sFileNote;
	}

	inline std::string FileName() const
	{
		return _sFileName;
	}

	std::pair<TX, TY> GetValue(_uint_t uIndex) const
	{
		if (uIndex < _KeyPoints.size())
		{
			return _KeyPoints[uIndex];
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

	bool HasScaleX() const
	{
		if (_pScaleX)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	TX GetScaleX() const
	{
		if (_pScaleX)
		{
			return *_pScaleX;
		}
		else
		{
			std::cerr
				<< "Error!! Code: aries::EFScatter::GetScaleX() const" << std::endl
				<< "        Note: _pScaleX doesn't exist!!"           << std::endl;
			exit(-1);
		}
	}

	bool HasScaleY() const
	{
		if (_pScaleY)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	TY GetScaleY() const
	{
		if (_pScaleY)
		{
			return *_pScaleY;
		}
		else
		{
			std::cerr
				<< "Error!! Code: aries::EFScatter::GetScaleY() const" << std::endl
				<< "        Note: _pScaleY doesn't exist!!"           << std::endl;
			exit(-1);
		}
	}

	virtual inline const bool PathDependent() const
	{
		return false;
	}

	void clear()
	{
		_KeyPoints.clear();
	}

	void Clear()
	{
		this->clear();
	}

	virtual bool AddValue(TX tX, TY tY)
	{
		TX NewTX = tX;
		if (_pScaleX)
		{
			NewTX *= (*_pScaleX);
		}

		TX NewTY = tY;
		if (_pScaleY)
		{
			NewTY *= (*_pScaleY);
		}

		_KeyPoints.push_back(std::make_pair(NewTX, NewTY));
		return true;
	}

	virtual TY operator () (const TX& tx) const = 0;

protected:

	std::string _sFileName;

	std::string _sFileNote;

	TX*         _pScaleX;

	TY*         _pScaleY;

	std::vector<std::pair<TX, TY> > _KeyPoints;

	_uint_t CountingInputFileSize(std::ifstream& iFile) const
	{
		iFile.clear();
		iFile.seekg(0, std::ios::beg);
		_uint_t ulNumData = 0;
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
};



template <typename TX, typename TY>
std::ostream& operator << (std::ostream& os, const aries::EFScatter<TX, TY>& t)
{
	if (t.FileName() != "")
	{
		os << "File name: " << t.FileName() << std::endl;
	}
	else
	{
		os << "File name: (Null)" << std::endl;
	}

	if (t.FileNote() != "")
	{
		os << "File note: " << t.FileNote() << std::endl;
	}
	else
	{
		os << "File note: (Null)" << std::endl;
	}

	if (t.size() > 0)
	{
		os << "Key points:" << std::endl;
		for (_uint_t ul=0; ul<t.size(); ul++)
		{
			os << t.GetValue(ul).first << ", " << t.GetValue(ul).second << std::endl;
		}
	}
	return os;
}

}   // namespace aries

#endif   // _ARIES_EXPLICIT_FUNCTION_SCATTER_H

