#ifndef _VEDO_CONSTANTS_H
#define _VEDO_CONSTANTS_H

#include <iostream>
//#include <inttypes.h>
#include <sstream>
#include <string>

namespace vedo
{

static const std::string sVersion     = "X5 build 312";
static const std::string sReleaseDate = "2015-03-12";

#ifdef _VEDO_DEBUG
	static const bool bDebugMode = true;
#else
	static const bool bDebugMode = false;
#endif   // _VEDO_DEBUG

#ifdef _STD_CPP_11
	static const bool bCPP11 = true;
#else
	static const bool bCPP11 = false;
#endif   // _STD_CPP_11

#ifdef _MS_WINDOWS
	static const std::string sPlatform = "Microsoft Windows";
#else
	#ifdef _UNIX_LIKE
		static const std::string sPlatform = "UNIX-like";
	#else
		static const std::string sPlatform = "";
	#endif   // _UNIX_LIKE
#endif   // _MS_WINDOWS

// The following data type make VEDO's ability consistent in both LLP64 (Microsoft Windows) & LP64 (UNIX-like OS) platforms
// (Wikipedia: 64-bit computing, http://en.wikipedia.org/wiki/64-bit_computing)
// (Fundamental types, http://en.cppreference.com/w/cpp/language/types)
//typedef   signed short     int    vedo_int16_t;     // Range:                    -32,768 ~                     32,767
//typedef unsigned short     int    vedo_uint16_t;    // Range:                          0 ~                     65,535
typedef   signed           int    vedo_int32_t;     // Range:             -2,147,483,648 ~              2,147,483,647
typedef unsigned           int    vedo_uint32_t;    // Range:                          0 ~              4,294,697,295
// "unsigned/unsigned long int" is 32-bit in LLP64, but 64-bit in LP64 platform, that's why we don't use it.
typedef   signed long long int    vedo_int64_t;     // Range: -9,223,372,036,854,775,808 ~  9,223,372,036,854,775,807
typedef unsigned long long int    vedo_uint64_t;    // Range:                          0 ~ 18,446,744,073,709,551,615
typedef                    float  vedo_float32_t;   // Range:   1e-37 ~ 1e38
typedef                    double vedo_float64_t;   // Range:  1e-307 ~ 1e308
typedef               long double vedo_float80_t;   // Range: 1e-4931 ~ 1e4932

// Instead of int/unsigned int, please use "vedo_int_t"/"vedo_uint_t" in whole system
// Instead of float/double, please use "vedo_float_t" in whole system
#ifdef _VEDO_INT32
	typedef vedo_int32_t  vedo_int_t;
	typedef vedo_uint32_t vedo_uint_t;
	static const vedo_uint_t uVEDO_INT_BITS = 32;
#else
	#ifdef _VEDO_INT64
		typedef vedo_int64_t  vedo_int_t;
		typedef vedo_uint64_t vedo_uint_t;
		static const vedo_uint_t uVEDO_INT_BITS = 64;
	#else
		// Default type
		typedef vedo_int64_t  vedo_int_t;
		typedef vedo_uint64_t vedo_uint_t;
		static const vedo_uint_t uVEDO_INT_BITS = 64;
	#endif   // _VEDO_INT64
#endif   // _VEDO_INT32

// Instead of float/double, please use "vedo_float_t" in whole system
#ifdef _VEDO_FLOAT32
	typedef vedo_float32_t vedo_float_t;
	static const vedo_uint_t uVEDO_FLOAT_BITS = 32;
#else
	#ifdef _VEDO_FLOAT64
		typedef vedo_float64_t vedo_float_t;
		static const vedo_uint_t uVEDO_FLOAT_BITS = 64;
	#else
		#ifdef _VEDO_FLOAT80
			typedef vedo_float80_t vedo_float_t;
			static const vedo_uint_t uVEDO_FLOAT_BITS = 80;
		#else
			// Default type
			typedef vedo_float64_t vedo_float_t;
			static const vedo_uint_t uVEDO_FLOAT_BITS = 64;
		#endif   // _VEDO_FLOAT80
	#endif   // _VEDO_FLOAT64
#endif   // _VEDO_FLOAT32

static const vedo_uint_t  uSizeOfConstant     = 3;     // dSafetyFactor, uNumUDDDOStatus, and uNumUDDImpactStatus
static const vedo_uint_t  uNumUDDDOStatus     = 0;     // Number of user-defined data in DOStatus
static const vedo_uint_t  uNumUDDImpactStatus = 0;     // Number of user-defined data in ImpactStatus



class Constants
{
public:

	static Constants* Instance();

	Constants(const Constants&);

	const Constants& operator = (const Constants&);

/*
	inline unsigned SizeOfConstant() const
	{
		return _uSizeOfConstant;
	}

	inline void SetSizeOfConstant(const unsigned& s)
	{
		_uSizeOfConstant = s;
	}
*/

	~Constants();

	inline std::string SysInfo() const
	{
		return _sSysInfo;
	}

	inline vedo_float_t SafetyFactor() const
	{
		return dSafetyFactor;
	}

	inline void SetSafetyFactor(const vedo_float_t& d)
	{
		dSafetyFactor = d;
	}

	inline vedo_uint_t ImpactBufferSize() const
	{
		return uImpactBufferSize;
	}

	inline void SetImpactBufferSize(const vedo_uint_t& u)
	{
		uImpactBufferSize = u;
	}

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

	//vedo_float_t String2Float(const std::string&);

	//vedo_int_t String2Int(const std::string&);

	//vedo_uint_t String2UInt(const std::string&);

private:

	static Constants* _Constants;

	std::string _sSysInfo;

//	unsigned          _uSizeOfConstant     = 3;     // dSafetyFactor, uNumUDDDOStatus, and uNumUDDImpactStatus

	void SetDefaultValue();

	vedo_float_t dSafetyFactor;   // Safety factor for contact detection

	vedo_uint_t uImpactBufferSize;   // Buffer size for the class "Consultant"
	// uImpactBufferSize ~= 18,000 for 60,000 elements / 4 processors
	// uImpactBufferSize ~= 60,000 for 60,000 elements / 8 processors

	Constants();

};

}   // namespace vedo

#endif // _VEDO_CONSTANTS_H
