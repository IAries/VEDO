#include <FrameWork/Interfaces/ContactDetector.h>

ContactDetector::ContactDetector()
{
	cInfo.dImpactDepth = 0.0;
	cInfo.dDistance    = 0.0;
	cInfo.dOverlapArea = 0.0;
	cInfo.bActive      = true;
	cInfo.bUnBalance   = false;
	pBC                = 0;
};
