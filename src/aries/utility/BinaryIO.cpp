#include <aries/utility/DataType.h>
#include <fstream>
#include <string>

namespace aries
{

void WriteString(const std::string& ostring, std::ofstream &idof)
{
	_uint_t sLength = (_uint_t) ostring.length();
	idof.write ((char*) &sLength, sizeof(_uint_t));
	idof.write (ostring.c_str(), sLength);
}

void ReadString(std::string& os, std::ifstream &idof)
{
	_uint_t sLength;
	idof.read ((char*) &sLength, sizeof(_uint_t));
	char* cpBuffer = new char[sLength];
	idof.read(cpBuffer, sLength);
	os.assign(cpBuffer, 0, sLength);
	delete cpBuffer;
}

bool CheckSubName(std::string FileName, std::string SubName)
{
	return (FileName.rfind(SubName) == (FileName.size() - SubName.size()));
}

/*
_float_t Constants::String2Float(const std::string& s)
{
	#ifdef _STD_CPP_11
		#ifdef _FLOAT32
			return std::stof(s);   // "string" to "float"
		#else
			#ifdef _FLOAT64
				return std::stod(s);   // "string" to "double"
			#else
				#ifdef _FLOAT80
					return std::stold(s);   // "string" to "long double"
				#else
					return std::stold(s);   // "string" to "long double"
				#endif   // _FLOAT80
			#endif   // _FLOAT64
		#endif   // _FLOAT32
	#else
		std::istringstream iss(s);
		vedo_float_t f;
		if (!(iss >> f))
		{
			return 0.0;
		}
		return f;
	#endif
}
*/

/*
_int_t Constants::String2Int(const std::string& s)
{
	#ifdef _STD_CPP_11
		#ifdef _INT32
			return std::stoi(s);   // "string" to "int"
		#else
			#ifdef _INT64
				return std::stoll(s);   // "string" to "long long int"
			#else
				return std::stoll(s);   // "string" to "long long int"
			#endif   // _INT64
		#endif   // _INT32
	#else
		std::istringstream iss(s);
		vedo_int_t i;
		if (!(iss >> i))
		{
			return 0;
		}
		return i;
	#endif
}
*/

/*
_uint_t String2UInt(const std::string& s)
{
	#ifdef _STD_CPP_11
		return (_uint_t)String2Int(s);
	#else
		std::istringstream iss(s);
		_uint_t u;
		if (!(iss >> u))
		{
			return 0;
		}
		return u;
	#endif
}
*/

}   // namespace aries
