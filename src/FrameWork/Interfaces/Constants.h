#ifndef _VEDO_CONSTANTS_H
#define _VEDO_CONSTANTS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

namespace VEDO
{

static const std::string sPublish            = "2013-03-24";
static       double      dSafetyFactor       = 1.1;
static const unsigned    uImpactBufferSize   = 65535;   // Consultant
// uImpactBufferSize ~= 18,000 for 60,000 elements / 4 processors
// uImpactBufferSize ~= 60,000 for 60,000 elements / 8 processors

static const unsigned    uNumUDDDOStatus     = 0;    // Number of user-defined data in DOStatus
static const unsigned    uNumUDDImpactStatus = 18;   // Number of user-defined data in ImpactStatus
};   // namespace VEDO

#endif _VEDO_CONSTANTS_H
