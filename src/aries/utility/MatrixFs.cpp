#include <aries/utility/MatrixFs.h>

namespace aries
{
	MatrixFs::MatrixFs(const _uint_t& rows, const _uint_t& columns): _rows(rows), _columns(columns), _XSolver(0)
	{
		_matrix = new _float_t* [_rows];
		_matval = new _float_t  [_rows*_columns];
		for (_uint_t u=0; u<_rows; ++u)
		{
			_matrix[u] = _matval + _columns * u;
		}
		*this = 0.0;
	}

	MatrixFs::MatrixFs(const _uint_t& rows, const _uint_t& columns, const _float_t& value): MatrixFs(rows, columns)
	{
		*this = value;
		_XSolver = 0;
	}

	MatrixFs::MatrixFs(const _uint_t& rows, const _uint_t& columns, const _float_t* pvalue): MatrixFs(rows, columns)
	{
		memcpy(_matval, pvalue, sizeof(_float_t) * _rows * _columns);
		_XSolver = 0;
	}

	MatrixFs::MatrixFs(const MatrixFs& m): MatrixFs()
	{
		*this = m;
	}

	const MatrixFs& MatrixFs::operator = (const MatrixFs& m)
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

	MatrixFs::~MatrixFs()
	{
		if (_matval != 0)
		{
			delete [] _matval;
		}

		if (_matrix != 0)
		{
			delete [] _matrix;
		}

		if (_XSolver != 0)
		{
			delete [] _XSolver;
		}
	}

	void MatrixFs::resize(const _uint_t& rows, const _uint_t& columns, const _float_t& value)
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

