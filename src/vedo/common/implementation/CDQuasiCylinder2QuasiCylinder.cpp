#include <vedo/njr/interfaces/Constants.h>
#include <vedo/common/interfaces/CDQuasiCylinder2QuasiCylinder.h>
#include <vedo/common/interfaces/DOQuasiCylinder.h>
#include <cmath>

namespace vedo
{

void CDQuasiCylinder_QuasiCylinder::CalDistance(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	njr::Vector3d vCenterSlave      = pdoSlave->GetDOStatus()->GetPosition();
	njr::Vector3d vDirectionSlave   = pdoSlave->GetDOStatus()->GetOrientationZ();
	vedo_float_t  dRediusSlave      = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	vedo_float_t  dHeightSlave      = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.height;
	njr::Vector3d vEndPoint1OfSlave = vCenterSlave - 0.5 * dHeightSlave * vDirectionSlave;
	njr::Vector3d vEndPoint2OfSlave = vCenterSlave + 0.5 * dHeightSlave * vDirectionSlave;
	// This is a testing class & still not finished.
}

void CDQuasiCylinder_QuasiCylinder::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	// This is a testing class & still not finished.
}

}   // namespace vedo
