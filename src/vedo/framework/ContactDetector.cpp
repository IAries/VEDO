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
	cInfo.uShapeTypeMaster = DOShapeType::NoShapeType;
	cInfo.uShapeTypeSlave  = DOShapeType::NoShapeType;
	pBC                    = 0;

}

}   // namespace vedo
