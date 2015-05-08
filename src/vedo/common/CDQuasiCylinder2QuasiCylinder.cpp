#include <aries/utility/Constants.h>
#include <vedo/common/CDQuasiCylinder2QuasiCylinder.h>
#include <vedo/common/DOQuasiCylinder.h>
#include <cmath>

namespace vedo
{

CDQuasiCylinder_QuasiCylinder::CDQuasiCylinder_QuasiCylinder(): ContactDetector()
{
	cInfo.uShapeTypeMaster = DOShapeType::QuasiCylinder;
	cInfo.uShapeTypeSlave  = DOShapeType::QuasiCylinder;
}

void CDQuasiCylinder_QuasiCylinder::CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	Vector3df vCenterSlave      = pdoSlave->GetDOStatus()->GetPosition();
	Vector3df vDirectionSlave   = pdoSlave->GetDOStatus()->GetOrientationZ();
	_float_t  dRediusSlave      = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	_float_t  dHeightSlave      = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.height;
	Vector3df vEndPoint1OfSlave = vCenterSlave - 0.5 * dHeightSlave * vDirectionSlave;
	Vector3df vEndPoint2OfSlave = vCenterSlave + 0.5 * dHeightSlave * vDirectionSlave;
	// This is a testing class & still not finished.
}

void CDQuasiCylinder_QuasiCylinder::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// This is a testing class & still not finished.
}

}   // namespace vedo
