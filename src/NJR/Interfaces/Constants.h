#ifndef _NJR_CONSTANTS_H
#define _NJR_CONSTANTS_H

//#define _USE_MATH_DEFINES
//#include <math.h>
#include <cmath>
#include <string>

namespace njr
{
// Common math constants
//static const double dPI             = M_PI;
static const double dPI             = 4.0 * std::atan(1.0);
static const double dOneTwelfthPI   = dPI / 12.0;
static const double dOneEighthPI    = 0.125 * dPI;   // dPI/8
static const double dOneSixthPI     = dPI / 6.0;
static const double dQuarterPI      = 0.25 * dPI;    // dPI/4
static const double dOneThirdPI     = dPI / 3.0;
static const double dHalfPI         = 0.5 * dPI;     // dPI/2
static const double dOneAndHalfPI   = 1.5 * dPI;     // dPI*3/2
static const double dDoublePI       = 2.0 * dPI;
static const double dFourthThirdsPI = 4.0 / 3.0 * dPI;
static const double dDegree2PI      = 1.0 / 180.0 * dPI;

// Predefine Praticular Vectors, its activities will be defined in class "Vector2d" & "Vector3d"
enum ParticularVector {ZERO, ORIGIN, AXIALX, AXIALY, AXIALZ};
};   // namespace njr



namespace njrdxf
{

enum Color   // size: unsigned __int32
{
	bylayer = 256,
	byblock =   0,
	red	    =   1,
	yellow  =   2,
	green   =   3,
	cyan    =   4,
	blue    =   5,
	magenta =   6
};

enum Code
{
	type     =  0,
	layer    =  8,
	color    = 62,
	handle   =  5,
	keyword  =  0,
	secname  =  2,
	pheight  = 40,
	pangle   = 50,
	ptext    =  1,
	px0      = 10,
	py0      = 20,
	pz0      = 30,
	px1      = 11,
	py1      = 21,
	pz1      = 31,
	px2      = 12,
	py2      = 22,
	pz2      = 32,
	px3      = 13,
	py3      = 23,
	pz3      = 33,
	submaker = 100
};

};   // namespace njrdxf

#endif // _NJR_CONSTANTS_H
