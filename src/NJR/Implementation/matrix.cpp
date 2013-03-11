#include <NJR/Interfaces/matrix.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

const char* NJRmatrix::characteristic() const
{
	if ( (_rows==1) && (_cols==1) )
	{
		return "VALUE";
	}
	else if ( ( (_rows==1) || (_cols==1) ) && (_rows != _cols) )
	{
		return "VECTOR";
	}
	else if ( (_rows == _cols) && (_rows != 1) )
	{
		return "CUBIC";
	}
	else
	{
		return "IRREGULAR";
	}
};

NJRmatrix::NJRmatrix()
{
	_rows  = 0;
	_cols  = 0;
	matrix = 0;
	matval = 0;
};

NJRmatrix::NJRmatrix(const unsigned int& r, const unsigned int& c)
{
	unsigned int i;
	_rows = r;
	_cols = c;

	matrix = new double* [_rows];
	matval = new double  [_rows*_cols];

	for (i=0; i<r; ++i)
	{
		matrix[i] = matval + c * i;
	}

	*this = 0.0;
};

NJRmatrix::NJRmatrix(const NJRmatrix& M)
{
	register unsigned int i;

	_rows = M._rows;
	_cols = M._cols;

	matrix = new double * [_rows];
	matval = new double   [_rows*_cols];

	for (i=0; i<_rows; ++i)
	{
		matrix[i] = matval + _cols * i;
	}

	memcpy(matval, M.matval, sizeof(double) * _rows * _cols);
};

NJRmatrix::~NJRmatrix()
{
	if (matval != 0)
	{
		delete [] matval;
	}

	if (matrix != 0)
	{
		delete [] matrix;
	}
};

void NJRmatrix::Resize(const unsigned int& r, const unsigned int& c)
{
	if(matval != 0)
	{
		delete [] matval;
	}

	if(matrix != 0 )
	{
		delete [] matrix;
	}

	unsigned int i;
	_rows = r;
	_cols = c;

	matrix = new double* [_rows];
	matval = new double  [_rows*_cols];

	for (i=0; i<r; ++i)
	{
		matrix [i] = matval + c * i;
	}

	*this = 0.0;
};


const NJRmatrix& NJRmatrix::operator = (const NJRmatrix& M)
{
	register unsigned int i;

	if ( (_rows != M._rows) || (_cols != M._cols) )
	{
		if(matval != 0 )
		{
			delete [] matval;
		}

		if(matrix != 0)
		{
			delete [] matrix;
		}

		_rows = M._rows;
		_cols = M._cols;

		matrix = new double* [_rows];
		matval = new double  [_rows*_cols];

		for (i=0; i<_rows; ++i)
		{
			matrix[i] = matval + _cols * i;
		}
	}

	memcpy (matval, M.matval, sizeof(double) * _rows * _cols);
	return *this;
};

const NJRmatrix& NJRmatrix::operator = (const double& v)
{
	register unsigned int i;
	register unsigned int j;

	for (i=0; i<_rows; ++i)
	{
		for (j=0; j<_cols; ++j)
		{
			matrix[i][j] = (i==j) ? v : 0.0;
		}
	}

	return *this;
};

NJRmatrix NJRmatrix::operator * (const NJRmatrix &M) const
{
	register unsigned int i;
	register unsigned int j;
	register unsigned int k;
	register unsigned int m;

	NJRmatrix C(_rows, M._cols);

	if (_cols != M._rows)
	{
		cerr
			<< "Matrix operator * do nothing and terminate the program"
			<< endl;
		exit(0);
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
};

NJRmatrix NJRmatrix::operator + (const NJRmatrix &M) const
{
	register unsigned int i;
	register unsigned int j;

	NJRmatrix C(_rows, _cols);

	if (( _cols != M._cols) || ( _rows != M._rows))
	{
		cerr
			<< "matrix operator + do nothing and terminate the program"
			<< endl;
		exit(0);
	}

	for (i=0; i<C._rows; ++i)
	{
		for (j=0; j<C._cols; ++j)
		{
			C.matrix[i][j] = matrix[i][j] + M.matrix[i][j];
		}
	}

	return C;
};

NJRmatrix NJRmatrix::operator - (const NJRmatrix &M) const
{
	register unsigned int i;
	register unsigned int j;

	NJRmatrix C(_rows,_cols);

	if ( ( _cols != M._cols) || ( _rows != M._rows) )
	{
		cerr
			<< "matrix operator - do nothing and terminate the program"
			<< endl;
		exit(0);
	}

	for (i=0; i<C._rows; ++i)
	{
		for (j=0; j<C._cols; ++j)
		{
			C.matrix[i][j] = matrix[i][j] - M.matrix[i][j];
		}
	}

	return C;
};

NJRmatrix NJRmatrix::Select (unsigned int* selected, unsigned int len) const
{
	register unsigned int i;
	register unsigned int j;

	if (_cols < len)
	{
		cerr
			<< "Matrix select function do nothing and terminate the program"
			<< endl;
		exit(0);
	}

	NJRmatrix C(_rows, len);

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
			cerr << "Select error !! selected columns out of range !!" << endl;
			exit(0);
		}
	}

	return C;
};

const NJRmatrix& NJRmatrix::Transpose()
{
	NJRmatrix t(_cols, _rows);

	register unsigned int i;
	register unsigned int j;

	for (i=0; i<_rows; ++i)
	{
		for (j=0; j<_cols; ++j)
		{
			t.matrix[j][i] = matrix[i][j];
		}
	}

	return (*this = t);
};

double& NJRmatrix::operator () (const unsigned int& r, const unsigned int& c)
{
	if ( (r >= _rows) || (c >= _cols) )
	{
		cout << "over matrix operator()" << endl;
		exit(0);
	}
	else
	{
		return matrix[r][c];
	}
};

double NJRmatrix::Get(const unsigned int& r, const unsigned int& c) const
{
	if ( (r >= _rows) || (c >= _cols) )
	{
		cout << "over matrix.get()" << endl;
		exit(0);
	}
	else
	{
		return matrix[r][c];
	}
};

ostream& operator << (ostream& os, const NJRmatrix& m)
{
	register unsigned int i;
	register unsigned int j;

	printf ("rows=%3d cols=%3d  ", m.rows(), m.columns());

	cout << m.characteristic() << endl;

	for (i=0; i<m.rows(); ++i)
	{
		for (j=0; j<m.columns(); ++j)
		{
			printf( (j==m.columns()-1) ? "%3.1f\n" :"%3.1f ", m.Get(i,j) );
		}
	}

	return os;
};
