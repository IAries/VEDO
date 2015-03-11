#include <vedo/framework/interfaces/ImpactSolver.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace vedo
{

ImpactSolver::ImpactSolver(const IactModel* cpIactml) : ImStatus()
{
	cpIactModel = cpIactml;
	pBC         = 0;
}

ImpactSolver::~ImpactSolver()
{
}

}   // namespace vedo
