#include <FrameWork/Interfaces/ImpactSolver.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace VEDO
{

ImpactSolver::ImpactSolver(const IactModel* cpIactml) : ImStatus()
{
	cpIactModel = cpIactml;
};

ImpactSolver::~ImpactSolver()
{
};

};   // namespace VEDO
