#include <aries/utility/Matrixf.h>

namespace aries
{

Matrixf::Matrixf(const _uint_t& rows, const _uint_t& columns): Matrix(rows, columns), _XSolver(0)
{
	*this = 0.0;
}

Matrixf::Matrixf(const _uint_t& rows, const _uint_t& columns, const _float_t& fvalue): Matrixf(rows, columns)
{
	*this = fvalue;
	_XSolver = 0;
}

Matrixf::Matrixf(const _uint_t& rows, const _uint_t& columns, const _float_t* fpvalue): Matrixf(rows, columns)
{
	memcpy(_matval, fpvalue, sizeof(_float_t) * _rows * _columns);
	_XSolver = 0;
}

Matrixf::Matrixf(const Matrixf& m): Matrixf()
{
	*this = m;
}

Matrixf::Matrixf(const Matrix<_float_t>& m)
{
	*this = m;
}

const Matrixf& Matrixf::operator = (const Matrixf& m)
{
	if ((_rows != m._rows) || (_columns != m._columns))
	{
		if (_matval != 0)
		{
			delete [] _matval;
		}

		if (_matrix != 0)
		{
			delete [] _matrix;
		}

		_rows    = m._rows;
		_columns = m._columns;
		_XSolver = m._XSolver;
		_matrix  = new _float_t* [_rows];
		_matval  = new _float_t  [_rows*_columns];

		for (_uint_t u=0; u<_rows; ++u)
		{
			_matrix[u] = _matval + _columns * u;
		}
	}

	memcpy (_matval, m._matval, sizeof(_float_t) * _rows * _columns);
	return *this;
}

const Matrixf& Matrixf::operator = (const Matrix<_float_t>& m)
{
	if ((_rows != m.rows()) || (_columns != m.columns()))
	{
		if (_matval != 0)
		{
			delete [] _matval;
		}

		if (_matrix != 0)
		{
			delete [] _matrix;
		}

		_rows    = m.rows();
		_columns = m.columns();
		_XSolver = 0;
		_matrix  = new _float_t* [_rows];
		_matval  = new _float_t  [_rows*_columns];

		for (_uint_t u=0; u<_rows; ++u)
		{
			_matrix[u] = _matval + _columns * u;
		}
	}

	memcpy (_matval, m.Reference(), sizeof(_float_t) * _rows * _columns);
	return *this;
}

Matrixf::~Matrixf()
{
/*
	if (_matval != 0)
	{
		delete [] _matval;
	}

	if (_matrix != 0)
	{
		delete [] _matrix;
	}
*/
/*
	if (_XSolver != 0)
	{
		delete [] _XSolver;
	}
*/
}

const Matrixf& Matrixf::operator = (const _float_t& fvalue)
{
	for (_uint_t i=0; i<_rows; ++i)
	{
		for (_uint_t j=0; j<_columns; ++j)
		{
			_matrix[i][j] = fvalue;
		}
	}
	_XSolver = 0;
	return *this;
}

void Matrixf::resize(const _uint_t& rows, const _uint_t& columns)
{
	if (_matval != 0)
	{
		delete [] _matval;
	}

	if (_matrix != 0)
	{
		delete [] _matrix;
	}

	_rows    = rows;
	_columns = columns;
	_matrix  = new _float_t* [_rows];
	_matval  = new _float_t  [_rows*_columns];

	for (_uint_t u=0; u<_rows; ++u)
	{
		_matrix[u] = _matval + _columns * u;
	}

	*this = 0.0;
}

void Matrixf::Resize(const _uint_t& rows, const _uint_t& columns)
{
	this->resize(rows, columns);
}

Matrixf Matrixf::SolveX(const Matrixf& Y) const
{
	return _XSolver->Solve(*this, Y);
}

Matrixf::Matrixf(): Matrix(), _XSolver(0)
{
}

}   // namespace aries
