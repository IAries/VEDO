#ifndef _ARIES_MATRIX_H
#define _ARIES_MATRIX_H

#include <aries/utility/DataType.h>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>

namespace aries
{

template <typename T>
class Matrix
{

public:

	Matrix(const _uint_t& rows, const _uint_t& columns, const T& tvalue): Matrix(rows, columns)
	{
		*this = tvalue;
	}

	Matrix(const _uint_t& rows, const _uint_t& columns, const T* tpvalue): Matrix(rows, columns)
	{
		memcpy(_matval, tpvalue, sizeof(T) * _rows * _columns);
	}

	Matrix(const Matrix& m): Matrix()
	{
		*this = m;
	}

	const Matrix& operator = (const Matrix& m)
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
			_matrix  = new T* [_rows];
			_matval  = new T  [_rows*_columns];

			for (_uint_t u=0; u<_rows; ++u)
			{
				_matrix[u] = _matval + _columns * u;
			}
		}

		memcpy (_matval, m._matval, sizeof(T) * _rows * _columns);
		return *this;
	}

	~Matrix()
	{
		if (_matval != 0)
		{
			delete [] _matval;
		}

		if (_matrix != 0)
		{
			delete [] _matrix;
		}
	}

	void resize(const _uint_t& rows, const _uint_t& columns, const T& tvalue)
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
		_matrix  = new T* [_rows];
		_matval  = new T  [_rows*_columns];

		for (_uint_t u=0; u<_rows; ++u)
		{
			_matrix[u] = _matval + _columns * u;
		}

		*this = tvalue;
	}

	void Resize(const _uint_t& rows, const _uint_t& columns, const T& tvalue)
	{
		this->resize(rows, columns, tvalue);
	}

	inline _uint_t rows() const
	{
		return _rows;
	}

	inline _uint_t columns() const
	{
		return _columns;
	}

	inline const T* Reference() const
	{
		return _matval;
	}

	const Matrix& operator = (const T& tvalue)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] = tvalue;
			}
		}
		return *this;
	}

	Matrix operator + (const Matrix& m) const
	{
		if ((_columns != m._columns) || (_rows != m._rows))
		{
			std::cerr
				<< "Error!! Code: aries::Matrix::operator + (const Matrix&)" << std::endl
				<< "        Note: Matrix: (" << _rows << "x" << _columns << ") + (" << m._rows << "x" << m._columns << ")" << std::endl;
			exit(-1);
		}
		else
		{
			Matrix m2(*this);
			for (_uint_t i=0; i<m2._rows; ++i)
			{
				for (_uint_t j=0; j<m2._columns; ++j)
				{
					m2._matrix[i][j] += m._matrix[i][j];
				}
			}
			return m2;
		}
	}

	Matrix operator + (const T& tvalue) const
	{
		Matrix m2(*this);
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				m2._matrix[i][j] += tvalue;
			}
		}
		return m2;
	}

	const Matrix& operator += (const T& tvalue)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] += tvalue;
			}
		}
		return (*this);
	}

	Matrix operator - (const Matrix& m) const
	{
		if ((_columns != m._columns) || (_rows != m._rows))
		{
			std::cerr
				<< "Error!! Code: aries::Matrix::operator - (const Matrix&)" << std::endl
				<< "        Note: Matrix: (" << _rows << "x" << _columns << ") - (" << m._rows << "x" << m._columns << ")" << std::endl;
			exit(-1);
		}
		else
		{
			Matrix m2(*this);
			for (_uint_t i=0; i<m2._rows; ++i)
			{
				for (_uint_t j=0; j<m2._columns; ++j)
				{
					m2._matrix[i][j] -= m._matrix[i][j];
				}
			}
			return m2;
		}
	}

	Matrix operator - (const T& tvalue) const
	{
		Matrix m2(*this);
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				m2._matrix[i][j] -= tvalue;
			}
		}
		return m2;
	}

	const Matrix& operator -= (const T& tvalue)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] -= tvalue;
			}
		}
		return (*this);
	}

	Matrix operator * (const Matrix& M) const
	{
		_uint_t i, j, k, m;
		Matrix c(_rows, M._columns);

		if (_columns != M._rows)
		{
			std::cerr
				<< "Error!! Code: aries::Matrix::operator * (const Matrix&)" << std::endl
				<< "        Note: Matrix: (" << _rows << "x" << _columns << ") * (" << M._rows << "x" << M._columns << ")" << std::endl;
			exit(-1);
		}
		else
		{
			m = _columns;
		}

		for (i=0; i<c._rows; ++i)
		{
			for (j=0; j<c._columns; ++j)
			{
				c._matrix[i][j] = _matrix[i][0] * M._matrix[0][j];
			}
		}
		for (k=1; k<m; ++k)
		{
			for (i=0; i<c._rows; ++i)
			{
				for (j=0; j<c._columns; ++j)
				{
					c._matrix[i][j] += _matrix[i][k] * M._matrix[k][j];
				}
			}
		}

		return c;
	}

	Matrix operator * (const T& tvalue) const
	{
		Matrix m2(*this);
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				m2._matrix[i][j] *= tvalue;
			}
		}
		return m2;
	}

	const Matrix& operator *= (const T& tvalue)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] *= tvalue;
			}
		}
		return (*this);
	}

	Matrix operator / (const T& tvalue) const
	{
		Matrix m2(*this);
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				m2._matrix[i][j] /= tvalue;
			}
		}
		return m2;
	}

	const Matrix& operator /= (const T& tvalue)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] /= tvalue;
			}
		}
		return (*this);
	}

	Matrix select(_uint_t* selected, _uint_t len) const
	{
		register _uint_t i;
		register _uint_t j;

		if (_columns < len)
		{
			std::cerr
				<< "Error!! Code: aries::Matrix::select(_uint_t*, _uint_t)" << std::endl
				<< "        Note: Matrix: (" << _rows << "x" << _columns << "), select: " << len << std::endl;
			exit(-1);
		}

		Matrix c(_rows, len);

		for (_uint_t j=0; j<c._columns; ++j)
		{
			if (selected[j] < _columns)
			{
				for (_uint_t i=0; i<c._rows; ++i)
				{
					c._matrix[i][j] = _matrix[i][selected[j]] ;
				}
			}
			else
			{
				std::cerr
					<< "Error!! Code: aries::Matrix::select(_uint_t*, _uint_t)" << std::endl
					<< "        Note: Selected columns out of range" << std::endl;
				exit(-1);
			}
		}

		return c;
	}

	const std::string characteristic() const
	{
		if ((_rows == 1) && (_columns == 1))
		{
			return "VALUE";
		}
		else if (( (_rows == 1) || (_columns == 1) ) && (_rows != _columns))
		{
			return "VECTOR";
		}
		else if ((_rows == _columns) && (_rows != 1))
		{
			return "CUBIC";
		}
		else
		{
			return "IRREGULAR";
		}
   	}

	Matrix transpose() const
	{
		Matrix m(_columns, _rows);
		for (_uint_t ui=0; ui<_columns; ui++)
		{
			for (_uint_t uj=0; uj<_rows; uj++)
			{
				m(ui, uj) = _matrix[uj][ui];
			}
		}
		return m;
	}

	Matrix Transpose() const
	{
		return this->transpose();
	}

	T determine() const
	{
		if (_rows == _columns)
		{
			switch (_rows)
			{
				case 1:
					return _matval[0];
					break;
				case 2:
					return _matrix[0][0] * _matrix[1][1] - _matrix[0][1] * _matrix[1][0];
					break;
				case 3:
					return
						  _matrix[0][0] * _matrix[1][1] * _matrix[2][2]
						+ _matrix[0][1] * _matrix[1][2] * _matrix[2][0]
						+ _matrix[0][2] * _matrix[1][0] * _matrix[2][1]
						- _matrix[0][2] * _matrix[1][1] * _matrix[2][0]
						- _matrix[0][1] * _matrix[1][0] * _matrix[2][2]
						- _matrix[0][0] * _matrix[1][2] * _matrix[2][1];
					break;
				default:
					T tdetermine = 0.0;
					for (_uint_t ui=0; ui<_rows; ui++)
					{
						if (ui%2 == 0)
						{
							tdetermine += _matrix[ui][0] * (this->SubMatrix(ui, 0).determine());
						}
						else
						{
							tdetermine -= _matrix[ui][0] * (this->SubMatrix(ui, 0).determine());
						}
					}
					return tdetermine;
					break;
			}
		}
		else
		{
			std::cerr
				<< "Error!! Code: T aries::Matrix::determine()" << std::endl
				<< "        Note: Nonsquare matrix (" << _rows << "x" << _columns << ") has no determine" << std::endl;
			exit(-1);
		}
	}

	_uint_t rank() const
	{
		if (_rows == _columns)
		{
			if (_rows == 1)
			{
				if (_matval[0] == 0.0)
				{
					return 0;
				}
				else
				{
					return 1;
				}
			}

			if ((this->determine()) != 0.0)
			{
				return _rows;
			}
			else
			{
				_uint_t uRank = 0;
				for (_uint_t ui=0; ui<_rows; ui++)
				{
					for (_uint_t uj=0; uj<_columns; uj++)
					{
						uRank = std::max(uRank, (this->SubMatrix(ui, uj).rank()));
					}
				}
				return uRank;
			}
		}
		else
		{
			std::cerr
				<< "Error!! Code: T aries::Matrix::rank()" << std::endl
				<< "        Note: Nonsquare matrix (" << _rows << "x" << _columns << ") has no rank" << std::endl;
			exit(-1);
		}
	}

	const Matrix& Switch(const _uint_t& row1, const _uint_t& row2)
	{
		if ((row1<_rows) && (row2<_rows))
		{
			_matrix[row1] = _matval + _columns * (row2);
			_matrix[row2] = _matval + _columns * (row1);
			return (*this);
		}
		else
		{
			std::cerr
				<< "Error!! Code: const Matrix& aries::Matrix::switch(const _uint_t&, const _uint_t&)" << std::endl
				<< "        Note: Switched rows out of range"                                          << std::endl;
			exit(-1);
		}
	}

	Matrix adjoint() const
	{
		if (_rows == _columns)
		{
			Matrix m(_rows, _columns);
			for (_uint_t ui=0; ui<_rows; ui++)
			{
				for (_uint_t uj=0; uj<_columns; uj++)
				{
					if ((ui+uj)%2 == 0)
					{
						m(ui, uj) =   SubMatrix(uj, ui).determine();
					}
					else
					{
						m(ui, uj) = -(SubMatrix(uj, ui).determine());
					}
				}
			}
			return m;
		}
		else
		{
			std::cerr
				<< "Error!! Code: Matrix aries::Matrix::adjoint(const _uint_t&, const _uint_t&)" << std::endl
				<< "        Note: Nonsquare matrix (" << _rows << "x" << _columns << ") has no adjoint matrix" << std::endl;
			exit(-1);
		}
	}

	Matrix Adjoint() const
	{
		return this->adjoint();
	}

	Matrix SubMatrix(const _uint_t& row, const _uint_t& column) const
	{
		if ((row<_rows) && (column<_columns))
		{
			Matrix m(_rows-1, _columns-1);
			_uint_t source_row, source_column;
			for (_uint_t ur=0; ur<_rows-1; ur++)
			{
				for (_uint_t uc=0; uc<_columns-1; uc++)
				{
					if (ur < row)
					{
						source_row = ur;
					}
					else
					{
						source_row = ur + 1;
					}
						if (uc < column)
					{
						source_column = uc;
					}
					else
					{
						source_column = uc + 1;
					}
					m(ur, uc) = _matrix[source_row][source_column];
				}
			}
			return m;
		}
		else
		{
			std::cerr
				<< "Error!! Code: Matrix aries::Matrix::SubMatrix(const _uint_t&, const _uint_t&)" << std::endl
				<< "        Note: Selected row/column out of range"                                << std::endl;
			exit(-1);
		}
	}

	Matrix inverse() const
	{
		if (_rows == _columns)
		{
			_float_t det = this->determine();
			if (det == 0.0)
			{
				std::cout
					<< "Caution!! Code: Matrix aries::Matrix::inverse()" << std::endl
					<< "          Note: No inverse matrix" << std::endl;
				return (*this);
			}
			else
			{
				return (this->adjoint()) / det;
			}
		}
		else
		{
			std::cerr
				<< "Error!! Code: Matrix aries::Matrix::inverse()" << std::endl
				<< "        Note: Nonsquare matrix (" << _rows << "x" << _columns << ") has no inverse matrix" << std::endl;
			exit(-1);
		}
	}

	Matrix Inverse() const
	{
		return this->inverse();
	}

	T& operator () (const _uint_t& r, const _uint_t& c)
	{
		if ((r >= _rows) || (c >= _columns))
		{
			std::cerr << "Error!! Code: aries::Matrix::operator () (const _uint_t&, const _uint_t&)" << std::endl;
			exit(-1);
		}
		else
		{
			return _matrix[r][c];
		}
	}

	T get(const _uint_t& r, const _uint_t& c) const
	{
		if ((r >= _rows) || (c >= _columns))
		{
			std::cerr << "Error!! Code: aries::Matrix::get() (const _uint_t&, const _uint_t&)" << std::endl;
			exit(-1);
		}
		else
		{
			return _matrix[r][c];
		}
	}

