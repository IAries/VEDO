#include <vedo/njr/interfaces/RickyRandom.h>
#include <cmath>

namespace njr
{

vedo::vedo_float_t Rickyrandom::u[97];
vedo::vedo_float_t Rickyrandom::c;
vedo::vedo_float_t Rickyrandom::cd;
vedo::vedo_float_t Rickyrandom::cm;
vedo::vedo_int_t   Rickyrandom::i97;
vedo::vedo_int_t   Rickyrandom::j97;
bool               Rickyrandom::test = false;

Rickyrandom::Rickyrandom()
{
	if ( test == false)
	{
		Rickyrandom::RandomInitialise(2003, 1225);
		test = true;
	}
}

Rickyrandom::Rickyrandom(vedo::vedo_int_t ij,vedo::vedo_int_t kl)
{
	if (test == false)
	{
		Rickyrandom::RandomInitialise(ij,kl);
		test = true;
	}
}

vedo::vedo_int_t Rickyrandom::operator() (vedo::vedo_int_t lo, vedo::vedo_int_t hi)
{
	vedo::vedo_int_t t ;
	if (lo > hi)
	{
		t  = lo;
		lo = hi;
		hi = t;
	}
	else if (lo == hi)
	{
		return hi;
	}

	return RandomInt(lo, hi);
}

vedo::vedo_float_t Rickyrandom::operator () (vedo::vedo_float_t lo, vedo::vedo_float_t hi)
{
	vedo::vedo_float_t t;

	if (lo > hi)
	{
		t  = lo;
		lo = hi;
		hi = t;
	}
	else if (lo == hi)
	{
		return(hi);
	}

	return RandomDouble(lo, hi);
}


void Rickyrandom::RandomInitialise(vedo::vedo_int_t ij, vedo::vedo_int_t kl)
{
	vedo::vedo_float_t s;
	vedo::vedo_float_t t;
	vedo::vedo_int_t   ii;
	vedo::vedo_int_t   i;
	vedo::vedo_int_t   j;
	vedo::vedo_int_t   k;
	vedo::vedo_int_t   l;
	vedo::vedo_int_t   jj;
	vedo::vedo_int_t   m;

	/*******************************************************
	 * Handle the seed range errors
	 * First random number seed must be between 0 and 31328
	 * Second seed must have a value between 0 and 30081
	 *******************************************************/
	if ((ij<0) || (ij>31328) || (kl<0) || (kl>30081))
	{
		ij = 1802;
		kl = 9373;
	}

	i = (ij/177)%177+2;
	j = (ij%177)    +2;
	k = (kl/169)%178+1;
	l = (kl%169);

	for (ii=0; ii<97; ii++)
	{
		s = 0.0;
		t = 0.5;
		for (jj=0; jj<24; jj++)
		{
			m = (((i*j)%179)*k)%179;
			i = j;
			j = k;
			k = m;
			l = (53*l+1)%169;
			if (((l*m%64)) >= 32)
			{
				s += t;
			}
		 	t *= 0.5;
		}
		u[ii] = s;
   }

   c    =   362436.0 / 16777216.0;
   cd   =  7654321.0 / 16777216.0;
   cm   = 16777213.0 / 16777216.0;
   i97  =       97;
   j97  =       33;

   test = true;
}

// This is the random number generator proposed by George Marsaglia in Florida State University Report: FSU-SCRI-87-50
vedo::vedo_float_t Rickyrandom::RandomUniform(void)
{
	vedo::vedo_float_t uni;

	// Make sure the initialisation routine has been called
	if (!test)
	{
		RandomInitialise(1802, 9373);
	}

	uni = u[i97-1] - u[j97-1];

	if (uni <= 0.0)
	{
		uni++;
	}

	u[i97-1] = uni;

	i97--;
	if (i97 == 0)
	{
		i97 = 97;
	}

	j97--;
	if (j97 == 0)
	{
		j97 = 97;
	}

	c -= cd;
	if (c < 0.0)
	{
		c += cm;
	}

	uni -= c;
	if (uni < 0.0)
	{
		uni++;
	}

	return(uni);
}

/******************************************************************************************************************************
 * ALGORITHM 712, COLLECTED ALGORITHMS FROM ACM. THIS WORK PUBLISHED IN TRANSACTIONS ON MATHEMATICAL SOFTWARE, VOL. 18, NO. 4,
 * DECEMBER, 1992, PP. 434-435. The function returns a normally distributed pseudo-random number with a given mean and standard
 * devaiation.  Calls are made to a function subprogram which must return independent random numbers uniform in the interval
 * (0, 1). The algorithm uses the ratio of uniforms method of A.J. Kinderman and J.F. Monahan augmented with quadratic bounding
 * curves.
 ******************************************************************************************************************************/
vedo::vedo_float_t Rickyrandom::RandomGaussian(vedo::vedo_float_t mean,vedo::vedo_float_t stddev)
{
	vedo::vedo_float_t q;
	vedo::vedo_float_t u;
	vedo::vedo_float_t v;
	vedo::vedo_float_t x;
	vedo::vedo_float_t y;

	/**************************************************************************************************************************
	 * Generate P = (u,v) uniform in rect. enclosing acceptance region. Make sure that any random numbers <= 0 are rejected,
	 * since gaussian() requires uniforms > 0, but RandomUniform() delivers >= 0.
	 **************************************************************************************************************************/
	do
	{
		u = RandomUniform();
		v = RandomUniform();
		if ( (u<=0.0) || (v<=0.0) )
		{
			u = 1.0;
			v = 1.0;
		}
		v = 1.7156*(v-0.5);

		// Evaluate the quadratic form
		x = u-0.449871;
		y = fabs(v)+0.386595;
		q = x*x+y*(0.19600*y-0.25472*x);

		// Accept P if inside inner ellipse
		if (q < 0.27597)
		{
			break;
		}

		// Reject P if outside outer ellipse, or outside acceptance region
	} while ( (q>0.27846) || ((v*v) > (-4.0*log(u)*u*u)) );

	// Return ratio of P's coordinates as the normal deviate
	return mean + stddev * v / u;
}

// Return random integer within a range, lower->upper INCLUSIVE
vedo::vedo_int_t Rickyrandom::RandomInt(vedo::vedo_int_t lower,vedo::vedo_int_t upper)
{
   return (vedo::vedo_int_t)(RandomUniform()*(upper-lower+1)) + lower;
}

// Return random float within a range, lower->upper
vedo::vedo_float_t Rickyrandom::RandomDouble(vedo::vedo_float_t lower,vedo::vedo_float_t upper)
{
   return (upper - lower) * RandomUniform() + lower;
}

}   // namespace njr
