#ifndef _NJR_CONSTANTS_H
#define _NJR_CONSTANTS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

namespace NJR
{
// Common math constants
static const double      dPI             = M_PI;
static const double      dOneTwelfthPI   = M_PI / 12.0;
static const double      dOneEighthPI    = 0.125 * M_PI;   // M_PI/8
static const double      dOneSixthPI     = M_PI / 6.0;
static const double      dQuarterPI      = 0.25 * M_PI;    // M_PI/4
static const double      dOneThirdPI     = M_PI / 3.0;
static const double      dHalfPI         = 0.5 * M_PI;     // M_PI/2
static const double      dOneAndHalfPI   = 1.5 * M_PI;     // M_PI*3/2
static const double      dDoublePI       = 2.0 * M_PI;
static const double      dFourthThirdsPI = 4.0 / 3.0 * M_PI;

};   // namespace NJR



namespace NJRDXF
{

// Predefine Praticular Vectors
enum ParticularVector
{
    ORIGIN = 0x0000,
    AXIALX = 0x0001,
    AXIALY = 0x0002,
    AXIALZ = 0x0004,
    ZERO   = 0x0010,
};

enum Color
{
	bylayer	= 256,
	byblock	=   0,
	red	    =   1,
	yellow	=   2,
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

};   // namespace NJRDXF

#endif _NJR_CONSTANTS_H