protected:

	_uint_t _rows;

	_uint_t _columns;

	T**     _matrix;

	T*      _matval;

	Matrix(): _rows(0), _columns(0), _matrix(0), _matval(0)
	{
	}

	Matrix(const _uint_t& rows, const _uint_t& columns): _rows(rows), _columns(columns)
	{
		_matrix = new T* [_rows];
		_matval = new T  [_rows*_columns];
		for (_uint_t u=0; u<_rows; ++u)
		{
			_matrix[u] = _matval + _columns * u;
		}
	}

	void print() const
	{
		std::cout << "aries::Matrix (" << _rows << " x " << _columns << ") (" << characteristic() << ")" << std::endl;
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				std::cout << std::setiosflags(std::ios::scientific) << std::setprecision(6) << _matrix[i][j];
				if (j == _columns-1)
				{
					std::cout << std::endl;
				}
				else
				{
					std::cout << '\t';
				}
			}
		}
		std::cout << std::resetiosflags(std::ios::scientific);
	}
};

}   // namespace aries



template <typename T>
aries::Matrix<T> operator - (const aries::Matrix<T>& m)
{
	return aries::Matrix<T>(m*(-1.0));
}

template <typename T>
aries::Matrix<T> operator * (const T& s , const aries::Matrix<T>& m)
{
	return aries::Matrix<T>(m*(s));
}

template <typename T>
std::ostream& operator << (std::ostream& os, const aries::Matrix<T>& m)
{
	os << "aries::Matrix (" << m.rows() << " x " << m.columns() << ") (" << m.characteristic() << ")" << std::endl;

	for (aries::_uint_t i=0; i<m.rows(); ++i)
	{
		for (aries::_uint_t j=0; j<m.columns(); ++j)
		{
			os << std::setiosflags(std::ios::scientific) << std::setprecision(6) << m.get(i, j);
			if (j == m.columns()-1)
			{
				os << std::endl;
			}
			else
			{
				os << '\t';
			}
		}
	}
	os << std::resetiosflags(std::ios::scientific);

	return os;
}

#endif   // _ARIES_MATRIX_H
