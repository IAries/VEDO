#ifndef _NJR_RICKY_RANDOM_H
#define _NJR_RICKY_RANDOM_H

#include <vedo/constants/interfaces/Constants.h>

namespace njr
{

class Rickyrandom
{
public:

	Rickyrandom ();

	Rickyrandom (vedo::vedo_int_t ij, vedo::vedo_int_t kl);

	vedo::vedo_int_t operator () (vedo::vedo_int_t lo, vedo::vedo_int_t hi);

	vedo::vedo_float_t operator () (vedo::vedo_float_t lo, vedo::vedo_float_t hi);

	// Initializtion by 2 integer
	void RandomInitialise(vedo::vedo_int_t, vedo::vedo_int_t);

	// Any possible vedo::vedo_float_t
	vedo::vedo_float_t RandomUniform(void);

	// Possible vedo::vedo_float_t obey normal dist.
	vedo::vedo_float_t RandomGaussian(vedo::vedo_float_t, vedo::vedo_float_t);

	// Possible integer between 2 integers
	vedo::vedo_int_t RandomInt(vedo::vedo_int_t, vedo::vedo_int_t);

	// Possible vedo::vedo_float_t between 2 doules
	vedo::vedo_float_t RandomDouble(vedo::vedo_float_t, vedo::vedo_float_t);

private:

	static vedo::vedo_float_t u[97];

	static vedo::vedo_float_t c;

	static vedo::vedo_float_t cd;

	static vedo::vedo_float_t cm;

	static vedo::vedo_int_t   i97;

	static vedo::vedo_int_t   j97;

	static bool test;
};

}   // namespace njr

#endif // _NJR_RICKY_RANDOM_H
