#include <aries/utility/Constants.h>
#include <vedo/framework/DOMap.h>
#include <algorithm>

namespace vedo
{

_float_t DOMap::CalSafeDistance(DOMap m, Vector3df vFF, _float_t dt)
{
	aries::Constants* aries_cp = aries::Constants::Instance();
	_float_t SafetyFactor = aries_cp->SafetyFactor();
	switch (m._cpdoml->GetShapeType() )
	{
		case QuasiPlate:
//			return m._cpdoml->GetShapeAttributes().quasiplate.height;
			return
				(m._cpdos->GetVelocity()*dt).length()
				 + (vFF * (0.5*dt*dt)).length()
				 + m._cpdoml->GetShapeAttributes().quasiplate.height * SafetyFactor;
		case QuasiPlateWithCircularHole:
//			return m._cpdoml->GetShapeAttributes().quasiplatewithcircularhole.height;
			return
				(m._cpdos->GetVelocity()*dt).length()
				 + (vFF * (0.5*dt*dt)).length()
				 + m._cpdoml->GetShapeAttributes().quasiplatewithcircularhole.height * SafetyFactor;
		case QuasiCylinder:
//			return (m._cpdoml->GetShapeAttributes().quasicylinder.radius)*2.0;
			return
				(m._cpdos->GetVelocity()*dt).length()
				 + (vFF * (0.5*dt*dt)).length()
				 + m._cpdoml->GetShapeAttributes().quasicylinder.radius * 2.0 * SafetyFactor;
		case Sphere:
			return
				(m._cpdos->GetVelocity()*dt).length()
				 + (vFF * (0.5*dt*dt)).length()
				 + (m._cpdoml->GetShapeAttributes().sphere.radius * SafetyFactor);
		case Triangle:
			return
				(m._cpdos->GetVelocity()*dt).length()
				 + (vFF * (0.5*dt*dt)).length()
				 + (m._cpdoml->GetShapeAttributes().triangle.range * SafetyFactor);
		default:
			std::cerr
				<< "Error!! Code: DOMap::CalSafeDistance(DOMap, Vector3df, _float_t)" << std::endl
				<< "        Note: DOShape is not in the std::list of LeapConsultant"          << std::endl;
			exit(-1);
	}
}

/*
_float_t DOMap::CalDistance(DOMap m1, DOMap m2)
{
	if (m1._cpdoml->GetShapeType() > m2._cpdoml->GetShapeType())
	{
		swap(m1, m2);
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere)
		&& (m2._cpdoml->GetShapeType() == Sphere) )
	{
		Vector3df vIm = m1._cpdos->GetPosition() - m2._cpdos ->GetPosition();
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere    )
		&& (m2._cpdoml->GetShapeType() == QuasiPlate) )
	{
		Vector3df vCa  = m1._cpdos->GetPosition();
		Vector3df vCb  = m2._cpdos->GetPosition();
		Vector3df vOz  = m2._cpdos->GetOrientationZ();
		Vector3df vOx  = m2._cpdos->GetOrientationX();
		Vector3df vOy  = vOz * vOx;
		Vector3df vCap = vCa - (vCa - vCb).project_on(vOz);

		_float_t dHWb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplate.width );
		_float_t dHLb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplate.length);

		_float_t Dapx = (vCap - vCb) % vOx;

		if ( Dapx < -dHWb)
		{
			Dapx = -dHWb;
		}
		else if ( Dapx >  dHWb)
		{
			Dapx = dHWb;
		}

		_float_t Dapy = (vCap - vCb) % vOy;
		if (Dapy < -dHLb)
		{
			Dapy = -dHLb;
		}
		else if (Dapy > dHLb)
		{
			Dapy = dHLb;
		}

		Vector3df vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
		Vector3df vIm   =  vCaps - vCa;
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere       )
		&& (m2._cpdoml->GetShapeType() == QuasiCylinder) )
	{
		_float_t dHHb
			= 0.5 * (m2._cpdoml->GetShapeAttributes().quasicylinder.height);
		Vector3df Ca     = m1._cpdos->GetPosition();
		Vector3df Cb     = m2._cpdos->GetPosition();
		Vector3df Vaxial = m2._cpdos->GetOrientationZ();

		_float_t Dap = (Ca - Cb)%Vaxial;

		Vector3df Cap    = Cb + (Vaxial * Dap);

		Vector3df vIm;

		if ( (Dap < dHHb) && (Dap > -dHHb) )
		{
			vIm = Cap - Ca;
		}
		else if ( Dap >= dHHb)
		{
			vIm = Cb + (Vaxial*dHHb) - Ca;
		}
		else
		{
			vIm = Cb - (Vaxial*dHHb) - Ca;
		}

		return vIm.length();
	}

	std::cerr
		<< "Error!! Code: DOMap::CalDistance(DOMap, DOMap)" << std::endl
		<< "        Note: DOShape is not in the std::list of DOMap" << std::endl;
	exit(-1);
}
*/

_float_t DOMap::CalDistance(DOMap m1, DOMap m2, const Boundary* pbc)
{
	if (m1._cpdoml->GetShapeType() > m2._cpdoml->GetShapeType())
	{
		std::swap(m1, m2);
	}

	if ((m1._cpdoml->GetShapeType() == Sphere) && (m2._cpdoml->GetShapeType() == Sphere))
	{
		Vector3df vIm = m1._cpdos->GetPosition() - m2._cpdos ->GetPosition();
		if (pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		}
		return vIm.length();
	}

	/*
	if ((m1._cpdoml->GetShapeType() == Sphere) && (m2._cpdoml->GetShapeType() == Triangle))
	{
		// m1 (Sphere)
		Vector3df vSphereCenter     = m1._cpdos->GetPosition();
		_float_t fRadius            = m1._cpdoml->GetShapeAttributes().sphere.radius;

		// m2 (Triangle)
		Vector3df vNormal           = m2._cpdos->GetOrientationZ();
		DOShapeAttributes dossa     = m2._cpdoml->GetShapeAttributes();
		Vector3df vPoint0           = Vector3df(dossa.triangle.point0x, dossa.triangle.point0y, dossa.triangle.point0z);
		Vector3df vPoint1           = Vector3df(dossa.triangle.point1x, dossa.triangle.point1y, dossa.triangle.point1z);
		Vector3df vPoint2           = Vector3df(dossa.triangle.point2x, dossa.triangle.point2y, dossa.triangle.point2z);
		Vector3df vTriangleCenter   = m2._cpdos->GetPosition();

		// Projected point of sphere center on triangle plane
		Vector3df vS2               = vSphereCenter - vSphereCenter.dot(vNormal) * vNormal;

		Vector3df vCenterToCenter   = vTriangleCenter - vSphereCenter;
		Vector3df vImpactDirection  = vCenterToCenter.direction();


		_float_t fProjectedDistance = (vSphereCenter - vTriangleCenter).dot(vNormal);
		Vector3df vProjectedPointOnTrianglePlate = vSphereCenter - fProjectedDistance * vNormal;
		_float_t dImpactDepth;
		Vector3df vImpactPoint;
		if (std::abs(fProjectedDistance) >= fRadius)
		{
			dImpactDepth = 0.0;
			vImpactPoint = vProjectedPointOnTrianglePlate;
		}
		else
		{
			Vector3df v0 = vPoint2 - vPoint0;
			Vector3df v1 = vPoint1 - vPoint0;
			Vector3df v2 = vProjectedPointOnTrianglePlate - vPoint0;
			_float_t fDot00 = v0.dot(v0);
			_float_t fDot01 = v0.dot(v1);
			_float_t fDot02 = v0.dot(v2);
			_float_t fDot11 = v1.dot(v1);
			_float_t fDot12 = v1.dot(v2);
			_float_t fInverDeno = 1.0 / (fDot00 * fDot11 - fDot01 * fDot01);
			_float_t fu = (fDot11 * fDot02 - fDot01 * fDot12) * fInverDeno;
			bool Outer = false;
			if ((fu < 0.0) || (fu > 1.0))
			{
				Outer = true;
			}
			else
			{
				_float_t fv = (fDot00 * fDot12 - fDot01 * fDot02) * fInverDeno;
				if ((fv < 0.0) || (fv > 1.0))
				{
					Outer = true;
				}
				else
				{
					if ((fu + fv) > 1.0)
					{
						Outer = true;
					}
					else
					{
						if (vImpactDirection.dot(vNormal) >= 0.0)
						{
							vImpactDirection = vNormal;
						}
						else
						{
							vImpactDirection = -vNormal;
						}
						dImpactDepth = (vProjectedPointOnTrianglePlate - vSphereCenter).length();
						vImpactPoint = vS2;
					}
				}
			}
			if (Outer)
			{
				Vector3df vEdge01             = (vPoint1 - vPoint0).direction();
				Vector3df vEdge12             = (vPoint2 - vPoint1).direction();
				Vector3df vEdge20             = (vPoint0 - vPoint2).direction();
				Vector3df v0S2                = vS2 - vPoint0;
				Vector3df v1S2                = vS2 - vPoint1;
				Vector3df v2S2                = vS2 - vPoint2;
				Vector3df vSProjectedOnEdge01 = vPoint0 + v0S2.dot(vEdge01) * vEdge01;
				Vector3df vSProjectedOnEdge12 = vPoint1 + v1S2.dot(vEdge12) * vEdge12;
				Vector3df vSProjectedOnEdge20 = vPoint2 + v2S2.dot(vEdge20) * vEdge20;
				_float_t  fDistanceToEdge01   = (vSphereCenter - vSProjectedOnEdge01).length();
				_float_t  fDistanceToEdge12   = (vSphereCenter - vSProjectedOnEdge12).length();
				_float_t  fDistanceToEdge20   = (vSphereCenter - vSProjectedOnEdge20).length();
				_float_t  fClosestDistance    = fDistanceToEdge01;
				vImpactPoint                  = vSProjectedOnEdge01;
				if (fClosestDistance > fDistanceToEdge12)
				{
					fClosestDistance   = fDistanceToEdge12;
					vImpactPoint = vSProjectedOnEdge12;
				}
				if (fClosestDistance > fDistanceToEdge20)
				{
					fClosestDistance   = fDistanceToEdge20;
					vImpactPoint = vSProjectedOnEdge20;
				}
				dImpactDepth     = fRadius - fClosestDistance;
				vImpactDirection = vS2 - vSphereCenter;
			}
		}

		Vector3df vIm = vSphereCenter - vImpactPoint;
		if (pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		}
		return vIm.length();
	}
	*/

	if ((m1._cpdoml->GetShapeType() == Sphere) && (m2._cpdoml->GetShapeType() == Triangle))
	{
		// m1 (Sphere)
		Vector3df vSphereCenter     = m1._cpdos->GetPosition();
		_float_t fRadius            = m1._cpdoml->GetShapeAttributes().sphere.radius;

		// m2 (Triangle)
		_float_t fRange             = m2._cpdoml->GetShapeAttributes().triangle.range;
		Vector3df vTriangleCenter   = m2._cpdos->GetPosition();

		Vector3df vIm = vSphereCenter - vTriangleCenter;
		vIm = vIm + 0.5 * fRange * vIm.direction();
		if (pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		}
		return vIm.length();
	}

	if ((m1._cpdoml->GetShapeType() == Sphere) && (m2._cpdoml->GetShapeType() == QuasiPlate))
	{
		Vector3df vCa  = m1._cpdos->GetPosition();
		Vector3df vCb  = m2._cpdos->GetPosition();
		Vector3df vOz  = m2._cpdos->GetOrientationZ();
		Vector3df vOx  = m2._cpdos->GetOrientationX();
		Vector3df vOy  = vOz.cross(vOx);
		Vector3df vCap = vCa - (vCa - vCb).project_on(vOz);

		_float_t dHWb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplate.width );
		_float_t dHLb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplate.length);

