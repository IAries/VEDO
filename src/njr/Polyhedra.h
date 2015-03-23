#ifndef _NJR_POLYHEDRA_H
#define _NJR_POLYHEDRA_H

#include <njr/HalfSpace.h>
#include <njr/Polygon.h>
#include <aries/utility/Vector3df.h>
#include <vector>

namespace njr
{

class NJRpolyhedra
{

public:

	NJRpolyhedra();

	NJRpolyhedra(const NJRpolyhedra &a);

	inline const aries::Vector3df& center() const
	{
		return _center;
	}

	inline const std::vector<njr::HalfSpace>& constrains() const
	{
		return _constrains;
	}

	NJRpolyhedra& operator = (const NJRpolyhedra &poly3d);

	NJRpolyhedra operator + (const NJRpolyhedra &poly3d) const;

	void SetCubic(aries::Vector3df center, _float_t length);

	void SetIcosahedron(aries::Vector3df center, _float_t radius);

	void SetRandom(aries::Vector3df center, _uint_t n, _float_t radius);

	void SetPlane(_float_t eqa, _float_t eqb, _float_t eqc, Sense sense, _float_t eqd);

	void SetRetangular(aries::Vector3df center, _float_t lx, _float_t ly, _float_t lz);

	inline void SetCenter(const aries::Vector3df& c)
	{
		_center = c;
	}

	void Normalize();

	void RotateAround(const aries::Vector3df&);

	void Translate(const aries::Vector3df&);

	NJRpolyhedra Mapping(const aries::Vector3df& center, const aries::Vector3df& Ox, const aries::Vector3df& Oz) const;

	NJRpolyhedra CoverPolyhedra () const;

	std::vector<NJRpolygon> faces() const;

	void AddConstrain(const njr::HalfSpace&);

	void Clear();

	void Purge();

	bool Check();

	void print() const;

private:

	std::vector<njr::HalfSpace> _constrains;

	aries::Vector3df _center;

};

}   // namespace njr

#endif // _NJR_POLYHEDRA_H
