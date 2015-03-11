#include <vedo/knight/interfaces/CalcDensity.h>
#include <vedo/framework/interfaces/DOMap.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/njr/interfaces/RandomGenerator.h>
#include <vedo/constants/interfaces/Constants.h>

using namespace std;

const vedo::vedo_int_t CalcDensity::RandomHitTestNum = 20000;

vedo::vedo_float_t CalcDensity::computeDensity
	(const vedo::DOWorld* pWorld,
	vedo::vedo_float_t xmin,
	vedo::vedo_float_t xmax,
	vedo::vedo_float_t ymin,
	vedo::vedo_float_t ymax,
	vedo::vedo_float_t zmin,
	vedo::vedo_float_t zmax ) const
{
	std::vector<vedo::DOMap> vDOMap;
	vedo::vedo_float_t xp, yp, zp, radius;
	for (vedo::vedo_uint_t i=0; i<(pWorld->GetSystemParameter()->GetDONumber()) ;++i)
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
	vedo::vedo_int_t hit = 0;
	for (vedo::vedo_uint_t i=0; i<RandomHitTestNum; ++i)
	{
		vedo::vedo_float_t ranX = RA.GeneratingDouble(xmin, xmax);
		vedo::vedo_float_t ranY = RA.GeneratingDouble(ymin, ymax);
        vedo::vedo_float_t ranZ = RA.GeneratingDouble(zmin, zmax);
		njr::Vector3d ranV(ranX, ranY, ranZ);

		bool hasHit = false;
		for (vedo::vedo_uint_t j=0; j<vDOMap.size(); ++j)
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