		*this = value;
	}

	void MatrixFs::Resize(const _uint_t& rows, const _uint_t& columns, const _float_t& value)
	{
		this->resize(rows, columns, value);
	}

	void MatrixFs::resize(const _uint_t& rows, const _uint_t& columns)
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

	void MatrixFs::Resize(const _uint_t& rows, const _uint_t& columns)
	{
		this->resize(rows, columns);
	}

	const MatrixFs& MatrixFs::operator = (const _float_t& value)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] = value;
			}
		}
		_XSolver = 0;
		return *this;
	}

	MatrixFs MatrixFs::operator + (const MatrixFs& m) const
	{
		if ((_columns != m._columns) || (_rows != m._rows))
		{
			std::cerr
				<< "Error!! Code: aries::MatrixFs::operator + (const MatrixFs&)" << std::endl
				<< "        Note: MatrixFs: (" << _rows << "x" << _columns << ") + (" << m._rows << "x" << m._columns << ")" << std::endl;
			exit(-1);
		}
		else
		{
			MatrixFs m2(*this);
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

	MatrixFs MatrixFs::operator + (const _float_t& value) const
	{
		MatrixFs m2(*this);
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				m2._matrix[i][j] += value;
			}
		}
		return m2;
	}

	const MatrixFs& MatrixFs::operator += (const _float_t& value)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] += value;
			}
		}
		return (*this);
	}

	MatrixFs MatrixFs::operator - (const MatrixFs& m) const
	{
		if ((_columns != m._columns) || (_rows != m._rows))
		{
			std::cerr
				<< "Error!! Code: aries::MatrixFs::operator - (const MatrixFs&)" << std::endl
				<< "        Note: MatrixFs: (" << _rows << "x" << _columns << ") - (" << m._rows << "x" << m._columns << ")" << std::endl;
			exit(-1);
		}
		else
		{
			MatrixFs m2(*this);
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

	MatrixFs MatrixFs::operator - (const _float_t& value) const
	{
		MatrixFs m2(*this);
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				m2._matrix[i][j] -= value;
			}
		}
		return m2;
	}

	const MatrixFs& MatrixFs::operator -= (const _float_t& value)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] -= value;
			}
		}
		return (*this);
	}

	MatrixFs MatrixFs::operator * (const MatrixFs& M) const
	{
		_uint_t i, j, k, m;
		MatrixFs c(_rows, M._columns);

		if (_columns != M._rows)
		{
			std::cerr
				<< "Error!! Code: aries::MatrixFs::operator * (const MatrixFs&)" << std::endl
				<< "        Note: MatrixFs: (" << _rows << "x" << _columns << ") * (" << M._rows << "x" << M._columns << ")" << std::endl;
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

	MatrixFs MatrixFs::operator * (const _float_t& value) const
	{
		MatrixFs m2(*this);
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				m2._matrix[i][j] *= value;
			}
		}
		return m2;
	}

	const MatrixFs& MatrixFs::operator *= (const _float_t& value)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] *= value;
			}
		}
		return (*this);
	}

	MatrixFs MatrixFs::operator / (const _float_t& value) const
	{
		MatrixFs m2(*this);
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				m2._matrix[i][j] /= value;
			}
		}
		return m2;
	}

	const MatrixFs& MatrixFs::operator /= (const _float_t& value)
	{
		for (_uint_t i=0; i<_rows; ++i)
		{
			for (_uint_t j=0; j<_columns; ++j)
			{
				_matrix[i][j] /= value;
			}
		}
		return (*this);
	}

	const std::string MatrixFs::characteristic() const
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

	MatrixFs MatrixFs::transpose() const
	{
		MatrixFs m(_columns, _rows);
		for (_uint_t ui=0; ui<_columns; ui++)
		{
			for (_uint_t uj=0; uj<_rows; uj++)
			{
				m(ui+1, uj+1) = _matrix[uj][ui];
			}
		}
		return m;
	}

	MatrixFs MatrixFs::Transpose() const
	{
		return this->transpose();
	}

	_float_t MatrixFs::determine() const
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
					_float_t tdetermine = 0.0;
					for (_uint_t ui=0; ui<_rows; ui++)
					{
						if (ui%2 == 0)
						{
							tdetermine += _matrix[ui][0] * (this->SubMatrix(ui+1, 1).determine());
						}
						else
						{
							tdetermine -= _matrix[ui][0] * (this->SubMatrix(ui+1, 1).determine());
						}
					}
					return tdetermine;
					break;
			}
		}
		else
		{
			std::cerr
				<< "Error!! Code: _float_t aries::MatrixFs::determine()" << std::endl
				<< "        Note: Nonsquare matrix (" << _rows << "x" << _columns << ") has no determine" << std::endl;
			exit(-1);
		}
	}

	_uint_t MatrixFs::rank() const
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
						uRank = std::max(uRank, (this->SubMatrix(ui+1, uj+1).rank()));
					}
				}
				return uRank;
			}
		}
		else
		{
			std::cerr
				<< "Error!! Code: _float_t aries::MatrixFs::rank()" << std::endl
				<< "        Note: Nonsquare matrix (" << _rows << "x" << _columns << ") has no rank" << std::endl;
			exit(-1);
		}
	}

	MatrixFs MatrixFs::adjoint() const
	{
		if (_rows == _columns)
		{
			MatrixFs m(_rows, _columns);
			for (_uint_t ui=1; ui<=_rows; ui++)
			{
				for (_uint_t uj=1; uj<=_columns; uj++)
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
				<< "Error!! Code: MatrixFs aries::MatrixFs::adjoint(const _uint_t&, const _uint_t&)" << std::endl
				<< "        Note: Nonsquare matrix (" << _rows << "x" << _columns << ") has no adjoint matrix" << std::endl;
			exit(-1);
		}
	}

	MatrixFs MatrixFs::Adjoint() const
	{
		return this->adjoint();
	}

	MatrixFs MatrixFs::SubMatrix(const _uint_t& rowFs, const _uint_t& columnFs) const
	{
		if ((rowFs>=1) && (columnFs>=1) && (rowFs<=_rows) && (columnFs<=_columns))
		{
			_uint_t row    = rowFs    - 1;
			_uint_t column = columnFs - 1;
			MatrixFs m(_rows-1, _columns-1);
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
						m(ur+1, uc+1) = _matrix[source_row][source_column];
				}
			}
			return m;
		}
		else
		{
			std::cerr
				<< "Error!! Code: MatrixFs aries::MatrixFs::SubMatrix(const _uint_t&, const _uint_t&)" << std::endl
				<< "        Note: Selected row/column out of range"                                << std::endl;
			exit(-1);
		}
	}

	MatrixFs MatrixFs::inverse() const
	{
		if (_rows == _columns)
		{
			_float_t det = this->determine();
			if (det == 0.0)
			{
				std::cout
					<< "Caution!! Code: MatrixFs aries::MatrixFs::inverse()" << std::endl
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
				<< "Error!! Code: MatrixFs aries::MatrixFs::inverse()" << std::endl
				<< "        Note: Nonsquare matrix (" << _rows << "x" << _columns << ") has no inverse matrix" << std::endl;
			exit(-1);
		}
	}

	MatrixFs MatrixFs::Inverse() const
	{
		return this->inverse();
	}

	_float_t& MatrixFs::operator () (const _uint_t& r, const _uint_t& c)
	{
		if ((r > _rows) || (c > _columns) || (r == 0) || (c == 0))
		{
			std::cerr << "Error!! Code: aries::MatrixFs::operator () (const _uint_t&, const _uint_t&)" << std::endl;
			exit(-1);
		}
		else
		{
			return _matrix[r-1][c-1];
		}
	}

	_float_t MatrixFs::get(const _uint_t& r, const _uint_t& c) const
	{
		if ((r > _rows) || (c > _columns) || (r == 0) || (c == 0))
		{
			std::cerr << "Error!! Code: aries::MatrixFs::get() (const _uint_t&, const _uint_t&)" << std::endl;
			exit(-1);
		}
		else
		{
			return _matrix[r-1][c-1];
		}
	}

	MatrixFs::MatrixFs(): _rows(0), _columns(0), _matrix(0), _matval(0), _XSolver(0)
	{
	}

	void MatrixFs::print() const
	{
		std::cout << "aries::MatrixFs (" << _rows << " x " << _columns << ") (" << characteristic() << ")" << std::endl;
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

	MatrixFs MatrixFs::SolveX(const MatrixFs& Y) const
	{
		return _XSolver->Solve(*this, Y);
	}

}   // namesoace aries



aries::MatrixFs operator - (const aries::MatrixFs& m)
{
	return aries::MatrixFs(m*(-1.0));
}

aries::MatrixFs operator * (const aries::_float_t& s, const aries::MatrixFs& m)
{
	return aries::MatrixFs(m*(s));
}

std::ostream& operator << (std::ostream& os, const aries::MatrixFs& m)
{
	os << "aries::MatrixFs (" << m.rows() << " x " << m.columns() << ") (" << m.characteristic() << ")" << std::endl;

	for (aries::_uint_t i=1; i<=m.rows(); i++)
	{
		for (aries::_uint_t j=1; j<=m.columns(); j++)
		{
			os << std::setiosflags(std::ios::scientific) << std::setprecision(6) << m.get(i, j);
			if (j == m.columns())
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
