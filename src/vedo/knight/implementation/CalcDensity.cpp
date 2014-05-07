#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/njr/interfaces/RandomGenerator.h>
#include <vedo/framework/interfaces/DOMap.h>
#include <vedo/knight/interfaces/CalcDensity.h>

using namespace std;

const int CalcDensity::RandomHitTestNum = 20000;

double CalcDensity::computeDensity
	(const vedo::DOWorld* pWorld,
	double xmin,
	double xmax,
	double ymin,
	double ymax,
	double zmin,
	double zmax ) const
{
	std::vector<vedo::DOMap> vDOMap;
	double xp, yp, zp, radius;
	for (unsigned int i=0; i<(pWorld->GetSystemParameter()->GetDONumber()) ;++i)
	{
		const vedo::DOStatus* pdos = pWorld->GetDOStatus(i);
		const vedo::DOModel* pmodel = pWorld->GetDOModel(pdos->GetDOName());
		if (pmodel->GetShapeType() != vedo::Sphere)
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
				(vedo::DOMap(i, pdos, pmodel, pdos->GetVelocity().length()));
		}
	}

	njr::RandomGenerator RA;
	int hit = 0;
	for (unsigned int i=0; i<RandomHitTestNum; ++i)
	{
		double ranX = RA(xmin, xmax);
		double ranY = RA(ymin, ymax);
        double ranZ = RA(zmin, zmax);
		njr::Vector3d ranV(ranX, ranY, ranZ);

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