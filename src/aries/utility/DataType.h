#ifndef _ARIES_DATATYPE_H
#define _ARIES_DATATYPE_H

//#include <inttypes.h>
#include <string>

namespace aries
{
// The following data type make a system consistent in both LLP64 (Microsoft Windows) & LP64 (UNIX-like OS) platforms
// (Wikipedia: 64-bit computing, http://en.wikipedia.org/wiki/64-bit_computing)
// (Fundamental types, http://en.cppreference.com/w/cpp/language/types)
typedef   signed short     int    _int16_t  ;   // Range:                    -32,768 ~                     32,767
typedef unsigned short     int    _uint16_t ;   // Range:                          0 ~                     65,535
typedef   signed           int    _int32_t  ;   // Range:             -2,147,483,648 ~              2,147,483,647
typedef unsigned           int    _uint32_t ;   // Range:                          0 ~              4,294,697,295
// "unsigned/unsigned long int" is 32-bit in LLP64, but 64-bit in LP64 platform, that's why we don't use it.
typedef   signed long long int    _int64_t  ;   // Range: -9,223,372,036,854,775,808 ~  9,223,372,036,854,775,807
typedef unsigned long long int    _uint64_t ;   // Range:                          0 ~ 18,446,744,073,709,551,615
typedef                    float  _float32_t;   // Range:   1e-37 ~ 1e38
typedef                    double _float64_t;   // Range:  1e-307 ~ 1e308
typedef               long double _float80_t;   // Range: 1e-4931 ~ 1e4932



// Instead of int/unsigned int, please use "aries::_int_t"/"aries::_uint_t" in whole system
#ifdef _INT32
	typedef _int32_t  _int_t;
	typedef _uint32_t _uint_t;
#else
	#ifdef _INT64
		typedef _int64_t  _int_t;
		typedef _uint64_t _uint_t;
	#else
		// Default type
		typedef _int64_t  _int_t;
		typedef _uint64_t _uint_t;
	#endif   // _INT64
#endif   // _INT32



// Instead of float/double, please use "_float_t" in whole system
#ifdef _FLOAT32
	typedef _float32_t _float_t;
#else
	#ifdef _FLOAT64
		typedef _float64_t _float_t;
	#else
		#ifdef _FLOAT80
			typedef _float80_t _float_t;
		#else
			// Default type
			typedef _float64_t _float_t;
		#endif   // _FLOAT80
	#endif   // _FLOAT64
#endif   // _FLOAT32

}   // namespace aries

#endif   // _ARIES_DATATYPE_H
