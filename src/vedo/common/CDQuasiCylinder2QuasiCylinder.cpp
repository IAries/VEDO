#include <aries/utility/Constants.h>
#include <vedo/common/CDQuasiCylinder2QuasiCylinder.h>
#include <vedo/common/DOQuasiCylinder.h>
#include <cmath>

namespace vedo
{

void CDQuasiCylinder_QuasiCylinder::CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	aries::Vector3df vCenterSlave      = pdoSlave->GetDOStatus()->GetPosition();
	aries::Vector3df vDirectionSlave   = pdoSlave->GetDOStatus()->GetOrientationZ();
	_float_t  dRediusSlave      = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	_float_t  dHeightSlave      = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.height;
	aries::Vector3df vEndPoint1OfSlave = vCenterSlave - 0.5 * dHeightSlave * vDirectionSlave;
	aries::Vector3df vEndPoint2OfSlave = vCenterSlave + 0.5 * dHeightSlave * vDirectionSlave;
	// This is a testing class & still not finished.
}

void CDQuasiCylinder_QuasiCylinder::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// This is a testing class & still not finished.
}

}   // namespace vedo
