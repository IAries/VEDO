#ifndef _NJR_POLYHEDRA_H
#define _NJR_POLYHEDRA_H

#include <NJR/Interfaces/HalfSpace.h>
#include <NJR/Interfaces/Polygon.h>
#include <NJR/Interfaces/Vector3d.h>
#include <vector>

namespace NJR
{

class NJRpolyhedra
{

public:

	NJRpolyhedra();

	NJRpolyhedra(const NJRpolyhedra &a);

	inline const Vector3d& center() const
	{
		return _center;
	};

	inline const std::vector<NJR::HalfSpace>& constrains() const
	{
		return _constrains;
	};

	NJRpolyhedra& operator = (const NJRpolyhedra &poly3d);

	NJRpolyhedra operator + (const NJRpolyhedra &poly3d) const;

	void SetCubic(Vector3d center, double length);

	void SetIcosahedron(Vector3d center, double radius);

	void SetRandom(Vector3d center, unsigned int n, double radius);

	void SetPlane(double eqa, double eqb, double eqc, Sense sense, double eqd);

	void SetRetangular(Vector3d center, double lx, double ly, double lz);

	inline void SetCenter(const Vector3d& c)
	{
		_center = c;
	};

	void Normalize();

	void RotateAround(const Vector3d&);

	void Translate(const Vector3d&);

	NJRpolyhedra Mapping
		(const Vector3d& center,
		const Vector3d& Ox,
		const Vector3d& Oz) const;

	NJRpolyhedra CoverPolyhedra () const;

	std::vector<NJRpolygon> faces() const;

	void AddConstrain(const NJR::HalfSpace&);

	void Clear();

	void Purge();

	bool Check();

	void print() const ;

private:

	std::vector<NJR::HalfSpace> _constrains;

	Vector3d _center;

};

};   // namespace NJR

#endif // _NJR_POLYHEDRA_H
