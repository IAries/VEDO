#ifndef _ARIES_MATRIX_FORTRAN_STYLE_H
#define _ARIES_MATRIX_FORTRAN_STYLE_H

#include <aries/utility/DataType.h>
#include <aries/utility/XSolver.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>

namespace aries
{

class MatrixFs
{

public:

	MatrixFs(const _uint_t& rows, const _uint_t& columns);

	MatrixFs(const _uint_t& rows, const _uint_t& columns, const _float_t& value);

	MatrixFs(const _uint_t& rows, const _uint_t& columns, const _float_t* tpvalue);

	MatrixFs(const MatrixFs& m);

	const MatrixFs& operator = (const MatrixFs& m);

	~MatrixFs();

	void resize(const _uint_t& rows, const _uint_t& columns, const _float_t& value);

	void Resize(const _uint_t& rows, const _uint_t& columns, const _float_t& value);

	void resize(const _uint_t& rows, const _uint_t& columns);

	void Resize(const _uint_t& rows, const _uint_t& columns);

	inline _uint_t rows() const
	{
		return _rows;
	}

	inline _uint_t columns() const
	{
		return _columns;
	}

	inline const _float_t* Reference() const
	{
		return _matval;
	}

	const MatrixFs& operator = (const _float_t& value);

	MatrixFs operator + (const MatrixFs& m) const;

	MatrixFs operator + (const _float_t& value) const;

	const MatrixFs& operator += (const _float_t& value);

	MatrixFs operator - (const MatrixFs& m) const;

	MatrixFs operator - (const _float_t& value) const;

	const MatrixFs& operator -= (const _float_t& value);

	MatrixFs operator * (const MatrixFs& M) const;

	MatrixFs operator * (const _float_t& value) const;

	const MatrixFs& operator *= (const _float_t& value);

	MatrixFs operator / (const _float_t& value) const;

	const MatrixFs& operator /= (const _float_t& value);

	const std::string characteristic() const;

	MatrixFs transpose() const;

	MatrixFs Transpose() const;

	_float_t determine() const;

	_uint_t rank() const;

	MatrixFs adjoint() const;

	MatrixFs Adjoint() const;

	MatrixFs SubMatrix(const _uint_t& row, const _uint_t& column) const;

	MatrixFs inverse() const;

	MatrixFs Inverse() const;

	_float_t& operator () (const _uint_t& r, const _uint_t& c);

	_float_t get(const _uint_t& r, const _uint_t& c) const;

	inline const XSolver* GetXSolver() const
	{
		return _XSolver;
	}

	inline void SetXSolver(const XSolver* xs)
	{
		_XSolver = xs;
	}

	// {Y} = [A] {X}, give [A] & {Y}, solve {X}
	MatrixFs SolveX(const MatrixFs& mY) const;

protected:

	_uint_t    _rows;

	_uint_t    _columns;

	_float_t** _matrix;

	_float_t*  _matval;

	MatrixFs();

	const XSolver* _XSolver;

	void print() const;
};

}   // namespace aries



aries::MatrixFs operator - (const aries::MatrixFs& m);

aries::MatrixFs operator * (const aries::_float_t& s, const aries::MatrixFs& m);

std::ostream& operator << (std::ostream& os, const aries::MatrixFs& m);



#endif   // _ARIES_MATRIX_FORTRAN_STYLE_H
