#include <vedo/framework/ContactDetector.h>

namespace vedo
{

ContactDetector::ContactDetector()
{
	cInfo.dImpactDepth     = 0.0;
	cInfo.dDistance        = 0.0;
	cInfo.dOverlapArea     = 0.0;
	cInfo.bActive          = true;
	cInfo.bUnBalance       = false;
	cInfo.uShapeTypeMaster = DOShapeType::NoType;
	cInfo.uShapeTypeSlave  = DOShapeType::NoType;
	pBC                    = 0;

}

}   // namespace vedo
