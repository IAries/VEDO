#ifndef _X_SOLVER_ADJOINT_H
#define _X_SOLVER_ADJOINT_H

#include <aries/utility/XSolver.h>

namespace aries
{

class MatrixFs;
class Matrixf;

class XSAdjoint: public XSolver {

public:

	// {Y} = [A] {X}, give [A] & {Y}, solve {X}
	virtual Matrixf Solve(const Matrixf& A, const Matrixf& Y) const;

	// {Y} = [A] {X}, give [A] & {Y}, solve {X}
	virtual MatrixFs Solve(const MatrixFs& A, const MatrixFs& Y) const;
};

}   //namespace aries

#endif   // _X_SOLVER_ADJOINT_H
