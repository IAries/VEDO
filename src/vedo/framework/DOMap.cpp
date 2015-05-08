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