		_float_t Dapx = (vCap - vCb).dot(vOx);

		if ( Dapx < -dHWb)
		{
			Dapx = -dHWb;
		}
		else if ( Dapx >  dHWb)
		{
			Dapx = dHWb;
		}

		_float_t Dapy = (vCap - vCb).dot(vOy);
		if (Dapy < -dHLb)
		{
			Dapy = -dHLb;
		}
		else if (Dapy > dHLb)
		{
			Dapy = dHLb;
		}

		Vector3df vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
		Vector3df vIm   =  vCaps - vCa;
		if (pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		}
		return vIm.length();
	}

	if ((m1._cpdoml->GetShapeType() == Sphere) && (m2._cpdoml->GetShapeType() == QuasiPlateWithCircularHole))
	{
		Vector3df vCa  = m1._cpdos->GetPosition();
		Vector3df vCb  = m2._cpdos->GetPosition();
		Vector3df vOz  = m2._cpdos->GetOrientationZ();
		Vector3df vOx  = m2._cpdos->GetOrientationX();
		Vector3df vOy  = vOz.cross(vOx);
		Vector3df vCap = vCa - (vCa - vCb).project_on(vOz);

		_float_t dHWb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplatewithcircularhole.width );
		_float_t dHLb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplatewithcircularhole.length);

		_float_t Dapx = (vCap - vCb).dot(vOx);

		if ( Dapx < -dHWb)
		{
			Dapx = -dHWb;
		}
		else if ( Dapx >  dHWb)
		{
			Dapx = dHWb;
		}

		_float_t Dapy = (vCap - vCb).dot(vOy);
		if (Dapy < -dHLb)
		{
			Dapy = -dHLb;
		}
		else if (Dapy > dHLb)
		{
			Dapy = dHLb;
		}

		Vector3df vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
		Vector3df vIm   =  vCaps - vCa;
		if (pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		}
		return vIm.length();
	}

	if ((m1._cpdoml->GetShapeType() == Sphere) && (m2._cpdoml->GetShapeType() == QuasiCylinder))
	{
		_float_t  dHHb   = 0.5 * (m2._cpdoml->GetShapeAttributes().quasicylinder.height);
		Vector3df Ca     = m1._cpdos->GetPosition();
		Vector3df Cb     = m2._cpdos->GetPosition();
		Vector3df Vaxial = m2._cpdos->GetOrientationZ();
		_float_t Dap     = (Ca - Cb).dot(Vaxial);
		Vector3df Cap    = Cb + (Vaxial * Dap);

		Vector3df vIm;

		if ((Dap < dHHb) && (Dap > -dHHb))
		{
			vIm = Cap - Ca;
		}
		else if (Dap >= dHHb)
		{
			vIm = Cb + (Vaxial*dHHb) - Ca;
		}
		else
		{
			vIm = Cb - (Vaxial*dHHb) - Ca;
		}

		if (pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		}

		return vIm.length();
	}

	std::cerr
		<< "Error!! DOMap::CalDistance(DOMap, DOMap, const Boundary*)" << std::endl
		<< "        Note: DOShape is not in the std::list of DOMap"    << std::endl;
	exit(-1);
}

