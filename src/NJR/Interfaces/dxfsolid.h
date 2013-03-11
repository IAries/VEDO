#ifndef DXF3DSOLID_H
#define DXF3DSOLID_H

#include <NJR/Interfaces/acadxf.h>
#include <NJR/Interfaces/polygon.h>
#include <list>
#include <string>

#define PI 3.14159265358979323846264338327950288

namespace NJRDXF
{
	class Solid;
	class Cuboid;
	class Cylinder;
	class Ellipsoid;
	class QuasiCylinder;
	class QuasiPlate;
	class Sphere;
	class Polygon;
};

// NJRDXF::Solid is base class of 3d Soilds
class NJRDXF::Solid
{

public:

 	Solid();

	virtual ~Solid();

	// Retrieves the Container of DXFGroups
	std::list<NJRDXF::Face *>& Get3dFaceContainer();

protected:

	// (Container of DXFGroup) is used to hold all DXFGroups of this solid
	std::list<NJRDXF::Face *> lcon3dFace;

	// (Face per angle) of this solid
	double fpa;

private:

	// (These member functions) are not allowable to access
	Solid(const Solid&);

	Solid& operator = (const Solid&);

};

// Cuboid is used to hold and set those faces of a Cuboid
class NJRDXF::Cuboid : public NJRDXF::Solid
{

public:

	Cuboid();

	void Set
		(const double& dWidth,
		const double& dLength,
		const double& dHeight,
		const NJRvector3d& vP,
		const NJRvector3d& vOX,
		const NJRvector3d& vOZ,
		const char* layer,
		const NJRDXF::Color& color);

private:

	Cuboid(const Cuboid&);

	Cuboid& operator = (const Cuboid&);

};

// Ellipsoid is used to hold and set those faces of a Ellipsoid

class NJRDXF::Ellipsoid : public NJRDXF::Solid
{

public:

	Ellipsoid(const double& fpa = PI/12.0);

	void Set
		(const double& dlx,
		const double& dly,
		const double& dlz,
		const NJRvector3d& vP,
		const NJRvector3d& vOX,
		const NJRvector3d& vOZ,
		const char* layer,
		const NJRDXF::Color& color);

private:

	Ellipsoid(const Ellipsoid&);

	Ellipsoid& operator = (const Ellipsoid&);

};

// Sphere is used to hold and set those faces of a sphere
class NJRDXF::Sphere : public NJRDXF::Ellipsoid
{

public:

	Sphere(const double& fpa = PI/12.0);

	void Set
		(const double& dRadius,
		const NJRvector3d& vP,
		const NJRvector3d& vOX,
		const NJRvector3d& vOZ,
		const char* layer,
		const NJRDXF::Color& color);

private:

	Sphere(const Sphere&);

	Sphere& operator = (const Sphere&);

};

// Cylinder is used to hold and set those faces of a Cylinder
class NJRDXF::Cylinder : public NJRDXF::Solid
{

public:

	Cylinder(const double& fpa = PI/12.0);

	void Set
		(const double& dRadius,
		const double& dHeight,
		const NJRvector3d& vP,
		const NJRvector3d& vOX,
		const NJRvector3d& vOZ,
		const char* layer,
		const NJRDXF::Color& color);

private:

	Cylinder(const Cylinder&);

	Cylinder& operator = (const Cylinder&);

};

// QuasiCylinder is used to hold and set those faces of a QuasiCylinder
class NJRDXF::QuasiCylinder: public NJRDXF::Solid
{

public:

	QuasiCylinder(const double& fpa = PI/12.0);

	void Set
		(const double& dRadius,
		const double& dHeight,
		const NJRvector3d& vP,
		const NJRvector3d& vOX,
		const NJRvector3d& vOZ,
		const char* layer,
		const NJRDXF::Color& color);

private:

	QuasiCylinder(const NJRDXF::Cylinder&);

	QuasiCylinder& operator = (const NJRDXF::Cylinder&);

};

// QuasiPlate is used to hold and set those faces of a QuasiPlate
class NJRDXF::QuasiPlate: public NJRDXF::Solid
{

public:

	QuasiPlate(const double& fpa = PI/12.0);

	void Set(const double& dWidth,
		const double& dLength,
		const double& dHeight,
		const NJRvector3d& vP,
		const NJRvector3d& vOX,
		const NJRvector3d& vOZ,
		const char* layer,
		const NJRDXF::Color& color);

private:

	QuasiPlate(const QuasiPlate&);

	QuasiPlate& operator = (const QuasiPlate&);

};

// QuasiPlate is used to hold and set those faces of a QuasiPlate

class NJRDXF::Polygon : public NJRDXF::Solid
{

public:

	Polygon();

	void Set
		(const NJRpolygon&,
		const NJRvector3d& vP,
		const NJRvector3d& vOX,
		const NJRvector3d& vOZ,
		const char* layer,
		const NJRDXF::Color &color);

private:

	Polygon (const Polygon &);

	Polygon & operator = (const Polygon &);

};

NJRDXF::ofstream& operator << (NJRDXF::ofstream&, NJRDXF::Solid*);

#endif // DXF3DSOLID_H
