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

// Common math constants
//#define _USE_MATH_DEFINES
//#include <math.h>
//static const _float_t fPI             = M_PI;
static const _float_t fPI             = 4.0 * std::atan(1.0);
static const _float_t fOneTwelfthPI   = fPI / 12.0;
static const _float_t fOneEighthPI    = 0.125 * fPI;   // dPI/8
static const _float_t fOneSixthPI     = fPI / 6.0;
static const _float_t fQuarterPI      = 0.25 * fPI;    // dPI/4
static const _float_t fOneThirdPI     = fPI / 3.0;
static const _float_t fHalfPI         = 0.5 * fPI;     // dPI/2
static const _float_t fOneAndHalfPI   = 1.5 * fPI;     // dPI*3/2
static const _float_t fDoublePI       = 2.0 * fPI;
static const _float_t fFourthThirdsPI = 4.0 / 3.0 * fPI;
static const _float_t fDegree2PI      = 1.0 / 180.0 * fPI;



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

	/*
	inline _float_t SampleData() const
	{
		return _fSampleData;
	}

	inline void SetSampleData(const _float_t& f)
	{
		_fSampleData = f;
	}
	*/

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

	inline std::string ReleaseDate() const
	{
		return _sParameterSet("ReleaseDate");
	}

	inline void SetReleaseDate(const std::string& s)
	{
		_sParameterSet["ReleaseDate"] = s;
	}

	inline std::string Version() const
	{
		return _sParameterSet("Version");
	}

	inline void SetVersion(const std::string& s)
	{
		_sParameterSet["Version"] = s;
	}

	inline std::string Information() const
	{
		return _sParameterSet("Information");
	}

private:

	static Constants*                      _Constants;

	ParameterSet<std::string, std::string> _sParameterSet;

	ParameterSet<std::string, _uint_t    > _uParameterSet;

	ParameterSet<std::string, _float_t   > _fParameterSet;

	void SetDefaultValue()
	{
		_sParameterSet["ReleaseDate"]        = "2015-03-23";
		_sParameterSet["Version"]            = "X5 build 323";
		_fParameterSet["SafetyFactor"      ] = 1.1;
		_uParameterSet["NumUDDDOStatus"    ] = 0;
		_uParameterSet["NumUDDImpactStatus"] = 0;
		_uParameterSet["ImpactBufferSize"  ] = 65535;
		// in VEDO, _uParameterSet["ImpactBufferSize"] ~= 18,000 for 60,000 elements / 4 processors; ~= 60,000 for 60,000 elements / 8 processors

		std::string sInformation = "(released in " + _sParameterSet["ReleaseDate"] + ", ";
		if (bDebugMode)
		{
			sInformation.append("debug");
		}
		else
		{
			sInformation.append("release");
		}
		sInformation.append(" mode");

		if (bCPP11)
		{
			sInformation.append(" with C++11 standard");
		}

		if(sPlatform != "")
		{
			sInformation.append(", built for ");
			sInformation.append(sPlatform);
			sInformation.append(" platform");
		}

		sInformation.append(", ");
		if (uINT_BITS == 32)
		{
			sInformation.append("32");
		}
		else if (uINT_BITS == 64)
		{
			sInformation.append("64");
		}
		sInformation.append("-bit integer & ");
		if (uFLOAT_BITS == 32)
		{
			sInformation.append("32");
		}
		else if (uFLOAT_BITS == 64)
		{
			sInformation.append("64");
		}
		else if (uFLOAT_BITS == 80)
		{
			sInformation.append("80");
		}
		sInformation.append("-bit float)");
		_sParameterSet["Information"] = sInformation;
	}

	Constants()
	{
	}
};

}   // namespace aries

#endif   // _ARIES_CONSTANTS_H
