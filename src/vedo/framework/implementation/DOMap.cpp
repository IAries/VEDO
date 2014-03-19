#include <vedo/Constants.h>
#include <vedo/framework/interfaces/DOMap.h>
#include <algorithm>

namespace vedo
{

double DOMap::CalSafeDistance(DOMap m, njr::Vector3d vFF, double dt)
{
	switch (m._cpdoml->GetShapeType() )
	{
		case QuasiPlate:
//			return m._cpdoml->GetShapeAttributes().quasiplate.height;
			return
				(m._cpdos->GetVelocity()*dt).length()
				+ (vFF * (0.5*dt*dt)).length()
				+ m._cpdoml->GetShapeAttributes().quasiplate.height * vedo::dSafetyFactor;
		case QuasiPlateWithCircularHole:
//			return m._cpdoml->GetShapeAttributes().quasiplatewithcircularhole.height;
			return
				(m._cpdos->GetVelocity()*dt).length()
				+ (vFF * (0.5*dt*dt)).length()
				+ m._cpdoml->GetShapeAttributes().quasiplatewithcircularhole.height * vedo::dSafetyFactor;
		case QuasiCylinder:
//			return (m._cpdoml->GetShapeAttributes().quasicylinder.radius)*2.0;
			return
				(m._cpdos->GetVelocity()*dt).length()
				+ (vFF * (0.5*dt*dt)).length()
				+ m._cpdoml->GetShapeAttributes().quasicylinder.radius * 2.0 * vedo::dSafetyFactor;
		case Sphere:
			return
				(m._cpdos->GetVelocity()*dt).length()
				+ (vFF * (0.5*dt*dt)).length()
				+ (m._cpdoml->GetShapeAttributes().sphere.radius * vedo::dSafetyFactor);
		default:
			std::cerr
				<< "Error!! Code: DOMap::CalSafeDistance(DOMap, njr::Vector3d, double)" << std::endl
				<< "        Note: DOShape is not in the std::list of LeapConsultant" << std::endl;
			exit(-1);
	}
};

/*
double DOMap::CalDistance(DOMap m1, DOMap m2)
{
	if (m1._cpdoml->GetShapeType() > m2._cpdoml->GetShapeType())
	{
		swap(m1, m2);
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere)
		&& (m2._cpdoml->GetShapeType() == Sphere) )
	{
		njr::Vector3d vIm = m1._cpdos->GetPosition() - m2._cpdos ->GetPosition();
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere    )
		&& (m2._cpdoml->GetShapeType() == QuasiPlate) )
	{
		njr::Vector3d vCa  = m1._cpdos->GetPosition();
		njr::Vector3d vCb  = m2._cpdos->GetPosition();
		njr::Vector3d vOz  = m2._cpdos->GetOrientationZ();
		njr::Vector3d vOx  = m2._cpdos->GetOrientationX();
		njr::Vector3d vOy  = vOz * vOx;
		njr::Vector3d vCap = vCa - (vCa - vCb).ProjectOn(vOz);

		double dHWb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplate.width );
		double dHLb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplate.length);

		double Dapx = (vCap - vCb) % vOx;

		if ( Dapx < -dHWb)
		{
			Dapx = -dHWb;
		}
		else if ( Dapx >  dHWb)
		{
			Dapx = dHWb;
		}

		double Dapy = (vCap - vCb) % vOy;
		if (Dapy < -dHLb)
		{
			Dapy = -dHLb;
		}
		else if (Dapy > dHLb)
		{
			Dapy = dHLb;
		}

		njr::Vector3d vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
		njr::Vector3d vIm   =  vCaps - vCa;
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere       )
		&& (m2._cpdoml->GetShapeType() == QuasiCylinder) )
	{
		double dHHb
			= 0.5 * (m2._cpdoml->GetShapeAttributes().quasicylinder.height);
		njr::Vector3d Ca     = m1._cpdos->GetPosition();
		njr::Vector3d Cb     = m2._cpdos->GetPosition();
		njr::Vector3d Vaxial = m2._cpdos->GetOrientationZ();

		double Dap = (Ca - Cb)%Vaxial;

		njr::Vector3d Cap    = Cb + (Vaxial * Dap);

		njr::Vector3d vIm;

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
};
*/

double DOMap::CalDistance(DOMap m1, DOMap m2, const Boundary* pbc)
{
	if (m1._cpdoml->GetShapeType() > m2._cpdoml->GetShapeType())
	{
		std::swap(m1, m2);
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere)
		&& (m2._cpdoml->GetShapeType() == Sphere) )
	{
		njr::Vector3d vIm = m1._cpdos->GetPosition() - m2._cpdos ->GetPosition();
		if(pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		};
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere    )
		&& (m2._cpdoml->GetShapeType() == QuasiPlate) )
	{
		njr::Vector3d vCa  = m1._cpdos->GetPosition();
		njr::Vector3d vCb  = m2._cpdos->GetPosition();
		njr::Vector3d vOz  = m2._cpdos->GetOrientationZ();
		njr::Vector3d vOx  = m2._cpdos->GetOrientationX();
		njr::Vector3d vOy  = vOz * vOx;
		njr::Vector3d vCap = vCa - (vCa - vCb).ProjectOn(vOz);

		double dHWb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplate.width );
		double dHLb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplate.length);

		double Dapx = (vCap - vCb) % vOx;

		if ( Dapx < -dHWb)
		{
			Dapx = -dHWb;
		}
		else if ( Dapx >  dHWb)
		{
			Dapx = dHWb;
		}

		double Dapy = (vCap - vCb) % vOy;
		if (Dapy < -dHLb)
		{
			Dapy = -dHLb;
		}
		else if (Dapy > dHLb)
		{
			Dapy = dHLb;
		}

		njr::Vector3d vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
		njr::Vector3d vIm   =  vCaps - vCa;
		if(pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		};
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere                    )
		&& (m2._cpdoml->GetShapeType() == QuasiPlateWithCircularHole) )
	{
		njr::Vector3d vCa  = m1._cpdos->GetPosition();
		njr::Vector3d vCb  = m2._cpdos->GetPosition();
		njr::Vector3d vOz  = m2._cpdos->GetOrientationZ();
		njr::Vector3d vOx  = m2._cpdos->GetOrientationX();
		njr::Vector3d vOy  = vOz * vOx;
		njr::Vector3d vCap = vCa - (vCa - vCb).ProjectOn(vOz);

		double dHWb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplatewithcircularhole.width );
		double dHLb = 0.5*(m2._cpdoml->GetShapeAttributes().quasiplatewithcircularhole.length);

		double Dapx = (vCap - vCb) % vOx;

		if ( Dapx < -dHWb)
		{
			Dapx = -dHWb;
		}
		else if ( Dapx >  dHWb)
		{
			Dapx = dHWb;
		}

		double Dapy = (vCap - vCb) % vOy;
		if (Dapy < -dHLb)
		{
			Dapy = -dHLb;
		}
		else if (Dapy > dHLb)
		{
			Dapy = dHLb;
		}

		njr::Vector3d vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
		njr::Vector3d vIm   =  vCaps - vCa;
		if(pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		};
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere       )
		&& (m2._cpdoml->GetShapeType() == QuasiCylinder) )
	{
		double dHHb
			= 0.5 * (m2._cpdoml->GetShapeAttributes().quasicylinder.height);
		njr::Vector3d Ca     = m1._cpdos->GetPosition();
		njr::Vector3d Cb     = m2._cpdos->GetPosition();
		njr::Vector3d Vaxial = m2._cpdos->GetOrientationZ();

		double Dap = (Ca - Cb)%Vaxial;

		njr::Vector3d Cap    = Cb + (Vaxial * Dap);

		njr::Vector3d vIm;

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

		if(pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		};
		return vIm.length();
	}

	std::cerr
		<< "Error!! DOMap::CalDistance(DOMap, DOMap, const Boundary*)" << std::endl
		<< "        Note: DOShape is not in the std::list of DOMap" << std::endl;
	exit(-1);
};

