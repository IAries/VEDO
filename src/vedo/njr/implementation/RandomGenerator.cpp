// Returns an uniformly distributed integer in [lo, hi]
#include <vedo/njr/interfaces/RandomGenerator.h>
#include <cmath>

namespace njr
{

vedo::vedo_int_t RandomGenerator::a;
vedo::vedo_int_t RandomGenerator::b;
vedo::vedo_int_t RandomGenerator::arr[55];
bool RandomGenerator::init = false ;

RandomGenerator::RandomGenerator()
{
	if (init == false)
	{
		RandomGenerator::sprand(999);
		init = true;
	}
}

RandomGenerator::RandomGenerator(vedo::vedo_int_t a)
{
	RandomGenerator::sprand(a);
	init = true;
}

vedo::vedo_int_t RandomGenerator::GeneratingInteger(vedo::vedo_int_t lo, vedo::vedo_int_t hi)
{
	vedo::vedo_int_t t;
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

	return ((vedo::vedo_int_t)(lprand()%(hi-lo+1)+lo));
}


vedo::vedo_float_t RandomGenerator::GeneratingDouble(vedo::vedo_float_t lo, vedo::vedo_float_t hi)
{
	vedo::vedo_float_t t ;
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

	return ( (vedo::vedo_float_t)lprand() / PRANDMAX * (hi-lo) + lo);
}

/******************************************************************************
 * RNG Initializer
 * Call once before using lprand
 ******************************************************************************/
void RandomGenerator::sprand(vedo::vedo_int_t seed)
{
	vedo::vedo_int_t i;
	vedo::vedo_int_t ii;
	vedo::vedo_int_t last;
	vedo::vedo_int_t next = 1;

	arr[0] = last = seed;

	for (i=1; i<55; i++)
	{
		ii = (21*i)%55;
		arr[ii] = next;
		next = last - next;

		if (next < 0)
		{
			next += PRANDMAX;
		}

		last = arr[ii];
	}

	a = 0;
	b = 24;

	for (i=0; i<165; i++)
	{
		last = lprand();
	}
}

/******************************************************************************
 * RNG
 * Returns long integers from the range 0 ... PRANDMAX-1
 ******************************************************************************/
vedo::vedo_int_t RandomGenerator::lprand()
{
	vedo::vedo_int_t t;

	if ((a--) == 0)
	{
		a = 54;
	}

	if ((b--) == 0)
	{
		b = 54;
	}

	t = arr[a] - arr[b];

	if (t < 0)
	{
		t += PRANDMAX;
	}

	arr[a] = t;

	return t;

}

/******************************************************************************************************************************
 * ALGORITHM 712, COLLECTED ALGORITHMS FROM ACM. THIS WORK PUBLISHED IN TRANSACTIONS ON MATHEMATICAL SOFTWARE, VOL. 18, NO. 4,
 * DECEMBER, 1992, PP. 434-435. The function returns a normally distributed pseudo-random number with a given mean and standard
 * devaiation.  Calls are made to a function subprogram which must return independent random numbers uniform in the interval
 * (0, 1). The algorithm uses the ratio of uniforms method of A.J. Kinderman and J.F. Monahan augmented with quadratic bounding
 * curves.
 ******************************************************************************************************************************/
vedo::vedo_float_t RandomGenerator::Gaussian(vedo::vedo_float_t mean,vedo::vedo_float_t stddev)
{
   vedo::vedo_float_t  q,u,v,x,y;
	/**************************************************************************************************************************
	 * Generate P = (u,v) uniform in rect. enclosing acceptance region Make ure that any random numbers <= 0 are rejected,
	 * since gaussian() requires uniforms > 0, but RandomUniform() delivers >= 0.
	 **************************************************************************************************************************/

	do
	{
		u = (vedo::vedo_float_t)lprand() / PRANDMAX;
		v = (vedo::vedo_float_t)lprand() / PRANDMAX;

		if ((u <= 0.0) || (v <= 0.0))
		{
       		u = 1.0;
       		v = 1.0;
   		}
		v = 1.7156 * (v-0.5);

		//  Evaluate the quadratic form
		x = u - 0.449871;
		y = std::fabs(v) + 0.386595;
		q = x*x + y * (0.19600*y - 0.25472*x);

		// Accept P if inside inner ellipse
		if (q < 0.27597)
		{
			break;
		}

		// Reject P if outside outer ellipse, or outside acceptance region
	} while ((q>0.27846) || (v*v > (-4.0*std::log(u)*u*u)));

    // Return ratio of P's coordinates as the normal deviate
    return mean + stddev * v / u;
}

}   // namespace njr
