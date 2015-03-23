#ifndef _NJR_RICKY_RANDOM_H
#define _NJR_RICKY_RANDOM_H

#include <njr/DataType.h>

namespace njr
{

class Rickyrandom
{
public:

	Rickyrandom ();

	Rickyrandom (_int_t ij, _int_t kl);

	_int_t operator () (_int_t lo, _int_t hi);

	_float_t operator () (_float_t lo, _float_t hi);

	// Initializtion by 2 integer
	void RandomInitialise(_int_t, _int_t);

	// Any possible _float_t
	_float_t RandomUniform(void);

	// Possible _float_t obey normal dist.
	_float_t RandomGaussian(_float_t, _float_t);

	// Possible integer between 2 integers
	_int_t RandomInt(_int_t, _int_t);

	// Possible _float_t between 2 doules
	_float_t RandomDouble(_float_t, _float_t);

private:

	static _float_t u[97];

	static _float_t c;

	static _float_t cd;

	static _float_t cm;

	static _int_t   i97;

	static _int_t   j97;

	static bool test;
};

}   // namespace njr

#endif // _NJR_RICKY_RANDOM_H
