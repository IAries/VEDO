#include <NJR/Interfaces/random.h>
#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/DOWorld_WriteVTK.h>
#include <FrameWork/Interfaces/DOMap.h>
#include <Knight/Interfaces/CalcCoordinateNum.h>

double CalcCoordinateNum::computeAvgCoordinateNum
	(const VEDO::DOWorld* pWorld,
	double xmin,
	double xmax,
	double ymin,
	double ymax,
	double zmin,
	double zmax ) const
{
	/**************************************************************************
	 * Intent: to contain all particles whose center lies in the range
	 * (xmin-r, xmax+r, ymin-r, ymax+r, zmin-r, zmax+r), where r denote
	 * particle radius
	 **************************************************************************/
	std::vector<VEDO::DOMap> vDOMap;
	std::vector<VEDO::DOMap> wholeMap = VEDO::DOMap::GetDOMap(pWorld);

	double xp, yp, zp, radius;

	for (unsigned int i=0; i<(pWorld->GetSystemParameter()->GetDONumber()); ++i)
	{
		const VEDO::DOStatus* pdos = pWorld->GetDOStatus(i);
		const VEDO::DOModel* pmodel = pWorld->GetDOModel(pdos->GetDOName());
		if (pmodel->GetShapeType() != VEDO::Sphere)
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
				(VEDO::DOMap(i, pdos, pmodel, pdos->GetVelocity().length()));
		}
	}

	long total = 0;
	int count = 0;
	for (unsigned int i=0; i<vDOMap.size(); ++i)
	{
		count=0;
		for (unsigned int j=0; j<wholeMap.size(); ++j)
		{
			VEDO::DOMap m1 = vDOMap[i];
			VEDO::DOMap m2 = wholeMap[j];

			if (m1.cpdos() == m2.cpdos())
			{
				continue;
			}
			if (m2.cpdoml()->GetShapeType() != VEDO::Sphere)
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
	return total/static_cast<double>(vDOMap.size());
}

void CalcCoordinateNum::outputCoordinateNumVTK
	(const VEDO::DOWorld* pWorld, const std::string& filename) const
{
	std::vector<VEDO::DOMap> wholeMap = VEDO::DOMap::GetDOMap(pWorld);
	std::vector<double> coordNumArr;

	for (unsigned int i=0; i<wholeMap.size(); ++i)
	{
		VEDO::DOMap m1 = wholeMap[i];
		if (m1.cpdoml()->GetShapeType() != VEDO::Sphere)
			continue;
		coordNumArr.push_back(0);
	}

	for (unsigned int i=0; i<wholeMap.size(); ++i)
	{
		VEDO::DOMap m1 = wholeMap[i];
		if (m1.cpdoml()->GetShapeType() != VEDO::Sphere)
			continue;

		for (unsigned int j=i+1; j<wholeMap.size(); ++j)
		{
			VEDO::DOMap m2 = wholeMap[j];

			if (m1.cpdos() == m2.cpdos())
				continue;

			if (m2.cpdoml()->GetShapeType() != VEDO::Sphere)
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
	VEDO::DataFieldVTKWriter* coordVTKField = VEDO::DataFieldVTKWriter::Instance();
	coordVTKField->clearAll();
	coordVTKField->addArray("CoordinateNumber", 1, coordNumArr);
//	pWorld->WriteVTK<DataFieldVTKWriter>(filename.c_str());
}
