#ifndef _X_SOLVER_H
#define _X_SOLVER_H

//#include <aries/utility/Matrixf.h>
//#include <aries/utility/Vector3df.h>

namespace aries
{

class Matrixf;

class XSolver
{

public:

	// {Y} = [A] {X}, give [A] & {Y}, solve {X}
	virtual Matrixf Solve(const Matrixf& A, const Matrixf& Y) const = 0;
};

}   //namespace aries

#endif   // _X_SOLVER_H
