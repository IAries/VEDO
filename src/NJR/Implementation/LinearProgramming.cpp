#include <NJR/Interfaces/LinearProgramming.h>
#include <NJR/Interfaces/random.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>

using namespace std;

NJRvector3d BMvector(1e6, 1e6, 1e6);

void NJRLinearProgramming::Clear()
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
};

bool NJRLinearProgramming::PurgeArtificialVariable()
{
	if (_checked == false)
	{
		NJRLinearProgramming::Check();
	}

	if (_feasiable == false)
	{
		return false;
	}

	vector<unsigned int> NewNbasis;

	for (unsigned int i=0; i<nbas; i++)
	{
		if (kid[nbasis[i]] != 'M')
		{
			NewNbasis.push_back(nbasis[i]);
		}
	}

	delete nbasis;

	nbasis = new unsigned int[NewNbasis.size()];

	for (unsigned int i=0; i< NewNbasis.size(); i++)
	{
		nbasis[i]=NewNbasis[i];
	}

	// rows unchange
	// num_s unchange
	nbas  -= num_m;
	cols  -= num_m;
	num_m  = 0;

	return true;
};

double  NJRLinearProgramming::GetObjValue()
{
	NJRmatrix BIA(rows, 1);
	NJRmatrix RCOST(1, cols);
	NJRmatrix XBASIS(rows, 1);
	NJRmatrix E(rows, rows);

	NJRmatrix rhsmemo = rhs;

	NJR::RandomGenerator random;
	double objval;
	double ch [2];

    unsigned int i;
	unsigned int temp;
	unsigned int pxin;
	unsigned int pxout;
	unsigned int cuti = 0;

	for (i=0; i<rows; ++i)
	{
		rhs(i, 0) = rhs(i, 0) * ( 1.0 + random(1e-9, -1e-9) );
	}

	while ( (++cuti) < (nbas*5) )
	{
		XBASIS = BI * rhs;
		objval = ( obj.Select(basis, rows) * XBASIS ) (0, 0);

		RCOST
			= ( obj.Select(basis,rows) * BI * A.Select(nbasis,nbas) )
			- obj.Select(nbasis,nbas);

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

};

void NJRLinearProgramming::Allocation()
{
	kid    = new char[cols];
	basis  = new unsigned int [rows];
	nbasis = new unsigned int [nbas];

	obj.Resize(1,cols);
	A.Resize(rows,cols);
	rhs.Resize(rows,1);
	BI.Resize(rows,rows);
};

NJRLinearProgramming::NJRLinearProgramming()
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
};

NJRLinearProgramming::NJRLinearProgramming(const NJRLinearProgramming &lp)
{
	cols  = lp.cols;
	rows  = lp.rows;
	num_m = lp.num_m;
	num_s = lp.num_s;
	nbas  = lp.nbas;

	NJRLinearProgramming::Allocation ();

	_checked   = lp._checked;
	_feasiable = lp._feasiable;
	_polygon   = lp._polygon;

	memcpy(kid,    lp.kid,     sizeof(char)         * cols);
	memcpy(basis,  lp.basis,   sizeof(unsigned int) * rows);
	memcpy(nbasis, lp.nbasis , sizeof(unsigned int) * nbas);

	obj = lp.obj;
	rhs = lp.rhs;
	BI  = lp.BI;
	A   = lp.A;
};

NJRLinearProgramming::~NJRLinearProgramming()
{
	NJRLinearProgramming::Clear();
};

const NJRLinearProgramming& NJRLinearProgramming::operator =
	(const NJRLinearProgramming& lp)
{
	NJRLinearProgramming::Clear();

	cols  = lp.cols;
	rows  = lp.rows;
	num_m = lp.num_m;
	num_s = lp.num_s;
	nbas  = lp.nbas ;

	NJRLinearProgramming::Allocation ();

	_checked   = lp._checked;
	_feasiable = lp._feasiable;
	_polygon   = lp._polygon;

	memcpy(kid   , lp.kid   , sizeof(char)         * cols);
	memcpy(basis , lp.basis , sizeof(unsigned int) * rows);
	memcpy(nbasis, lp.nbasis, sizeof(unsigned int) * nbas);

	obj = lp.obj;
	rhs = lp.rhs;
	BI  = lp.BI;
	A   = lp.A;

	return *this;
};

