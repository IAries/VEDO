#ifndef _ARIES_MATRIX_FLOAT_H
#define _ARIES_MATRIX_FLOAT_H

#include <aries/utility/DataType.h>
#include <aries/utility/Matrix.h>
#include <aries/utility/XSolver.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>

namespace aries
{

class Matrixf: public Matrix<_float_t>
{

public:

	Matrixf(const _uint_t& rows, const _uint_t& columns);

	Matrixf(const _uint_t& rows, const _uint_t& columns, const _float_t& fvalue);

	Matrixf(const _uint_t& rows, const _uint_t& columns, const _float_t* fpvalue);

	Matrixf(const Matrixf& m);

	Matrixf(const Matrix<_float_t>& m);

	const Matrixf& operator = (const Matrixf& m);

	const Matrixf& operator = (const Matrix<_float_t>& m);

	~Matrixf();

	const Matrixf& operator = (const _float_t& fvalue);

	void resize(const _uint_t& rows, const _uint_t& columns);

	void Resize(const _uint_t& rows, const _uint_t& columns);

	inline const XSolver* GetXSolver() const
	{
		return _XSolver;
	}

	inline void SetXSolver(const XSolver* xs)
	{
		_XSolver = xs;
	}

	// {Y} = [A] {X}, give [A] & {Y}, solve {X}
	Matrixf SolveX(const Matrixf& mY) const;

protected:

	Matrixf();

	const XSolver* _XSolver;
};

}   // namespace aries

#endif // _ARIES_MATRIX_FLOAT_H