std::vector<DOMap> DOMap::GetDOMap(const DOWorld* World)
{
	unsigned long numberDO = World->GetSystemParameter()->GetDONumber();

	std::vector<DOMap> vDOMap(numberDO);

	for (unsigned int ul=0; ul<numberDO; ++ul)
	{
		const DOStatus* cpdos = World->GetDOStatus(ul);
		const DOModel* cpdoml = World->GetDOModel(cpdos->GetDOName());
		vDOMap[ul] = DOMap(ul, cpdos, cpdoml, cpdos->GetVelocity().length() );
	}

	return vDOMap;
};

void DOMap::ExtremeValue
	(std::vector<DOMap> Map,
	 double& maxX,
	 double& minX,
	 double& maxY,
	 double& minY,
	 double& maxZ,
	 double& minZ,
	 double& maxR,
	 double& maxV,
	 double& maxS      )
{
	maxX
		= max_element(Map.begin(), Map.end(), ComX)->cpdos()->GetPosition().x();
	minX
		= min_element(Map.begin(), Map.end(), ComX)->cpdos()->GetPosition().x();
	maxY
		= max_element(Map.begin(), Map.end(), ComY)->cpdos()->GetPosition().y();
	minY
		= min_element(Map.begin(), Map.end(), ComY)->cpdos()->GetPosition().y();
	maxZ
		= max_element(Map.begin(), Map.end(), ComZ)->cpdos()->GetPosition().z();
	minZ
		= min_element(Map.begin(), Map.end(), ComZ)->cpdos()->GetPosition().z();
	maxV
		= max_element(Map.begin(), Map.end(), ComV)
			->cpdos()->GetVelocity().length();
	maxR
		= max_element(Map.begin(), Map.end(), ComR)
			->cpdoml()->GetShapeAttributes().sphere.radius;
	maxS
		= max_element(Map.begin(), Map.end(), ComS)->SafeLength();
};

};   // namespace vedo