std::vector<DOMap> DOMap::GetDOMap(const DOWorld* World)
{
	_uint_t numberDO = World->GetSystemParameter()->GetDONumber();

	std::vector<DOMap> vDOMap(numberDO);

	for (_uint_t ul=0; ul<numberDO; ++ul)
	{
		const DOStatus* cpdos = World->GetDOStatus(ul);
		const DOModel* cpdoml = World->GetDOModel(cpdos->GetDOName());
		vDOMap[ul] = DOMap(ul, cpdos, cpdoml, cpdos->GetVelocity().length() );
	}

	return vDOMap;
}

void DOMap::ExtremeValue
	(std::vector<DOMap> Map,
	 _float_t& maxX, _float_t& minX,
	 _float_t& maxY, _float_t& minY,
	 _float_t& maxZ, _float_t& minZ,
	 _float_t& maxR, _float_t& maxV, _float_t& maxS)
{
	maxX = max_element(Map.begin(), Map.end(), ComX)->cpdos()->GetPosition().x();
	minX = min_element(Map.begin(), Map.end(), ComX)->cpdos()->GetPosition().x();
	maxY = max_element(Map.begin(), Map.end(), ComY)->cpdos()->GetPosition().y();
	minY = min_element(Map.begin(), Map.end(), ComY)->cpdos()->GetPosition().y();
	maxZ = max_element(Map.begin(), Map.end(), ComZ)->cpdos()->GetPosition().z();
	minZ = min_element(Map.begin(), Map.end(), ComZ)->cpdos()->GetPosition().z();
	maxV = max_element(Map.begin(), Map.end(), ComV)->cpdos()->GetVelocity().length();
	maxR = max_element(Map.begin(), Map.end(), ComR)->cpdoml()->GetShapeAttributes().sphere.radius;
	maxS = max_element(Map.begin(), Map.end(), ComS)->SafeLength();
}

OutOfArea::OutOfArea(): Xn(0.0), Xp(0.0), Yn(0.0), Yp(0.0), Zn(0.0), Zp(0.0)
{
}

OutOfArea::~OutOfArea()
{
}

OutOfArea::OutOfArea(const OutOfArea& ooa): Xn(ooa.Xn), Xp(ooa.Xp), Yn(ooa.Yn), Yp(ooa.Yp), Zn(ooa.Zn), Zp(ooa.Zp)
{
}

const OutOfArea& OutOfArea::operator = (const OutOfArea& ooa)
{
	*this = ooa;
	return *this;
}

}   // namespace vedo
