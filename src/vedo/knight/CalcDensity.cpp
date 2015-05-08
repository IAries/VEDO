#include <vedo/knight/CalcDensity.h>
#include <vedo/framework/DOMap.h>
#include <aries/utility/Vector3d.h>
#include <njr/RandomGenerator.h>
#include <aries/utility/Constants.h>

using namespace std;

const vedo::_uint_t CalcDensity::RandomHitTestNum = 20000;

vedo::_float_t CalcDensity::computeDensity
	(const vedo::DOWorld* pWorld,
	vedo::_float_t xmin,
	vedo::_float_t xmax,
	vedo::_float_t ymin,
	vedo::_float_t ymax,
	vedo::_float_t zmin,
	vedo::_float_t zmax ) const
{
	std::vector<vedo::DOMap> vDOMap;
	vedo::_float_t xp, yp, zp, radius;
	for (vedo::_uint_t i=0; i<(pWorld->GetSystemParameter()->GetDONumber()) ;++i)
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
	vedo::_uint_t hit = 0;
	for (vedo::_uint_t i=0; i<RandomHitTestNum; ++i)
	{
		vedo::_float_t ranX = RA.GeneratingDouble(xmin, xmax);
		vedo::_float_t ranY = RA.GeneratingDouble(ymin, ymax);
        vedo::_float_t ranZ = RA.GeneratingDouble(zmin, zmax);
		vedo::Vector3df ranV(ranX, ranY, ranZ);

		bool hasHit = false;
		for (vedo::_uint_t j=0; j<vDOMap.size(); ++j)
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
