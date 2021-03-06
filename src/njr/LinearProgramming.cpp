#include <njr/LinearProgramming.h>
#include <njr/RandomGenerator.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>

namespace njr
{

Vector3df BMvector(1e6, 1e6, 1e6);

void LinearProgramming::Clear()
{
	cols       = 0;
	rows       = 0;
	num_m      = 0;
	num_s      = 0;
	nbas       = 0;

	_checked   = false;
	_feasiable = false;
	_polygon   = false;

	if (kid != 0)
	{
		delete [] kid;
		kid = 0;
	}

	if (basis != 0)
	{
		delete [] basis;
		basis = 0;
	}

	if (nbasis != 0 )
	{
		delete [] nbasis;
		nbasis = 0;
	}
}

bool LinearProgramming::PurgeArtificialVariable()
{
	if (_checked == false)
	{
		LinearProgramming::Check();
	}

	if (_feasiable == false)
	{
		return false;
	}

	std::vector<_uint_t> NewNbasis;

	for (_uint_t i=0; i<nbas; i++)
	{
		if (kid[nbasis[i]] != 'M')
		{
			NewNbasis.push_back(nbasis[i]);
		}
	}

	delete nbasis;

	nbasis = new _uint_t[NewNbasis.size()];

	for (_uint_t i=0; i< NewNbasis.size(); i++)
	{
		nbasis[i]=NewNbasis[i];
	}

	// rows unchange
	// num_s unchange
	nbas  -= num_m;
	cols  -= num_m;
	num_m  = 0;

	return true;
}

_float_t  LinearProgramming::GetObjValue()
{
	Matrix BIA(rows, 1);
	Matrix RCOST(1, cols);
	Matrix XBASIS(rows, 1);
	Matrix E(rows, rows);

	Matrix rhsmemo = rhs;

	RandomGenerator random;
	_float_t objval;
	_float_t ch [2];

    _uint_t i;
	_uint_t temp;
	_uint_t pxin;
	_uint_t pxout;
	_uint_t cuti = 0;

	for (i=0; i<rows; ++i)
	{
		rhs(i, 0) = rhs(i, 0) * ( 1.0 + random.GeneratingDouble(1e-9, -1e-9) );
	}

	while ((++cuti) < (nbas*5))
	{
		XBASIS = BI * rhs;
		objval = (obj.Select(basis, rows) * XBASIS)(0, 0);

		RCOST = (obj.Select(basis,rows) * BI * A.Select(nbasis,nbas)) - obj.Select(nbasis,nbas);

		for (i=0; i<nbas; ++i)
		{
			pxin = i;
			if (RCOST(0,i) < 0.0)
			{
				break;
			}
		}

		if (i == nbas)
		{
			break;
		}

		BIA = BI * A.Select(nbasis+pxin ,1);

		ch[0] = 1e24;

		for (i=0; i<rows; ++i)
		{
			if (BIA(i,0) == 0.0)
			{
				ch[1] = 1e24;
			}
			else
			{
				ch[1] = XBASIS(i,0) / BIA(i,0);
			}

			if ( (ch[1] > 0) && (ch[1] < ch[0]) )
			{
				ch[0] = ch [1];
				pxout = i;
			}
		}
		temp         = basis[pxout];
		basis[pxout] = nbasis[pxin];
		nbasis[pxin] = temp;

		E = 1;

		for (i=0; i<rows; ++i)
		{
			if (i != pxout)
			{
				E(i, pxout) = - BIA(i,0) / BIA(pxout, 0);
			}
			else
			{
				E(i, pxout) = 1.0 / BIA(pxout, 0);
			}
		}

		BI = E * BI;
	}

	rhs    = rhsmemo;
	XBASIS =  BI * rhs;
	objval = ( obj.Select(basis, rows) * XBASIS ) (0, 0);

	return objval;

}

void LinearProgramming::Allocation()
{
	kid    = new char[cols];
	basis  = new _uint_t [rows];
	nbasis = new _uint_t [nbas];

	obj.Resize(1,cols);
	A.Resize(rows,cols);
	rhs.Resize(rows,1);
	BI.Resize(rows,rows);
}

LinearProgramming::LinearProgramming()
{
	cols  = 0;
	rows  = 0;
	num_m = 0;
	num_s = 0;
	nbas  = 0;

	_checked   = false;
	_feasiable = false;
	_polygon   = false;

	kid    = 0;
	basis  = 0;
	nbasis = 0;
}

LinearProgramming::LinearProgramming(const LinearProgramming &lp)
{
	cols  = lp.cols;
	rows  = lp.rows;
	num_m = lp.num_m;
	num_s = lp.num_s;
	nbas  = lp.nbas;

	LinearProgramming::Allocation ();

	_checked   = lp._checked;
	_feasiable = lp._feasiable;
	_polygon   = lp._polygon;

	memcpy(kid,    lp.kid,     sizeof(char)              * cols);
	memcpy(basis,  lp.basis,   sizeof(_uint_t) * rows);
	memcpy(nbasis, lp.nbasis , sizeof(_uint_t) * nbas);

	obj = lp.obj;
	rhs = lp.rhs;
	BI  = lp.BI;
	A   = lp.A;
}

LinearProgramming::~LinearProgramming()
{
	LinearProgramming::Clear();
}

const LinearProgramming& LinearProgramming::operator = (const LinearProgramming& lp)
{
	LinearProgramming::Clear();

	cols  = lp.cols;
	rows  = lp.rows;
	num_m = lp.num_m;
	num_s = lp.num_s;
	nbas  = lp.nbas ;

	LinearProgramming::Allocation ();

	_checked   = lp._checked;
	_feasiable = lp._feasiable;
	_polygon   = lp._polygon;

	memcpy(kid   , lp.kid   , sizeof(char)              * cols);
	memcpy(basis , lp.basis , sizeof(_uint_t) * rows);
	memcpy(nbasis, lp.nbasis, sizeof(_uint_t) * nbas);

	obj = lp.obj;
	rhs = lp.rhs;
	BI  = lp.BI;
	A   = lp.A;

	return *this;
}

void LinearProgramming::Set(const njr::NJRpolyhedra &a)
{
	LinearProgramming::Clear();

	register _uint_t number;
	register _uint_t i;
	register _uint_t m;
	register _uint_t s;
	_uint_t n;
	_uint_t b;

	std::vector<njr::HalfSpace> vchf = a.constrains();

	for (i=0; i<vchf.size(); ++i)
	{
		vchf[i].Translate(BMvector);
	}

	for_each(vchf.begin(), vchf.end(), std::mem_fun_ref(&njr::HalfSpace::AbsRhs));

	number = (_uint_t) vchf.size();

	for (i=0; i< number; ++i)
	{
		switch ( vchf[i].sense() )
		{
			case L:
				++num_s;
				break;
			case G:
				++num_m;
				++num_s;
				break;
			case E:
				++num_m;
				break;
		}
	}
	rows = number;
	cols = 3 + num_m + num_s;
	nbas = cols - rows;

	LinearProgramming::Allocation();

	for (i=0; i<3; i++)
	{
		kid[i]   = 'P';
		obj(0,i) = 0.0;
	}

	for (; i<3+num_s; i++)
	{
		kid[i]   = 'S';
		obj(0,i) = 0.0;
	}

	for (; i<cols; i++)
	{
		kid[i]   = 'M';
		obj(0,i) = -2.0;
	}

	nbasis [0] = 0;
	nbasis [1] = 1;
	nbasis [2] = 2;

	for (i=0, m=3+num_s, s=3, n=3, b=0; i<number; ++i)
	{
		A(i, 0)   = vchf[i].a();
		A(i, 1)   = vchf[i].b();
		A(i, 2)   = vchf[i].c();
		rhs(i, 0) = vchf[i].d();

		switch (vchf[i].sense())
		{
			case G:
				nbasis[n] = s;
				basis[b]  = m;
				A(i,s)    = -1.0;
				A(i,m)    = 1.0;
				n++;
				b++;
				s++;
				m++;
				break;
			case E:
				basis[b] = m;
				A(i,m)   = 1.0;
				b++;
				m++;
				break;
			case L:
				basis[b] = s;
				A(i,s)   = 1.0;
				b++;
				s++;
				break;
		}
	}
}


bool LinearProgramming::Check()
{
	if (_checked)
	{
		return _feasiable;
	}

     Matrix BIA(rows, 1);
     Matrix RCOST(1, cols);
     Matrix XBASIS(rows, 1);
     Matrix E(rows, rows);

	 Matrix rhsmemo = rhs;

     _float_t objval;
     _float_t ch [2];

     _uint_t i;
     _uint_t temp;

	// Counter of iterationis
	_uint_t cuti = 0;

	// The pointer of the variable is selected from nonbasis set and will be put into basis set in next iteration.
	_uint_t pxin;

	// The pointer of the variable is selected from basis set and will be put into nonbasis set in next iteration.
	_uint_t pxout;

	RandomGenerator random(2000);

	for (i=0; i<rows; ++i)
	{
		rhs(i, 0) = rhs(i, 0) * (1.0 + random.GeneratingDouble(1e-9, -1e-9) );
	}

	BI = 1.0;

	while ((++cuti) < (nbas*5))
	{
		// Calculate the feasiable solution and objective value in this iteration.
		XBASIS = BI * rhs;
		objval = (obj.Select(basis, rows) * XBASIS) (0,0);

		if (objval >= 0)
		{
			break;
		}

		/********************************************************************************************************
		 * 1. calculate the reduce cost (RCOST) of each non-basis variable and make sue if all RCOSTS >= 0.
		 * 2. If all RCOST >= 0 , the feasiable solution in the iteration is the optimum solution of this stdlp.
		 * 3. Select one non-basis variable whoese RCOST < 0 into basis set in next iteration.
		 ********************************************************************************************************/

		RCOST = (obj.Select(basis,rows) * BI * A.Select(nbasis,nbas)) - obj.Select(nbasis,nbas);

		for (i=0; i<nbas; ++i)
		{
			pxin = i;
			if (RCOST(0,i) < 0.0)
			{
				break;
			}
		}

		if (i == nbas)
		{
			break;
		}

		// Select one basis variable into nonbasis set
        BIA = BI * A.Select(nbasis+pxin, 1);

        ch[0] = 1e24;

		for (i=0; i<rows; ++i)
		{
			if(BIA(i,0) == 0.0)
			{
				ch[1] = 1e24;
			}
			else
			{
				ch[1] = XBASIS(i,0) / BIA(i,0);
			}

			if ( (ch[1] > 0) && (ch[1] < ch[0]) )
			{
				ch[0] = ch[1];
				pxout = i;
			}
		}

		// Swap the variable of basis and the variable of non-basis selected
		if (pxout > rows)
		{
			throw 0.0;
			return false;
		}

		temp         = basis[pxout];
		basis[pxout] = nbasis[pxin];
		nbasis[pxin] = temp;

		// Calculte new BI

		E = 1;

		for (i=0; i<rows; ++i)
		{
			if (i != pxout)
			{
				E(i, pxout) = - BIA(i,0) / BIA(pxout,0);
			}
			else
			{
				E(i, pxout) = 1.0 / BIA(pxout,0) ;
			}
		}

		BI = E * BI;
	}

	_checked   = true;
	_feasiable = (objval == 0 ) ? true : false;

	temp = 0;
	for (i=0; i<nbas; ++i)
	{
		if ((kid[nbasis[i]] == 'S') || (kid[nbasis[i]] == 'P'))
		{
			temp++;
		}
	}

	_polygon = (temp==2) ? true : false;
	rhs      = rhsmemo;

	return _feasiable;
}

void LinearProgramming::print() const
{
	register _uint_t i;

	std::cout << ((_checked)   ? "checked "           : "unchecked "          );
	std::cout << ((_feasiable) ? "feasiable "         : "infeasiable "        );
	std::cout << ((_polygon)   ? "This is a polygon " : "This is not polygon ");
	std::cout << std::endl;

	printf("\nbasis\n");
	for (i=0; i<rows; ++i)
	{
		printf( (i%5==4) ? "[%3d %c]\n" : "[%3d %c] ", basis[i], kid[basis[i]]);
	}

	printf("\nnonbasis\n");
	for (i=0; i<nbas; ++i)
	{
		printf( (i%5==4) ? "[%3d %c]\n" : "[%3d %c]", nbasis[i], kid[nbasis[i]]);
	}
	printf ("\n");
}


NJRpolygon LinearProgramming::GetPolygon()
{
	std::vector<Vector3df>vertexes(0);

	if (_polygon == false)
	{
		return NJRpolygon(vertexes);
	}

	Matrix BIA(rows, 1);
	Matrix XBASIS(rows, 1);
	Matrix X(rows, 1);
	Matrix E(rows, rows);
	Matrix rhsmemo = rhs;

	_uint_t i;
	_uint_t num_ans;
	_uint_t temp;
	_float_t ch[2];

	/**************************************************************************************************************************
	 * There should be only two non-basis variables that is not artiffical variables,if all feasible solutions of this problems
	 * are on the same plane.
	 **************************************************************************************************************************/
    const _uint_t max_ans = (rows-1) * (rows-2) / 2;

	_uint_t nonbasis[3];
	_uint_t inibasis[2];

	// The pointer of the variable is selected from nonbasis set and will be put into basis set in next iteration.
	_uint_t  pxout=0;

	RandomGenerator random(2000);

	for (i=0; i<rows; ++i)
	{
		rhs(i, 0) = rhs(i, 0) * (1.0 + random.GeneratingDouble(1e-9, -1e-9));
	}

	// Select and record the only two non-basis variables that is not artiffical varisable
	for(temp=0, i=0 ;i<nbas; ++i)
	{
		if ((kid[nbasis[i]] == 'S') || (kid[nbasis[i]] == 'P'))
		{
			inibasis[temp]=nbasis[i];
			temp++;
		}
	}

	nonbasis[1] = inibasis[0];
	nonbasis[2] = inibasis[1];

	for (num_ans=0; num_ans<max_ans; num_ans++)
	{
		XBASIS = BI * rhs;
		X = BI * rhsmemo;

		_float_t x = 0.0;
		_float_t y = 0.0;
		_float_t z = 0.0;

		for(i=0; i<rows ;i++)
		{
			switch (basis[i])
			{
				case 0:
					x = X(i,0);
					break;
				case 1:
					y = X(i,0);
					break;
				case 2:
					z = X(i,0);
					break;
			}
		}

		vertexes.push_back(Vector3df(x,y,z) - BMvector);
		BIA = BI * (A.Select(nonbasis + 2 ,1));

		ch[0] = 1e24;

        for (i=0; i<rows ;++i)
		{
			ch[1] = (BIA(i,0)==0.0) ? 1e24 : XBASIS(i,0)/BIA(i,0);
			if ((ch[1] > 0) && (ch[1] < ch[0]))
			{
				ch[0] = ch [1] ;
				pxout = i;
			}
        }

		nonbasis[0]  = basis[pxout];
		basis[pxout] = nonbasis[2];

		if ((inibasis[0] == nonbasis[0]) && (inibasis[1] == nonbasis[1]))
		{
			break;
		}

		nonbasis[2] = nonbasis[1];
		nonbasis[1] = nonbasis[0];

		E = 1.0;

        for (i=0; i<rows; ++i)
		{
			E(i, pxout) = (i!=pxout) ? (-BIA(i,0)/BIA(pxout,0)) : (1.0/BIA(pxout,0));
		}
        BI = E * BI;
	}

	rhs = rhsmemo;
	return NJRpolygon(vertexes);
}

bool LinearProgramming::GetExtremeValue
	(_float_t& maxX, _float_t& minX, _float_t& maxY,
	 _float_t& minY, _float_t& maxZ, _float_t& minZ )
{

	if (LinearProgramming::PurgeArtificialVariable() == false)
	{
		return false;
	}

	obj(0,0) = 1.0;
	maxX     =  LinearProgramming::GetObjValue() - BMvector.dot(Vector3df(1.0, 0.0, 0.0));

	obj(0,0) = -1.0;
	minX     = -LinearProgramming::GetObjValue() - BMvector.dot(Vector3df(1.0, 0.0, 0.0));

	obj(0,0) = 0.0;

	obj(0,1) = 1.0;
	maxY     =  LinearProgramming::GetObjValue() - BMvector.dot(Vector3df(0.0, 1.0, 0.0));

	obj(0,1) = -1.0;
	minY     = -LinearProgramming::GetObjValue() - BMvector.dot(Vector3df(0.0, 1.0, 0.0));

	obj(0,1) = 0.0;

	obj(0,2) = 1.0;
	maxZ     = LinearProgramming::GetObjValue() - BMvector.dot(Vector3df(0.0, 0.0, 1.0));

	obj(0,2) = -1.0;

	minZ     = -LinearProgramming::GetObjValue() - BMvector.dot(Vector3df(0.0, 0.0, 1.0));

	obj(0,2) = 0.0;

	return true;
}

}   // namespace njr
