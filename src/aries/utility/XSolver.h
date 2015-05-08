#ifndef _X_SOLVER_H
#define _X_SOLVER_H

//#include <aries/utility/Matrixf.h>
//#include <aries/utility/Vector3df.h>
#include <aries/utility/DataType.h>

namespace aries
{

class MatrixFs;
class Matrixf;

class XSolver
{

public:

	// {Y} = [A] {X}, give [A] & {Y}, solve {X}
	virtual Matrixf Solve(const Matrixf& A, const Matrixf& Y) const = 0;

	// {Y} = [A] {X}, give [A] & {Y}, solve {X}
	virtual MatrixFs Solve(const MatrixFs& A, const MatrixFs& Y) const = 0;
};

}   //namespace aries

#endif   // _X_SOLVER_H
