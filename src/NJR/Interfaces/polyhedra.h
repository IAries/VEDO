#ifndef _NJR_POLYHEDRA_H
#define _NJR_POLYHEDRA_H

#include <NJR/Interfaces/halfspace.h>
#include <NJR/Interfaces/polygon.h>
#include <NJR/Interfaces/vector3d.h>
#include <vector>

class NJRpolyhedra
{

public:

	NJRpolyhedra();

	NJRpolyhedra(const NJRpolyhedra &a);

	inline const NJRvector3d& center() const
	{
		return _center;
	};

	inline const std::vector<NJRhalfspace>& constrains() const
	{
		return _constrains;
	};

	NJRpolyhedra& operator = (const NJRpolyhedra &poly3d);

	NJRpolyhedra operator + (const NJRpolyhedra &poly3d) const;

	void SetCubic(NJRvector3d center, double length);

	void SetIcosahedron(NJRvector3d center, double radius);

	void SetRandom(NJRvector3d center, unsigned int n, double radius);

	void SetPlane(double eqa, double eqb, double eqc, Sense sense, double eqd);

	void SetRetangular(NJRvector3d center, double lx, double ly, double lz);

	inline void SetCenter(const NJRvector3d& c)
	{
		_center = c;
	};

	void Normalize();

	void RotateAround(const NJRvector3d&);

	void Translate(const NJRvector3d&);

	NJRpolyhedra Mapping
		(const NJRvector3d& center,
		const NJRvector3d& Ox,
		const NJRvector3d& Oz) const;

	NJRpolyhedra CoverPolyhedra () const;

	std::vector<NJRpolygon> faces() const;

	void AddConstrain(const NJRhalfspace&);

	void Clear();

	void Purge();

	bool Check();

	void print() const ;

private:

	std::vector<NJRhalfspace> _constrains;

	NJRvector3d _center;

};

#endif // _NJR_POLYHEDRA_H
