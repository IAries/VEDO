#ifndef _NJR_LINEAR_PROGRAMMING_H
#define _NJR_LINEAR_PROGRAMMING_H

#include <vedo/njr/interfaces/Matrix.h>
#include <vedo/njr/interfaces/Polyhedra.h>
#include <vedo/njr/interfaces/Polygon.h>
#include <vedo/constants/interfaces/Constants.h>

namespace njr
{

class LinearProgramming
{

public:

	LinearProgramming();

	LinearProgramming(const LinearProgramming&);

	virtual ~LinearProgramming();

	const LinearProgramming& operator = (const LinearProgramming&);

	void Set(const njr::NJRpolyhedra &);

	bool Check();

	void Clear();

	njr::NJRpolygon GetPolygon();

	bool GetExtremeValue
		(vedo::vedo_float_t& maxX, vedo::vedo_float_t& minX, vedo::vedo_float_t& maxY,
		 vedo::vedo_float_t& minY, vedo::vedo_float_t& maxZ, vedo::vedo_float_t& minZ);

	inline bool ISPolygon() const
	{
		return _polygon;
	}

	inline bool checked() const
	{
		return _checked;
	}

	inline bool feasiable() const
	{
		return _feasiable;
	}

	void print() const;

private:

	vedo::vedo_uint_t cols;

	vedo::vedo_uint_t rows;

	vedo::vedo_uint_t num_m;

	vedo::vedo_uint_t num_s;

	vedo::vedo_uint_t nbas;

	bool _checked;

	bool _feasiable;

	bool _polygon;

	char *kid;

	vedo::vedo_uint_t *basis;

	vedo::vedo_uint_t *nbasis;

	Matrix obj;

	Matrix A;

	Matrix rhs;

	Matrix BI;

    void Allocation();

	vedo::vedo_float_t GetObjValue();

	bool PurgeArtificialVariable();

};

}   // namespace njr

#endif // _NJR_LINEAR_PROGRAMMING_H
