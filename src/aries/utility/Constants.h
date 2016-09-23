#ifndef _ARIES_CONSTANTS_H
#define _ARIES_CONSTANTS_H

#include <aries/utility/DataType.h>
#include <aries/utility/ParameterSet.h>
#include <cmath>
#include <iostream>
//#include <inttypes.h>
#include <sstream>
#include <string>

namespace aries
{

namespace math
{

// Common math constants
//#define _USE_MATH_DEFINES
//#include <math.h>
//static const _float_t _PI             = M_PI;
static const _float_t _PI             = 4.0 * std::atan(1.0);
static const _float_t _SquarePI       = _PI * _PI;
static const _float_t _OneTwelfthPI   = _PI / 12.0;
static const _float_t _OneEighthPI    = 0.125 * _PI;   // _PI/8
static const _float_t _OneSixthPI     = _PI / 6.0;
static const _float_t _QuarterPI      = 0.25 * _PI;    // _PI/4
static const _float_t _OneThirdPI     = _PI / 3.0;
static const _float_t _HalfPI         = 0.5 * _PI;     // _PI/2
static const _float_t _OneAndHalfPI   = 1.5 * _PI;     // _PI*3/2
static const _float_t _DoublePI       = 2.0 * _PI;
static const _float_t _FourthThirdsPI = 4.0 / 3.0 * _PI;
static const _float_t _Degree2PI      = 1.0 / 180.0 * _PI;

}



namespace information
{

static const std::string _ReleaseDate = "2016-09-23";

static const std::string _Version     = "X6 build 923";

#ifdef _DEBUG
	static const std::string _DebugMode = "debug";
#else
	static const std::string _DebugMode = "release";
#endif   // _DEBUG

#ifdef _STD_CPP_11
	static const std::string _CPP11 = " with C++11 standard";
#else
	static const std::string _CPP11 = "";
#endif   // _STD_CPP_11

#ifdef _MS_WINDOWS
	static const std::string _Platform = "Microsoft Windows";
#else
	#ifdef _UNIX_LIKE
		static const std::string _Platform = "UNIX-like";
	#else
		static const std::string _Platform = "";
	#endif   // _UNIX_LIKE
#endif   // _MS_WINDOWS

#ifdef _INT32
	static const std::string _INT_BITS = "32";
#else
	#ifdef _INT64
		static const std::string _INT_BITS = "64";
	#else
		// Default type
		static const std::string _INT_BITS = "64";
	#endif   // _INT64
#endif   // _INT32

#ifdef _FLOAT32
	static const std::string _FLOAT_BITS = "32";
#else
	#ifdef _FLOAT64
		static const std::string _FLOAT_BITS = "64";
	#else
		#ifdef _FLOAT80
			static const std::string _FLOAT_BITS = "80";
		#else
			// Default type
			static const std::string _FLOAT_BITS = "64";
		#endif   // _FLOAT80
	#endif   // _FLOAT64
#endif   // _FLOAT32

static const std::string _Information
	= "(released in " + _ReleaseDate + ", " + _DebugMode + " mode" + _CPP11 + ", built for " + _Platform + " platform, "
	+ _INT_BITS + "-bit integer & " + _FLOAT_BITS + "-bit float)";

}



class Constants
{

public:

	static Constants* Instance()
	{
		if (_Constants == 0)
		{
			_Constants = new Constants;
			_Constants->SetDefaultValue();
		}
		return _Constants;
	}

	Constants(const Constants& c)
	{
		*this = c;
	}

	const Constants& operator = (const Constants& c)
	{
		this->Instance();
		return *this;
	}

	~Constants()
	{
	}

	inline _float_t SafetyFactor() const
	{
		return _fParameterSet("SafetyFactor");
	}

	inline void SetSafetyFactor(const _float_t& f)
	{
		_fParameterSet["SafetyFactor"] = f;
	}

	inline _uint_t ImpactBufferSize() const
	{
		return _uParameterSet("ImpactBufferSize");
	}

	inline void SetImpactBufferSize(const _uint_t& u)
	{
		_uParameterSet["ImpactBufferSize"] = u;
	}

	inline _uint_t NumUDDDOStatus() const
	{
		return _uParameterSet("NumUDDDOStatus");
	}

	inline void SetNumUDDDOStatus(const _uint_t& u)
	{
		_uParameterSet["NumUDDDOStatus"] = u;
	}

	inline _uint_t NumUDDImpactStatus() const
	{
		return _uParameterSet("NumUDDImpactStatus");
	}

	inline void SetNumUDDImpactStatus(const _uint_t& u)
	{
		_uParameterSet["NumUDDImpactStatus"] = u;
	}

protected:

	static Constants*                      _Constants;

	ParameterSet<std::string, std::string> _sParameterSet;

	ParameterSet<std::string, _uint_t    > _uParameterSet;

	ParameterSet<std::string, _float_t   > _fParameterSet;

	void SetDefaultValue()
	{
		_fParameterSet["SafetyFactor"      ] = 1.1;
		_uParameterSet["NumUDDDOStatus"    ] = 0;
		_uParameterSet["NumUDDImpactStatus"] = 0;
		_uParameterSet["ImpactBufferSize"  ] = 65535;
		// in VEDO, _uParameterSet["ImpactBufferSize"] ~= 18,000 for 60,000 elements / 4 processors; ~= 60,000 for 60,000 elements / 8 processors
	}

	Constants()
	{
	}
};

}   // namespace aries

#endif   // _ARIES_CONSTANTS_H
