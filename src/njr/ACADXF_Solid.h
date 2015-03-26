#ifndef _NJR_ACADXF_SOLID_H
#define _NJR_ACADXF_SOLID_H

#include <aries/utility/Constants.h>
#include <njr/ACADXF.h>
#include <njr/Polygon.h>
#include <list>
#include <string>

namespace njrdxf
{

// Solid is base class of 3d Soilds
class Solid
{

public:

 	Solid();

	virtual ~Solid();

	// Retrieves the Container of DXFGroups
	std::list<Face *>& Get3dFaceContainer();

protected:

	// (Container of DXFGroup) is used to hold all DXFGroups of this solid
	std::list<Face *> lcon3dFace;

	// (Face per angle) of this solid
	_float_t fpa;

private:

	// (These member functions) are not allowable to access
	Solid(const Solid&);

	Solid& operator = (const Solid&);

};



// Cuboid is used to hold and set those faces of a Cuboid
class Cuboid : public Solid
{

public:

	Cuboid();

	void Set
		(const _float_t& dWidth, const _float_t& dLength, const _float_t& dHeight,
		 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color);

private:

	Cuboid(const Cuboid&);

	Cuboid& operator = (const Cuboid&);

};

// Ellipsoid is used to hold and set those faces of a Ellipsoid

class Ellipsoid : public Solid
{

public:

	Ellipsoid(const _float_t& fpa = aries::math::_OneTwelfthPI);

	void Set
		(const _float_t& dlx, const _float_t& dly, const _float_t& dlz,
		 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color);

private:

	Ellipsoid(const Ellipsoid&);

	Ellipsoid& operator = (const Ellipsoid&);

};



// Sphere is used to hold and set those faces of a sphere
class Sphere : public Ellipsoid
{

public:

	Sphere(const _float_t& fpa = aries::math::_OneTwelfthPI);

	void Set
		(const _float_t& dRadius,
		 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color);

private:

	Sphere(const Sphere&);

	Sphere& operator = (const Sphere&);

};



// Cylinder is used to hold and set those faces of a Cylinder
class Cylinder : public Solid
{

public:

	Cylinder(const _float_t& fpa = aries::math::_OneTwelfthPI);

	void Set
		(const _float_t& dRadius, const _float_t& dHeight,
		 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color);

private:

	Cylinder(const Cylinder&);

	Cylinder& operator = (const Cylinder&);

};



// QuasiCylinder is used to hold and set those faces of a QuasiCylinder
class QuasiCylinder: public Solid
{

public:

	QuasiCylinder(const _float_t& fpa = aries::math::_OneTwelfthPI);

	void Set
		(const _float_t& dRadius, const _float_t& dHeight,
		 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color);

private:

	QuasiCylinder(const Cylinder&);

	QuasiCylinder& operator = (const Cylinder&);

};



// QuasiPlate is used to hold and set those faces of a QuasiPlate
class QuasiPlate: public Solid
{

public:

	QuasiPlate(const _float_t& fpa = aries::math::_OneTwelfthPI);

	void Set
		(const _float_t& dWidth, const _float_t& dLength, const _float_t& dHeight,
		 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color);

private:

	QuasiPlate(const QuasiPlate&);

	QuasiPlate& operator = (const QuasiPlate&);

};



// Aries: Need to modified. It was copied from the QuasiPlate but noy consider about the circular hole
// QuasiPlateWithCircularHole is used to hold and set those faces of a QuasiPlateWithCircularHole
class QuasiPlateWithCircularHole: public Solid
{

public:

	QuasiPlateWithCircularHole(const _float_t& fpa = aries::math::_OneTwelfthPI);

	void Set
		(const _float_t& dWidth, const _float_t& dLength, const _float_t& dHeight,
		 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color& color);

private:

	QuasiPlateWithCircularHole(const QuasiPlateWithCircularHole&);

	QuasiPlateWithCircularHole& operator = (const QuasiPlateWithCircularHole&);

};



// Polygon is used to hold and set those faces of a Polygon
class Polygon : public Solid
{

public:

	Polygon();

	void Set
		(const njr::NJRpolygon&,
		 const aries::Vector3df& vP, const aries::Vector3df& vOX, const aries::Vector3df& vOZ, const char* layer, const Color &color);

private:

	Polygon (const Polygon &);

	Polygon & operator = (const Polygon &);

};

njrdxf::ofstream& operator << (njrdxf::ofstream&, Solid*);

}   // namespace njrdxf

#endif // _NJR_ACADXF_SOLID_H
