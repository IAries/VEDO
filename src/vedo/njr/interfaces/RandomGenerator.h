#ifndef _NJR_RANDOM_GENERATOR_H
#define _NJR_RANDOM_GENERATOR_H

#define PRANDMAX 1000000000

#include <vedo/constants/interfaces/Constants.h>

namespace njr
{

class RandomGenerator
{

public:

	RandomGenerator();

	RandomGenerator(vedo::vedo_int_t a);

	vedo::vedo_int_t GeneratingInteger(vedo::vedo_int_t lo, vedo::vedo_int_t hi);

	vedo::vedo_float_t GeneratingDouble(vedo::vedo_float_t lo, vedo::vedo_float_t hi);

	vedo::vedo_float_t Gaussian(vedo::vedo_float_t mean, vedo::vedo_float_t stddev);

private:

   static vedo::vedo_int_t a;

   static vedo::vedo_int_t b;

   static vedo::vedo_int_t arr[55];

   static bool init;

   void sprand (vedo::vedo_int_t seed);

   vedo::vedo_int_t lprand();
};

}   // namespace njr

#endif // _NJR_RANDOM_GENERATOR_H
