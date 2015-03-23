#include <aries/utility/Constants.h>
#include <njr/ACADXF.h>
#include <njr/ACADXF_Solid.h>
#include <vedo/framework/DOWorld.h>
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

    njrdxf::Ellipsoid                  ellipsoid(aries::fQuarterPI);
	njrdxf::Sphere                     sphere(aries::fOneThirdPI);
	njrdxf::QuasiPlate                 cuboid;
	njrdxf::QuasiPlateWithCircularHole cuboid2;
	njrdxf::Cylinder                   cylinder(aries::fQuarterPI);
	njrdxf::Text                       text;

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
					 pdoml->GetShapeColor()      );
				bdxf << &ellipsoid;
				break;
			case Sphere:
				sphere.Set
					(shA.sphere.radius,
					 (*idos)->GetPosition(),
					 (*idos)->GetOrientationX(),
					 (*idos)->GetOrientationZ(),
					 pdoml->GetDOGroup().c_str(),
					 pdoml->GetShapeColor()      );
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
					 pdoml->GetShapeColor()      );
				bdxf << &cuboid;
				break;
			case QuasiPlateWithCircularHole:
			    // Aries: Need to be modified. It was copied from the QuasiPlate but not consider about the hole
				cuboid2.Set
					(shA.quasiplatewithcircularhole.width,
					 shA.quasiplatewithcircularhole.length,
					 shA.quasiplatewithcircularhole.height,
					 (*idos)->GetPosition(),
					 (*idos)->GetOrientationX(),
					 (*idos)->GetOrientationZ(),
					 pdoml->GetDOGroup().c_str(),
					 pdoml->GetShapeColor()                );
				bdxf << &cuboid2;
				break;
			case QuasiCylinder:
				cylinder.Set
					(shA.quasicylinder.radius,
					 shA.quasicylinder.height,
					 (*idos)->GetPosition(),
					 (*idos)->GetOrientationX(),
					 (*idos)->GetOrientationZ(),
					 pdoml->GetDOGroup().c_str(),
					 pdoml->GetShapeColor()      );
				bdxf << &cylinder;
				break;
			case Polyhedra:
				polyhedra = pdoml->GetPolyhedra();
				faces = polyhedra.faces();
				for (_uint_t i=0; i<faces.size(); ++i)
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
	}
}

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
	njrdxf::Sphere sphere(aries::fOneEighthPI);
	njrdxf::QuasiPlate qplate;
	njrdxf::QuasiPlateWithCircularHole qplate2;
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
					 pdoml->GetShapeColor()      );
				bdxf << &ellipsoid;
				break;
			case Sphere:
				sphere.Set
					(shA.sphere.radius,
					 (*idos)->GetPosition(),
					 (*idos)->GetOrientationX(),
					 (*idos)->GetOrientationZ(),
					 pdoml->GetDOGroup().c_str(),
					 pdoml->GetShapeColor()      );
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
					 pdoml->GetShapeColor()      );
				bdxf << &qplate;
				break;

			case QuasiPlateWithCircularHole:
				qplate2.Set
					(shA.quasiplatewithcircularhole.width,
					 shA.quasiplatewithcircularhole.length,
					 shA.quasiplatewithcircularhole.height,
					 (*idos)->GetPosition(),
					 (*idos)->GetOrientationX(),
					 (*idos)->GetOrientationZ(),
					 pdoml->GetDOGroup().c_str(),
					 pdoml->GetShapeColor()                );
				bdxf << &qplate2;
				break;

			case QuasiCylinder:
				qcylinder.Set
					(shA.quasicylinder.radius,
					 shA.quasicylinder.height,
					 (*idos)->GetPosition(),
					 (*idos)->GetOrientationX(),
					 (*idos)->GetOrientationZ(),
					 pdoml->GetDOGroup().c_str(),
					 pdoml->GetShapeColor()      );
				bdxf << &qcylinder;
				break;
			case Polyhedra:
				polyhedra = pdoml->GetPolyhedra();
				faces = polyhedra.faces();
				for (_uint_t i=0; i<faces.size(); ++i)
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
	}
}

}   // namespace vedo
