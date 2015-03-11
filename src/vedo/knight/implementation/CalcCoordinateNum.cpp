#include <vedo/knight/interfaces/CalcCoordinateNum.h>
#include <vedo/framework/interfaces/DOWorld_WriteVTK.h>
#include <vedo/framework/interfaces/DOMap.h>
#include <vedo/njr/interfaces/RandomGenerator.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/constants/interfaces/Constants.h>

vedo::vedo_float_t CalcCoordinateNum::computeAvgCoordinateNum
	(const vedo::DOWorld* pWorld,
	vedo::vedo_float_t xmin,
	vedo::vedo_float_t xmax,
	vedo::vedo_float_t ymin,
	vedo::vedo_float_t ymax,
	vedo::vedo_float_t zmin,
	vedo::vedo_float_t zmax ) const
{
	/**************************************************************************
	 * Intent: to contain all particles whose center lies in the range
	 * (xmin-r, xmax+r, ymin-r, ymax+r, zmin-r, zmax+r), where r denote
	 * particle radius
	 **************************************************************************/
	std::vector<vedo::DOMap> vDOMap;
	std::vector<vedo::DOMap> wholeMap = vedo::DOMap::GetDOMap(pWorld);

	vedo::vedo_float_t xp, yp, zp, radius;

	for (vedo::vedo_uint_t i=0; i<(pWorld->GetSystemParameter()->GetDONumber()); ++i)
	{
		const vedo::DOStatus* pdos = pWorld->GetDOStatus(i);
		const vedo::DOModel* pmodel = pWorld->GetDOModel(pdos->GetDOName());
		if (pmodel->GetShapeType() != vedo::Sphere)
		{
			continue;
		}
		xp=pdos->GetPosition().x();
		yp=pdos->GetPosition().y();
		zp=pdos->GetPosition().z();
		radius = pmodel->GetShapeAttributes().sphere.radius;
		if (   (xp < (xmax + radius))
			&& (xp > (xmin - radius))
			&& (yp < (ymax + radius))
			&& (yp > (ymin - radius))
			&& (zp < (zmax + radius))
			&& (zp > (zmin - radius)) )
		{
			vDOMap.push_back
				(vedo::DOMap(i, pdos, pmodel, pdos->GetVelocity().length()));
		}
	}

	vedo::vedo_int_t total = 0;
	vedo::vedo_int_t count = 0;
	for (vedo::vedo_uint_t i=0; i<vDOMap.size(); ++i)
	{
		count=0;
		for (vedo::vedo_uint_t j=0; j<wholeMap.size(); ++j)
		{
			vedo::DOMap m1 = vDOMap[i];
			vedo::DOMap m2 = wholeMap[j];

			if (m1.cpdos() == m2.cpdos())
			{
				continue;
			}
			if (m2.cpdoml()->GetShapeType() != vedo::Sphere)
			{
				continue;
			}
			if (((m1.cpdos()->GetPosition()-m2.cpdos()->GetPosition()).length())
				<= (m1.cpdoml()->GetShapeAttributes().sphere.radius
					+ m2.cpdoml()->GetShapeAttributes().sphere.radius))
			{
				count++;
			}
		}
		total += count;
	}
	return total/static_cast<vedo::vedo_float_t>(vDOMap.size());
}

void CalcCoordinateNum::outputCoordinateNumVTK
	(const vedo::DOWorld* pWorld, const std::string& filename) const
{
	std::vector<vedo::DOMap> wholeMap = vedo::DOMap::GetDOMap(pWorld);
	std::vector<vedo::vedo_float_t> coordNumArr;

	for (vedo::vedo_uint_t i=0; i<wholeMap.size(); ++i)
	{
		vedo::DOMap m1 = wholeMap[i];
		if (m1.cpdoml()->GetShapeType() != vedo::Sphere)
			continue;
		coordNumArr.push_back(0);
	}

	for (vedo::vedo_uint_t i=0; i<wholeMap.size(); ++i)
	{
		vedo::DOMap m1 = wholeMap[i];
		if (m1.cpdoml()->GetShapeType() != vedo::Sphere)
			continue;

		for (vedo::vedo_uint_t j=i+1; j<wholeMap.size(); ++j)
		{
			vedo::DOMap m2 = wholeMap[j];

			if (m1.cpdos() == m2.cpdos())
				continue;

			if (m2.cpdoml()->GetShapeType() != vedo::Sphere)
				continue;

			if (((m1.cpdos()->GetPosition() - m2.cpdos()->GetPosition()).length())
				< (m1.cpdoml()->GetShapeAttributes().sphere.radius
					+ m2.cpdoml()->GetShapeAttributes().sphere.radius))
			{
				coordNumArr[i]++;
				coordNumArr[j]++;
			}
		}
	}
	vedo::DataFieldVTKWriter* coordVTKField = vedo::DataFieldVTKWriter::Instance();
	coordVTKField->clearAll();
	coordVTKField->addArray("CoordinateNumber", 1, coordNumArr);
    #ifdef _STD_CPP_11
        pWorld->WriteVTK<vedo::DataFieldVTKWriter>(filename.c_str());
    #endif   // _STD_CPP_11
}
