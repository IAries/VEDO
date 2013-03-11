#include <FrameWork/Interfaces/DOMap.h>
#include <algorithm>

using namespace std;

double DOMap::CalSafeDistance(DOMap m, NJRvector3d vFF, double dt)
{
	switch (m._cpdoml->GetShapeType() )
	{
		case QuasiPlate:
//			return m._cpdoml->GetShapeAttributes().quasiplate.height;
			return
				(m._cpdos->GetVelocity()*dt).length()
				+ (vFF * (0.5*dt*dt)).length()
				+ m._cpdoml->GetShapeAttributes().quasiplate.height * 1.1;
		case QuasiCylinder:
//			return (m._cpdoml->GetShapeAttributes().quasicylinder.radius)*2.0;
			return
				(m._cpdos->GetVelocity()*dt).length()
				+ (vFF * (0.5*dt*dt)).length()
				+ m._cpdoml->GetShapeAttributes().quasicylinder.radius * 2.2;
		case Sphere:
			return
				(m._cpdos->GetVelocity()*dt).length()
				+ (vFF * (0.5*dt*dt)).length()
				+ (m._cpdoml->GetShapeAttributes().sphere.radius * 1.1);
		default:
			cerr << "DOShape is not in the list of LeapConsultant\n";
			exit(0);
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
		NJRvector3d vIm = m1._cpdos->GetPosition() - m2._cpdos ->GetPosition();
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere    )
		&& (m2._cpdoml->GetShapeType() == QuasiPlate) )
	{
		NJRvector3d vCa  = m1._cpdos->GetPosition();
		NJRvector3d vCb  = m2._cpdos->GetPosition();
		NJRvector3d vOz  = m2._cpdos->GetOrientationZ();
		NJRvector3d vOx  = m2._cpdos->GetOrientationX();
		NJRvector3d vOy  = vOz * vOx;
		NJRvector3d vCap = vCa - (vCa - vCb).ProjectOn(vOz);

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

		NJRvector3d vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
		NJRvector3d vIm   =  vCaps - vCa;
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere       )
		&& (m2._cpdoml->GetShapeType() == QuasiCylinder) )
	{
		double dHHb
			= 0.5 * (m2._cpdoml->GetShapeAttributes().quasicylinder.height);
		NJRvector3d Ca     = m1._cpdos->GetPosition();
		NJRvector3d Cb     = m2._cpdos->GetPosition();
		NJRvector3d Vaxial = m2._cpdos->GetOrientationZ();

		double Dap = (Ca - Cb)%Vaxial;

		NJRvector3d Cap    = Cb + (Vaxial * Dap);

		NJRvector3d vIm;

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

	cerr << "DOShape is not in the list of DOMap\n";
	exit(0);
};
*/

double DOMap::CalDistance(DOMap m1, DOMap m2, const Boundary* pbc)
{
	if (m1._cpdoml->GetShapeType() > m2._cpdoml->GetShapeType())
	{
		swap(m1, m2);
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere)
		&& (m2._cpdoml->GetShapeType() == Sphere) )
	{
		NJRvector3d vIm = m1._cpdos->GetPosition() - m2._cpdos ->GetPosition();
		if(pbc)
		{
			pbc->DifferenceBoundaryConditions(&vIm);
		};
		return vIm.length();
	}

	if (   (m1._cpdoml->GetShapeType() == Sphere    )
		&& (m2._cpdoml->GetShapeType() == QuasiPlate) )
	{
		NJRvector3d vCa  = m1._cpdos->GetPosition();
		NJRvector3d vCb  = m2._cpdos->GetPosition();
		NJRvector3d vOz  = m2._cpdos->GetOrientationZ();
		NJRvector3d vOx  = m2._cpdos->GetOrientationX();
		NJRvector3d vOy  = vOz * vOx;
		NJRvector3d vCap = vCa - (vCa - vCb).ProjectOn(vOz);

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

		NJRvector3d vCaps = (vOx * Dapx) + (vOy * Dapy) + vCb;
		NJRvector3d vIm   =  vCaps - vCa;
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
		NJRvector3d Ca     = m1._cpdos->GetPosition();
		NJRvector3d Cb     = m2._cpdos->GetPosition();
		NJRvector3d Vaxial = m2._cpdos->GetOrientationZ();

		double Dap = (Ca - Cb)%Vaxial;

		NJRvector3d Cap    = Cb + (Vaxial * Dap);

		NJRvector3d vIm;

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

	cerr << "DOShape is not in the list of DOMap\n";
	exit(0);
};

vector<DOMap> DOMap::GetDOMap(const DOWorld* World)
{
	unsigned long numberDO = World->GetSystemParameter()->GetDONumber();

	vector<DOMap> vDOMap(numberDO);

	for (unsigned int ul=0; ul<numberDO; ++ul)
	{
		const DOStatus* cpdos = World->GetDOStatus(ul);
		const DOModel* cpdoml = World->GetDOModel(cpdos->GetDOName());
		vDOMap[ul] = DOMap(ul, cpdos, cpdoml, cpdos->GetVelocity().length() );
	}

	return vDOMap;
};

void DOMap::ExtremeValue
	(vector<DOMap> Map,
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
