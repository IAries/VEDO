#ifndef _NJR_LINEAR_PROGRAMMING_H
#define _NJR_LINEAR_PROGRAMMING_H

#include <njr/Matrix.h>
#include <njr/Polyhedra.h>
#include <njr/Polygon.h>


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
		(_float_t& maxX, _float_t& minX, _float_t& maxY,
		 _float_t& minY, _float_t& maxZ, _float_t& minZ);

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

	_uint_t cols;

	_uint_t rows;

	_uint_t num_m;

	_uint_t num_s;

	_uint_t nbas;

	bool _checked;

	bool _feasiable;

	bool _polygon;

	char *kid;

	_uint_t *basis;

	_uint_t *nbasis;

	Matrix obj;

	Matrix A;

	Matrix rhs;

	Matrix BI;

    void Allocation();

	_float_t GetObjValue();

	bool PurgeArtificialVariable();

};

}   // namespace njr

#endif // _NJR_LINEAR_PROGRAMMING_H
