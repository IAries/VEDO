#include <NJR/Interfaces/acadxf.h>
#include <NJR/Interfaces/dxfsolid.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <algorithm>
#include <cstdlib>
#include <cmath>

using namespace std;

void DOWorld::Draw(const char* filename)  const
{
	NJRDXF::ofstream bdxf(filename);
	vector<DOStatus *>::const_iterator idos;
	const DOModel* pdoml;
	DOShapeAttributes shA;

	NJRpolyhedra polyhedra;
	vector<NJRpolygon> faces;
	NJRDXF::Polygon	polygon;

    NJRDXF::Ellipsoid  ellipsoid(PI/4.0);
	NJRDXF::Sphere     sphere(PI/3.0);
	NJRDXF::QuasiPlate cuboid;
	NJRDXF::Cylinder   cylinder(PI/4.0);
	NJRDXF::Text       text;

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
	NJRDXF::ofstream bdxf(filename);
	vector<DOStatus *>::const_iterator idos;
	const DOModel* pdoml;
	DOShapeAttributes shA;

	NJRpolyhedra polyhedra;
	vector<NJRpolygon> faces;
	NJRDXF::Polygon polygon;

    NJRDXF::Ellipsoid ellipsoid;
	NJRDXF::Sphere sphere(PI/8);
	NJRDXF::QuasiPlate qplate;
	NJRDXF::QuasiCylinder qcylinder;
	NJRDXF::Text text;

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
