#ifndef _NJR_POLYHEDRA_H
#define _NJR_POLYHEDRA_H

#include <vedo/njr/interfaces/HalfSpace.h>
#include <vedo/njr/interfaces/Polygon.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/constants/interfaces/Constants.h>
#include <vector>

namespace njr
{

class NJRpolyhedra
{

public:

	NJRpolyhedra();

	NJRpolyhedra(const NJRpolyhedra &a);

	inline const Vector3d& center() const
	{
		return _center;
	}

	inline const std::vector<njr::HalfSpace>& constrains() const
	{
		return _constrains;
	}

	NJRpolyhedra& operator = (const NJRpolyhedra &poly3d);

	NJRpolyhedra operator + (const NJRpolyhedra &poly3d) const;

	void SetCubic(Vector3d center, vedo::vedo_float_t length);

	void SetIcosahedron(Vector3d center, vedo::vedo_float_t radius);

	void SetRandom(Vector3d center, vedo::vedo_uint_t n, vedo::vedo_float_t radius);

	void SetPlane(vedo::vedo_float_t eqa, vedo::vedo_float_t eqb, vedo::vedo_float_t eqc, Sense sense, vedo::vedo_float_t eqd);

	void SetRetangular(Vector3d center, vedo::vedo_float_t lx, vedo::vedo_float_t ly, vedo::vedo_float_t lz);

	inline void SetCenter(const Vector3d& c)
	{
		_center = c;
	}

	void Normalize();

	void RotateAround(const Vector3d&);

	void Translate(const Vector3d&);

	NJRpolyhedra Mapping(const Vector3d& center, const Vector3d& Ox, const Vector3d& Oz) const;

	NJRpolyhedra CoverPolyhedra () const;

	std::vector<NJRpolygon> faces() const;

	void AddConstrain(const njr::HalfSpace&);

	void Clear();

	void Purge();

	bool Check();

	void print() const;

private:

	std::vector<njr::HalfSpace> _constrains;

	Vector3d _center;

};

}   // namespace njr

#endif // _NJR_POLYHEDRA_H
