#ifndef _ARIES_BINARY_IO_H
#define _ARIES_BINARY_IO_H

#include <aries/utility/DataType.h>
#include <fstream>
#include <string>

namespace aries
{

void WriteString(const std::string& ostring, std::ofstream &idof);

void ReadString(std::string& os, std::ifstream &idof);

bool CheckSubName(std::string FileName, std::string SubName);

template <typename T>
T String2T(const std::string& s)
{
	std::istringstream iss(s);
	T t;
	if (!(iss >> t))
	{
		return 0;
	}
	return t;
}

//_float_t Constants::String2Float(const std::string& s)

//_int_t Constants::String2Int(const std::string& s);

//_uint_t String2UInt(const std::string& s);

}   // namespace aries

#endif // _ARIES_BINARY_IO_H
