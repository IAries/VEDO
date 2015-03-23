#ifndef _NJR_RANDOM_GENERATOR_H
#define _NJR_RANDOM_GENERATOR_H

#define PRANDMAX 1000000000

#include <njr/DataType.h>

namespace njr
{

class RandomGenerator
{

public:

	RandomGenerator();

	RandomGenerator(_int_t a);

	_int_t GeneratingInteger(_int_t lo, _int_t hi);

	_float_t GeneratingDouble(_float_t lo, _float_t hi);

	_float_t Gaussian(_float_t mean, _float_t stddev);

private:

   static _int_t a;

   static _int_t b;

   static _int_t arr[55];

   static bool init;

   void sprand (_int_t seed);

   _int_t lprand();
};

}   // namespace njr

#endif // _NJR_RANDOM_GENERATOR_H
