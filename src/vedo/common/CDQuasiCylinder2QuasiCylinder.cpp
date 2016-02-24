#include <aries/utility/Utility.h>
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
	Vector3df vCenterSlave     = pdoSlave->GetDOStatus()->GetPosition();
	Vector3df vDirectionSlave  = pdoSlave->GetDOStatus()->GetOrientationZ();
	_float_t  dRediusSlave     = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	_float_t  dHeightSlave     = pdoSlave->GetDOModel()->GetShapeAttributes().quasicylinder.height;
	Line3df LineSlave(vCenterSlave, vDirectionSlave, dHeightSlave);

	Vector3df vCenterMaster    = pdoMaster->GetDOStatus()->GetPosition();
	Vector3df vDirectionMaster = pdoMaster->GetDOStatus()->GetOrientationZ();
	_float_t  dRediusMaster    = pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.radius;
	_float_t  dHeightMaster    = pdoMaster->GetDOModel()->GetShapeAttributes().quasicylinder.height;
	Line3df LineMaster(vCenterMaster, vDirectionMaster, dHeightMaster);

	cInfo.vCenterToCenter = vCenterMaster - vCenterSlave;

	if (std::abs(std::abs(vDirectionSlave.dot(vDirectionMaster)) - 1.0) <= 1.0e-5)
	{
		cInfo.vImpactDirection = LineMaster.p1() - LineSlave.p1() - (LineMaster.p1() - LineSlave.p1()).dot(vDirectionSlave) * vDirectionSlave;
		cInfo.dImpactDepth = dRediusSlave + dRediusMaster - cInfo.vImpactDirection.length();
		cInfo.vImpactDirection.normalized();
		cInfo.dOverlapArea = 0.0;   // Aries: We still not finished this part

		Vector3df vLineMasterP1OnLineSlave = LineSlave.p1()  + (LineMaster.p1() - LineSlave.p1() ).dot(vDirectionSlave ) * vDirectionSlave;
		Vector3df vLineMasterP2OnLineSlave = LineSlave.p1()  + (LineMaster.p2() - LineSlave.p1() ).dot(vDirectionSlave ) * vDirectionSlave;
		Vector3df vLineSlaveP1OnLineMaster = LineMaster.p1() + (LineSlave.p1()  - LineMaster.p1()).dot(vDirectionMaster) * vDirectionMaster;
		Vector3df vLineSlaveP2OnLineMaster = LineMaster.p1() + (LineSlave.p2()  - LineMaster.p1()).dot(vDirectionMaster) * vDirectionMaster;

		Vector3df vValidP1OnLineSlave;
		if (LineSlave.InCoverSpace(vLineMasterP1OnLineSlave))
		{
			vValidP1OnLineSlave = vLineMasterP1OnLineSlave;
		}
		else
		{
			vValidP1OnLineSlave = LineSlave.p1();
		}

		Vector3df vValidP2OnLineSlave;
		if (LineSlave.InCoverSpace(vLineMasterP2OnLineSlave))
		{
			vValidP2OnLineSlave = vLineMasterP2OnLineSlave;
		}
		else
		{
			vValidP2OnLineSlave = LineSlave.p2();
		}

		Vector3df vValidP1OnLineMaster;
		if (LineMaster.InCoverSpace(vLineSlaveP1OnLineMaster))
		{
			vValidP1OnLineMaster = vLineSlaveP1OnLineMaster;
		}
		else
		{
			vValidP1OnLineMaster = LineMaster.p1();
		}

		Vector3df vValidP2OnLineMaster;
		if (LineMaster.InCoverSpace(vLineSlaveP2OnLineMaster))
		{
			vValidP2OnLineMaster = vLineSlaveP2OnLineMaster;
		}
		else
		{
			vValidP2OnLineMaster = LineMaster.p2();
		}

		cInfo.vImpactPoint = 0.5 * (vValidP1OnLineSlave  + vValidP2OnLineSlave) + (dRediusSlave - 0.5 * cInfo.dImpactDepth) * cInfo.vImpactDirection;
	}
	else
	{
		// Find the common perpendicular "vP-vQ" (vP on pdoSlave; vQ on pdoMaster) on the infinite central lines of two cylinders
		Line3df PQ = aries::CommonPerpendicular(LineSlave, LineMaster);
		Vector3df vP = PQ.p1();
		Vector3df vQ = PQ.p2();
		if (LineSlave.InCoverSpace(vP) && LineMaster.InCoverSpace(vQ))
		{
			cInfo.dImpactDepth = dRediusSlave + dRediusMaster - (vQ - vP).length();
			cInfo.dOverlapArea = 0.0;   // Aries: We still not finished this part
			cInfo.vImpactDirection = (vQ - vP).direction();
			cInfo.vImpactPoint = vP + (dRediusSlave - 0.5 * cInfo.dImpactDepth) * cInfo.vImpactDirection;
		}
		else
		{
			// Aries: We can deak with this condition more carefully in the future.
			cInfo.dImpactDepth = 0.0;
			cInfo.dOverlapArea = 0.0;   // Aries: We still not finished this part
			cInfo.vImpactDirection = (vQ - vP).direction();
			cInfo.vImpactPoint = vP + (dRediusSlave - 0.5 * cInfo.dImpactDepth) * cInfo.vImpactDirection;
		}
	}
}

void CDQuasiCylinder_QuasiCylinder::Detect(const DiscreteObject* pdoSlave, const DiscreteObject* pdoMaster)
{
	CDQuasiCylinder_QuasiCylinder::CalDistance(pdoSlave, pdoMaster);

	if (cInfo.dImpactDepth > 0)
	{
		cInfo.bUnBalance = (cInfo.bActive == false);
		cInfo.bActive = true;
	}
	else
	{
		cInfo.bActive    = false;
		cInfo.bUnBalance = false;
	}
}

}   // namespace vedo
