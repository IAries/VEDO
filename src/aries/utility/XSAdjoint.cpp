#include <aries/utility/XSAdjoint.h>
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

Matrixf XSAdjoint::Solve(const Matrixf& smA, const Matrixf& nsmB) const
{
	if (smA.rows() == smA.columns())
	{
		return smA.inverse() * nsmB;
	}
	else
	{
		std::cerr
			<< "Error!! Code: Matrixf aries::utility::XSAdjoint::Solve(const Matrixf&, const Matrixf&)" << std::endl
			<< "        Note: Nonsquare matrix cannot bo solved."                                       << std::endl;
		exit(-1);
	}
}

}   //namespace aries
