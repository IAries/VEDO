#ifndef _RICKYRANDOM_H
#define _RICKYRANDOM_H

namespace NJR
{

class Rickyrandom
{

public:

	Rickyrandom ();

	Rickyrandom (int ij, int kl);

	int operator () (int lo, int hi);

	double operator () (double lo, double hi);

	// Initializtion by 2 integer
	void RandomInitialise(int, int);

	// Any possible double
	double RandomUniform(void);

	// Possible double obey normal dist.
	double RandomGaussian(double, double);

	// Possible integer between 2 integers
	int RandomInt(int, int);

	// Possible double between 2 doules
	double RandomDouble(double, double);

private:

	static double u[97];
	static double c;
	static double cd;
	static double cm;
	static int i97;
	static int j97;
	static bool test;

};

};   // namespace NJR

#endif // _RICKYANDOM_H
