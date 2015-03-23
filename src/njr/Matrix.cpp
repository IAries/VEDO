#include <njr/Matrix.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace njr
{

const char* Matrix::characteristic() const
{
	if ((_rows==1) && (_cols==1))
	{
		return "VALUE";
	}
	else if (( (_rows==1) || (_cols==1) ) && (_rows != _cols))
	{
		return "VECTOR";
	}
	else if ((_rows == _cols) && (_rows != 1))
	{
		return "CUBIC";
	}
	else
	{
		return "IRREGULAR";
	}
}

Matrix::Matrix()
{
	_rows  = 0;
	_cols  = 0;
	matrix = 0;
	matval = 0;
}

Matrix::Matrix(const _uint_t& r, const _uint_t& c)
{
	_uint_t i;
	_rows = r;
	_cols = c;

	matrix = new _float_t* [_rows];
	matval = new _float_t  [_rows*_cols];

	for (i=0; i<r; ++i)
	{
		matrix[i] = matval + c * i;
	}

	*this = 0.0;
}

Matrix::Matrix(const Matrix& M)
{
	register _uint_t i;

	_rows = M._rows;
	_cols = M._cols;

	matrix = new _float_t * [_rows];
	matval = new _float_t   [_rows*_cols];

	for (i=0; i<_rows; ++i)
	{
		matrix[i] = matval + _cols * i;
	}

	memcpy(matval, M.matval, sizeof(_float_t) * _rows * _cols);
}

Matrix::~Matrix()
{
	if (matval != 0)
	{
		delete [] matval;
	}

	if (matrix != 0)
	{
		delete [] matrix;
	}
}

void Matrix::Resize(const _uint_t& r, const _uint_t& c)
{
	if (matval != 0)
	{
		delete [] matval;
	}

	if (matrix != 0)
	{
		delete [] matrix;
	}

	_uint_t i;
	_rows = r;
	_cols = c;

	matrix = new _float_t* [_rows];
	matval = new _float_t  [_rows*_cols];

	for (i=0; i<r; ++i)
	{
		matrix [i] = matval + c * i;
	}

	*this = 0.0;
}


const Matrix& Matrix::operator = (const Matrix& M)
{
	register _uint_t i;

	if ((_rows != M._rows) || (_cols != M._cols))
	{
		if (matval != 0 )
		{
			delete [] matval;
		}

		if (matrix != 0)
		{
			delete [] matrix;
		}

		_rows = M._rows;
		_cols = M._cols;

		matrix = new _float_t* [_rows];
		matval = new _float_t  [_rows*_cols];

		for (i=0; i<_rows; ++i)
		{
			matrix[i] = matval + _cols * i;
		}
	}

	memcpy (matval, M.matval, sizeof(_float_t) * _rows * _cols);
	return *this;
}

const Matrix& Matrix::operator = (const _float_t& v)
{
	register _uint_t i;
	register _uint_t j;

	for (i=0; i<_rows; ++i)
	{
		for (j=0; j<_cols; ++j)
		{
			matrix[i][j] = (i==j) ? v : 0.0;
		}
	}

	return *this;
}

Matrix Matrix::operator * (const Matrix &M) const
{
	register _uint_t i;
	register _uint_t j;
	register _uint_t k;
	register _uint_t m;

	Matrix C(_rows, M._cols);

	if (_cols != M._rows)
	{
		std::cerr << "Error!! Code: Matrix::operator * (const Matrix)" << std::endl;
		exit(-1);
	}
	else
	{
		m = _cols;
	}

	for (k=0; k<m; ++k)
	{
		for (i=0; i<C._rows; ++i)
		{
			for (j=0; j<C._cols; ++j)
			{
				if (matrix[i][k] != 0.0)
				{
					C.matrix[i][j] += matrix[i][k] * M.matrix[k][j];
				}
			}
		}
	}

	return C;
}

Matrix Matrix::operator + (const Matrix &M) const
{
	register _uint_t i;
	register _uint_t j;

	Matrix C(_rows, _cols);

	if ((_cols != M._cols) || (_rows != M._rows))
	{
		std::cerr << "Error!! Code: Matrix::operator + (const Matrix)" << std::endl;
		exit(-1);
	}

	for (i=0; i<C._rows; ++i)
	{
		for (j=0; j<C._cols; ++j)
		{
			C.matrix[i][j] = matrix[i][j] + M.matrix[i][j];
		}
	}

	return C;
}

Matrix Matrix::operator - (const Matrix &M) const
{
	register _uint_t i;
	register _uint_t j;

	Matrix C(_rows,_cols);

	if ((_cols != M._cols) || ( _rows != M._rows))
	{
		std::cerr << "Error!! Code: Matrix::operator - (const Matrix)" << std::endl;
		exit(-1);
	}

	for (i=0; i<C._rows; ++i)
	{
		for (j=0; j<C._cols; ++j)
		{
			C.matrix[i][j] = matrix[i][j] - M.matrix[i][j];
		}
	}

	return C;
}

Matrix Matrix::Select(_uint_t* selected, _uint_t len) const
{
	register _uint_t i;
	register _uint_t j;

	if (_cols < len)
	{
		std::cerr << "Error!! Code: Matrix::Select (vedo_uint_t*, vedo_uint_t)" << std::endl;
		exit(-1);
	}

	Matrix C(_rows, len);

	for (j=0; j<C._cols; ++j)
	{
		if (selected[j] < _cols)
		{
             for (i=0; i<C._rows; ++i)
			 {
				C.matrix[i][j] = matrix[i][selected[j]] ;
			 }
		}
		else
		{
			std::cerr
				<< "Error!! Code: Matrix::Select (vedo_uint_t*, vedo_uint_t)" << std::endl
				<< "        Note: Selected columns out of range"              << std::endl;
			exit(-1);
		}
	}

	return C;
}

const Matrix& Matrix::Transpose()
{
	Matrix t(_cols, _rows);

	register _uint_t i;
	register _uint_t j;

	for (i=0; i<_rows; ++i)
	{
		for (j=0; j<_cols; ++j)
		{
			t.matrix[j][i] = matrix[i][j];
		}
	}

	return (*this = t);
}

_float_t& Matrix::operator () (const _uint_t& r, const _uint_t& c)
{
	if ((r >= _rows) || (c >= _cols))
	{
		std::cerr << "Error!! Code: Matrix::operator () (const vedo_uint_t&, const vedo_uint_t&)" << std::endl;
		exit(-1);
	}
	else
	{
		return matrix[r][c];
	}
}

_float_t Matrix::Get(const _uint_t& r, const _uint_t& c) const
{
	if ((r >= _rows) || (c >= _cols))
	{
		std::cerr << "Error!! Code: Matrix::Get() (const vedo_uint_t&, const vedo_uint_t&)" << std::endl;
		exit(-1);
	}
	else
	{
		return matrix[r][c];
	}
}

}   // namespace njr



std::ostream& operator << (std::ostream& os, const njr::Matrix& m)
{
	register njr::_uint_t i;
	register njr::_uint_t j;

	printf ("rows=%3d cols=%3d  ", m.rows(), m.columns());

	std::cout << m.characteristic() << std::endl;

	for (i=0; i<m.rows(); ++i)
	{
		for (j=0; j<m.columns(); ++j)
		{
			printf( (j==m.columns()-1) ? "%3.1f\n" :"%3.1f ", m.Get(i,j) );
		}
	}

	return os;
}
