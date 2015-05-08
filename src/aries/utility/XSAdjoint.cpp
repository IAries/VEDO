#include <aries/utility/XSAdjoint.h>
#include <aries/utility/MatrixFs.h>
#include <aries/utility/Matrixf.h>

namespace aries
{

/*
void XSAdjoint::operator = (const XSAdjoint& xs)
{
	if (this != &xs)
	{
		*this = xs;
	}
}
*/

Matrixf XSAdjoint::Solve(const Matrixf& A, const Matrixf& Y) const
{
	if (A.rows() == A.columns())
	{
		return A.inverse() * Y;
	}
	else
	{
		std::cerr
			<< "Error!! Code: Matrixf aries::utility::XSAdjoint::Solve(const Matrixf&, const Matrixf&)" << std::endl
			<< "        Note: Nonsquare matrix cannot bo solved."                                       << std::endl;
		exit(-1);
	}
}

MatrixFs XSAdjoint::Solve(const MatrixFs& A, const MatrixFs& Y) const
{
	if (A.rows() == A.columns())
	{
		return A.inverse() * Y;
	}
	else
	{
		std::cerr
			<< "Error!! Code: MatrixFs aries::utility::XSAdjoint::Solve(const MatrixFs&, const MatrixFs&)" << std::endl
			<< "        Note: Nonsquare matrix cannot bo solved."                                          << std::endl;
		exit(-1);
	}
}

}   //namespace aries
