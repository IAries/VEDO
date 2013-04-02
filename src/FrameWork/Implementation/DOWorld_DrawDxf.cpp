#include <NJR/Interfaces/ACADXF.h>
#include <NJR/Interfaces/ACADXF_Solid.h>
#include <Framework/Interfaces/Constants.h>
#include <Framework/Interfaces/DOWorld.h>
#include <algorithm>
#include <cstdlib>
#include <cmath>

namespace vedo
{

void DOWorld::Draw(const char* filename)  const
{
	njrdxf::ofstream bdxf(filename);
	std::vector<DOStatus *>::const_iterator idos;
	const DOModel* pdoml;
	DOShapeAttributes shA;

	njr::NJRpolyhedra polyhedra;
	std::vector<njr::NJRpolygon> faces;
	njrdxf::Polygon	polygon;

    njrdxf::Ellipsoid  ellipsoid(njr::dQuarterPI);
	njrdxf::Sphere     sphere(njr::dOneThirdPI);
	njrdxf::QuasiPlate cuboid;
	njrdxf::Cylinder   cylinder(njr::dQuarterPI);
	njrdxf::Text       text;

	for (idos = cDOStatus.begin(); idos != cDOStatus.end(); ++idos)
	{
		pdoml = DOWorld::GetDOModel((*idos)->GetDOName());
		shA   = pdoml->GetShapeAttributes();

		switch (pdoml->GetShapeType())
		{
			case Ellipsoid:
				ellipsoid.Set
				 	(shA.ellipsoid.xlength,
					shA.ellipsoid.ylength,
					shA.ellipsoid.zlength,
					(*idos)->GetPosition(),
					(*idos)->GetOrientationX(),
					(*idos)->GetOrientationZ(),
					pdoml->GetDOGroup().c_str(),
					pdoml->GetShapeColor());
				bdxf << &ellipsoid;
				break;
			case Sphere:
				sphere.Set
					(shA.sphere.radius,
					(*idos)->GetPosition(),
					(*idos)->GetOrientationX(),
					(*idos)->GetOrientationZ(),
					pdoml->GetDOGroup().c_str(),
					pdoml->GetShapeColor());
					bdxf << &sphere;
				break;
			case QuasiPlate:
				cuboid.Set
					(shA.quasiplate.width,
					shA.quasiplate.length,
					shA.quasiplate.height,
					(*idos)->GetPosition(),
					(*idos)->GetOrientationX(),
					(*idos)->GetOrientationZ(),
					pdoml->GetDOGroup().c_str(),
					pdoml->GetShapeColor());
				bdxf << &cuboid;
				break;
			case QuasiCylinder:
				cylinder.Set
					(shA.quasicylinder.radius,
					shA.quasicylinder.height,
					(*idos)->GetPosition(),
					(*idos)->GetOrientationX(),
					(*idos)->GetOrientationZ(),
					pdoml->GetDOGroup().c_str(),
					pdoml->GetShapeColor());
				bdxf << &cylinder;
				break;
			case Polyhedra:
				polyhedra = pdoml->GetPolyhedra();
				faces = polyhedra.faces();
				for (unsigned int i=0; i<faces.size(); ++i)
				{
					polygon.Set
						(faces[i],
						(*idos)->GetPosition(),
						(*idos)->GetOrientationX(),
						(*idos)->GetOrientationZ(),
						pdoml->GetDOGroup().c_str(),
						pdoml->GetShapeColor()      );
					bdxf << &polygon;
				}
				break;
			default:
				text.Set((*idos)->GetPosition(),"Unknown 3dSolid Model");
				bdxf << &text;
		}
	};
};

void DOWorld::HighDraw(const char* filename) const
{
	njrdxf::ofstream bdxf(filename);
	std::vector<DOStatus *>::const_iterator idos;
	const DOModel* pdoml;
	DOShapeAttributes shA;

	njr::NJRpolyhedra polyhedra;
	std::vector<njr::NJRpolygon> faces;
	njrdxf::Polygon polygon;

    njrdxf::Ellipsoid ellipsoid;
	njrdxf::Sphere sphere(njr::dOneEighthPI);
	njrdxf::QuasiPlate qplate;
	njrdxf::QuasiCylinder qcylinder;
	njrdxf::Text text;

	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		pdoml = DOWorld::GetDOModel((*idos)->GetDOName());
		shA = pdoml->GetShapeAttributes();

		switch (pdoml->GetShapeType())
		{
			case Ellipsoid:
				 ellipsoid.Set
				 	(shA.ellipsoid.xlength,
					shA.ellipsoid.ylength,
					shA.ellipsoid.zlength,
					(*idos)->GetPosition(),
					(*idos)->GetOrientationX(),
					(*idos)->GetOrientationZ(),
					pdoml->GetDOGroup().c_str(),
					pdoml->GetShapeColor());
				bdxf << &ellipsoid;
				break;
			case Sphere:
				sphere.Set
					(shA.sphere.radius,
					(*idos)->GetPosition(),
					(*idos)->GetOrientationX(),
					(*idos)->GetOrientationZ(),
					pdoml->GetDOGroup().c_str(),
					pdoml->GetShapeColor());
				bdxf << &sphere;
				break;
			case QuasiPlate:
				qplate.Set
					(shA.quasiplate.width,
					shA.quasiplate.length,
					shA.quasiplate.height,
					(*idos)->GetPosition(),
					(*idos)->GetOrientationX(),
					(*idos)->GetOrientationZ(),
					pdoml->GetDOGroup().c_str(),
					pdoml->GetShapeColor());
				bdxf << &qplate;
				break;

			case QuasiCylinder:
				qcylinder.Set
					(shA.quasicylinder.radius,
					shA.quasicylinder.height,
					(*idos)->GetPosition(),
					(*idos)->GetOrientationX(),
					(*idos)->GetOrientationZ(),
					pdoml->GetDOGroup().c_str(),
					pdoml->GetShapeColor());
					bdxf << &qcylinder;
				break;
			case Polyhedra:
				polyhedra = pdoml->GetPolyhedra();
				faces = polyhedra.faces();
				for (unsigned int i=0; i<faces.size(); ++i)
				{
					polygon.Set
						(faces[i],
						(*idos)->GetPosition(),
						(*idos)->GetOrientationX(),
						(*idos)->GetOrientationZ(),
						pdoml->GetDOGroup().c_str(),
						pdoml->GetShapeColor());
					bdxf << &polygon;
				}
				break;
			default:
				text.Set((*idos)->GetPosition(),"Unknown 3dSolid Model");
				bdxf << &text;
		}
	};
};

};   // namespace vedo
