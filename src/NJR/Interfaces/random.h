#ifndef _NJRRANDOM_H
#define _NJRRANDOM_H

#define PRANDMAX 1000000000

namespace NJR
{

class RandomGenerator
{

public:

	RandomGenerator();

	RandomGenerator(long a);

	int operator () (int lo, int hi);

	double operator () (double lo, double hi);

	double Gaussian(double mean, double stddev);

private:

   static int a;
   static int b;
   static long arr[55];
   static bool init;
   void sprand (long seed);
   long lprand();

};

};   // namespace NJR

#endif // _NJRRANDOM_H
