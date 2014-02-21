#ifndef _VEDO_CONSTANTS_H
#define _VEDO_CONSTANTS_H

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

namespace vedo
{
static const std::string sVersion            = "X4 build 221";
static const std::string sPublish            = "2014-02-21";
static       double      dSafetyFactor       = 1.1;
static const unsigned    uImpactBufferSize   = 65535;   // Consultant
// uImpactBufferSize ~= 18,000 for 60,000 elements / 4 processors
// uImpactBufferSize ~= 60,000 for 60,000 elements / 8 processors

static unsigned          uNumUDDDOStatus     = 0;   // Number of user-defined data in DOStatus
static unsigned          uNumUDDImpactStatus = 0;   // Number of user-defined data in ImpactStatus
};   // namespace vedo

#endif _VEDO_CONSTANTS_H

