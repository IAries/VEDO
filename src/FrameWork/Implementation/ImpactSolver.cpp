#include <FrameWork/Interfaces/ImpactSolver.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace vedo
{

ImpactSolver::ImpactSolver(const IactModel* cpIactml) : ImStatus()
{
	cpIactModel = cpIactml;
};

ImpactSolver::~ImpactSolver()
{
};

};   // namespace vedo
