#include <vedo/constants/interfaces/Constants.h>
#include <sstream>

namespace vedo
{

Constants* Constants::_Constants = 0;

Constants* Constants::Instance()
{
	if (_Constants == 0)
	{
		_Constants = new Constants;
		_Constants->SetDefaultValue();
	}
	return _Constants;
}

Constants::Constants()
{
}

Constants::Constants(const Constants& f)
{
	*this = f;
}

const Constants& Constants::operator = (const Constants& f)
{
	this->Instance();
	return *this;
}

Constants::~Constants()
{
}

void Constants::SetDefaultValue()
{
	dSafetyFactor     = 1.1;
	uImpactBufferSize = 65535;

	_sSysInfo = "(released in " + sReleaseDate + ", ";
	if (bDebugMode)
	{
		_sSysInfo.append("debug");
	}
	else
	{
		_sSysInfo.append("release");
	}
	_sSysInfo.append(" mode");

	if (bCPP11)
	{
		_sSysInfo.append(" with C++11 standard");
	}

	if(sPlatform != "")
	{
		_sSysInfo.append(", built for ");
		_sSysInfo.append(sPlatform);
		_sSysInfo.append(" platform");
	}

	_sSysInfo.append(", ");
	if (uVEDO_INT_BITS == 32)
	{
		_sSysInfo.append("32");
	}
	else if (uVEDO_INT_BITS == 64)
	{
		_sSysInfo.append("64");
	}
	_sSysInfo.append("-bit integer & ");
	if (uVEDO_FLOAT_BITS == 32)
	{
		_sSysInfo.append("32");
	}
	else if (uVEDO_FLOAT_BITS == 64)
	{
		_sSysInfo.append("64");
	}
	else if (uVEDO_FLOAT_BITS == 80)
	{
		_sSysInfo.append("80");
	}
	_sSysInfo.append("-bit float)");
}

/*
vedo_float_t Constants::String2Float(const std::string& s)
{
	#ifdef _STD_CPP_11
		#ifdef _VEDO_FLOAT32
			return std::stof(s);   // "string" to "float"
		#else
			#ifdef _VEDO_FLOAT64
				return std::stod(s);   // "string" to "double"
			#else
				#ifdef _VEDO_FLOAT80
					return std::stold(s);   // "string" to "long double"
				#else
					return std::stold(s);   // "string" to "long double"
				#endif   // _VEDO_FLOAT80
			#endif   // _VEDO_FLOAT64
		#endif   // _VEDO_FLOAT32
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

vedo_int_t Constants::String2Int(const std::string& s)
{
	#ifdef _STD_CPP_11
		#ifdef _VEDO_INT32
			return std::stoi(s);   // "string" to "int"
		#else
			#ifdef _VEDO_INT64
				return std::stoll(s);   // "string" to "long long int"
			#else
				return std::stoll(s);   // "string" to "long long int"
			#endif   // _VEDO_INT64
		#endif   // _VEDO_INT32
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

vedo_uint_t Constants::String2UInt(const std::string& s)
{
	#ifdef _STD_CPP_11
		return (vedo_uint_t)String2Int(s);
	#else
		std::istringstream iss(s);
		vedo_uint_t u;
		if (!(iss >> u))
		{
			return 0;
		}
		return u;
	#endif
}
*/

}   // namespace vedo
