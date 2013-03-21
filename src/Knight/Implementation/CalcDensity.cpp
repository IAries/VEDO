#include <NJR/Interfaces/vector3d.h>
#include <NJR/Interfaces/random.h>
#include <FrameWork/Interfaces/DOMap.h>
#include <Knight/Interfaces/CalcDensity.h>

using namespace std;

const int CalcDensity::RandomHitTestNum = 20000;

double CalcDensity::computeDensity
	(const VEDO::DOWorld* pWorld,
	double xmin,
	double xmax,
	double ymin,
	double ymax,
	double zmin,
	double zmax ) const
{
	std::vector<VEDO::DOMap> vDOMap;
	double xp, yp, zp, radius;
	for (unsigned int i=0; i<(pWorld->GetSystemParameter()->GetDONumber()) ;++i)
	{
		const VEDO::DOStatus* pdos = pWorld->GetDOStatus(i);
		const VEDO::DOModel* pmodel = pWorld->GetDOModel(pdos->GetDOName());
		if (pmodel->GetShapeType() != VEDO::Sphere)
		{
			continue;
		}
		xp = pdos->GetPosition().x();
		yp = pdos->GetPosition().y();
		zp = pdos->GetPosition().z();
		radius = pmodel->GetShapeAttributes().sphere.radius;
		if (   (xp < (xmax+radius))
			&& (xp > (xmin-radius))
			&& (yp < (ymax+radius))
			&& (yp > (ymin-radius))
			&& (zp < (zmax+radius))
			&& (zp > (zmin-radius)) )
		{
			vDOMap.push_back
				(VEDO::DOMap(i, pdos, pmodel, pdos->GetVelocity().length()));
		}
	}

	NJR::RandomGenerator RA;
	int hit = 0;
	for (unsigned int i=0; i<RandomHitTestNum; ++i)
	{
		double ranX = RA(xmin, xmax);
		double ranY = RA(ymin, ymax);
        double ranZ = RA(zmin, zmax);
		NJR::NJRvector3d ranV(ranX, ranY, ranZ);

		bool hasHit = false;
		for (unsigned int j=0; j<vDOMap.size(); ++j)
		{
			if ((ranV - (vDOMap[j].cpdos()->GetPosition())).length()
				<= vDOMap[j].cpdoml()->GetShapeAttributes().sphere.radius)
			{
				hasHit = true;
				break;
			}
		}

		if(hasHit)
		{
			hit++;
		}
	}
	return hit/static_cast<float>(RandomHitTestNum)*100.0;
};