void NJRLinearProgramming::Set(const NJRpolyhedra &a)
{
	NJRLinearProgramming::Clear();

	register unsigned int number;
	register unsigned int i;
	register unsigned int m;
	register unsigned int s;
	unsigned int n;
	unsigned int b;

	vector<NJRhalfspace> vchf = a.constrains();

	for (i=0; i<vchf.size(); ++i)
	{
		vchf[i].Translate(BMvector);
	}

	for_each
		(vchf.begin(), vchf.end(), mem_fun_ref(&NJRhalfspace::AbsRhs));

	number = (unsigned int) vchf.size();

	for(i=0; i< number ; ++i)
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

	NJRLinearProgramming::Allocation();

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

	for (i=0 , m=3+num_s , s=3 , n=3 , b=0 ; i < number ; ++i)
	{
		A(i, 0)= vchf[i].a();
		A(i, 1)= vchf[i].b();
		A(i, 2)= vchf[i].c();
		rhs(i, 0) = vchf[i].d();

		switch ( vchf[i].sense() )
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
};


bool NJRLinearProgramming::Check()
{
	if (_checked)
	{
		return _feasiable;
	}

     NJRmatrix BIA(rows, 1);
     NJRmatrix RCOST(1, cols);
     NJRmatrix XBASIS(rows, 1);
     NJRmatrix E(rows, rows);

	 NJRmatrix rhsmemo = rhs;

     double objval;
     double ch [2];

     unsigned int i;
     unsigned int temp;

	// Counter of iterationis
	unsigned int cuti = 0;

	/**************************************************************************
	 * The pointer of the variable is selected from nonbasis set and will be
	 * put into basis set in next iteration.
	 **************************************************************************/
	unsigned int pxin;

	/**************************************************************************
	 * The pointer of the variable is selected from basis set and will be put
	 * into nonbasis set in next iteration.
	 **************************************************************************/
	unsigned int pxout;

	NJR::RandomGenerator random(2000);

	for (i=0; i<rows; ++i)
	{
		rhs(i, 0) = rhs(i, 0) * (1.0 + random(1e-9, -1e-9) );
	}

	BI = 1.0;

	while ( (++cuti) < (nbas*5) )
	{
		/**********************************************************************
		 * Calculate the feasiable solution and objective value in this
		 * iteration.
		 **********************************************************************/
		XBASIS = BI * rhs;
		objval = (obj.Select(basis, rows) * XBASIS) (0,0);

		if ( objval >= 0 )
		{
			break;
		}

		/**********************************************************************
		 * 1. calculate the reduce cost (RCOST) of each non-basis variable and
		 *    make sue if all RCOSTS >= 0.
		 * 2. If all RCOST >= 0 , the feasiable solution in the iteration is
		 *    the optimum solution of this stdlp.
		 * 3. Select one non-basis variable whoese RCOST < 0 into basis set in
		 *    next iteration.
		 **********************************************************************/

		RCOST
			= (obj.Select(basis,rows) * BI * A.Select(nbasis,nbas))
			- obj.Select(nbasis,nbas);

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
};

void NJRLinearProgramming::print () const
{
	register unsigned int i;

	cout << ((_checked)   ? "checked "           : "unchecked "          );
	cout << ((_feasiable) ? "feasiable "         : "infeasiable "        );
	cout << ((_polygon)   ? "This is a polygon " : "This is not polygon ");
	cout << endl;

	printf("\nbasis\n");
	for (i=0; i<rows; ++i)
	{
		printf
			( (i%5==4) ? "[%3d %c]\n" : "[%3d %c] ", basis[i], kid[basis[i]]);
	}

	printf("\nnonbasis\n");
	for (i=0; i<nbas; ++i)
	{
		printf
			( (i%5==4) ? "[%3d %c]\n" : "[%3d %c]", nbasis[i], kid[nbasis[i]]);
	}
	printf ("\n");
};


NJRpolygon NJRLinearProgramming::GetPolygon()
{
	vector<NJRvector3d>vertexes(0);

	if (_polygon==false)
	{
		return NJRpolygon(vertexes);
	}

	NJRmatrix BIA(rows, 1);
	NJRmatrix XBASIS(rows, 1);
	NJRmatrix X(rows, 1);
	NJRmatrix E(rows, rows);
	NJRmatrix rhsmemo = rhs;

	unsigned int i;
	unsigned int num_ans;
	unsigned int temp;
	double       ch[2];

	/**************************************************************************
	 * There should be only two non-basis variables that is not artiffical
	 * variables,if all feasible solutions of this problems are on the same
	 * plane.
	 **************************************************************************/
    const unsigned max_ans = (rows-1) * (rows-2) / 2;

	unsigned int nonbasis[3];
	unsigned int inibasis[2];

	/**************************************************************************
	 * The pointer of the variable is selected from nonbasis set and will be
	 * put into basis set in next iteration.
	 **************************************************************************/
	unsigned int  pxout=0;

   	NJR::RandomGenerator random(2000);

	for (i=0; i<rows; ++i)
	rhs(i, 0) = rhs(i, 0) * ( 1.0 + random(1e-9, -1e-9) );

	/**************************************************************************
	 * Select and record the only two non-basis variables that is not
	 * artiffical varisable
	 **************************************************************************/
	for(temp=0, i=0 ;i<nbas; ++i)
	{
		if ( (kid[nbasis[i]] == 'S') || (kid[nbasis[i]] == 'P') )
		{
			inibasis[temp]=nbasis[i];
			temp++;
		}
	}

	nonbasis[1]=inibasis[0];
	nonbasis[2]=inibasis[1];

	for (num_ans=0; num_ans<max_ans; num_ans++)
	{
		XBASIS = BI * rhs;
		X = BI * rhsmemo;

		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

		for(i=0; i<rows ;i++)
		{
			switch(basis[i])
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

		vertexes.push_back(NJRvector3d(x,y,z) - BMvector);
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

		if (   (inibasis[0] == nonbasis[0]) && (inibasis[1] == nonbasis[1]) )
		{
			break;
		}

		nonbasis[2] = nonbasis[1];
		nonbasis[1] = nonbasis[0];

		E = 1.0;

        for (i=0; i<rows; ++i)
		{
			E(i, pxout)
				= (i!=pxout) ? (-BIA(i,0)/BIA(pxout,0)) : (1.0/BIA(pxout,0));
		}
        BI = E * BI;
	}

	rhs = rhsmemo;
	return NJRpolygon(vertexes);
};


bool  NJRLinearProgramming::GetExtremeValue
	(double& maxX,
	double& minX,
	double& maxY,
	double& minY,
	double& maxZ,
	double& minZ)
{

	if (NJRLinearProgramming::PurgeArtificialVariable() == false)
	{
		return false;
	}

	obj(0,0) = 1.0;
	maxX     =  NJRLinearProgramming::GetObjValue() - BMvector % AXIALX;

	obj(0,0) = -1.0;
	minX     = -NJRLinearProgramming::GetObjValue() - BMvector % AXIALX;

	obj(0,0) = 0.0;

	obj(0,1) = 1.0;
	maxY     =  NJRLinearProgramming::GetObjValue() - BMvector % AXIALY;

	obj(0,1) = -1.0;
	minY     = -NJRLinearProgramming::GetObjValue() - BMvector % AXIALY;

	obj(0,1) = 0.0;

	obj(0,2) = 1.0;
	maxZ     = NJRLinearProgramming::GetObjValue() - BMvector % AXIALZ;

	obj(0,2) = -1.0;

	minZ     = -NJRLinearProgramming::GetObjValue() - BMvector % AXIALZ;

	obj(0,2) = 0.0;

	return true;
};
