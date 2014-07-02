#ifndef _VEDO_CONSTANTS_H
#define _VEDO_CONSTANTS_H

//#include <inttypes.h>
#include <string>

namespace vedo
{
typedef unsigned __int64 vedo_unsigned_long;
typedef          __int64 vedo_long;
//typedef uint64_t vedo_unsigned_long;
//typedef int64_t  vedo_long;

static const std::string sVersion            = "X4 build 702";
static const std::string sPublish            = "2014-07-02";

static unsigned          uSizeOfConstant     = 3;     // dSafetyFactor, uNumUDDDOStatus, and uNumUDDImpactStatus
static double            dSafetyFactor       = 1.1;   // Safety factor for contact detection
static unsigned          uNumUDDDOStatus     = 0;     // Number of user-defined data in DOStatus
static unsigned          uNumUDDImpactStatus = 0;     // Number of user-defined data in ImpactStatus

static const unsigned    uImpactBufferSize   = 65535;   // Buffer size for the class "Consultant"
// uImpactBufferSize ~= 18,000 for 60,000 elements / 4 processors
// uImpactBufferSize ~= 60,000 for 60,000 elements / 8 processors
};   // namespace vedo

#endif // _VEDO_CONSTANTS_H


