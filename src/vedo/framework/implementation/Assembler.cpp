#include <vedo/framework/interfaces/Assembler.h>

namespace vedo
{

Assembler::Assembler()
{
};

DiscreteObject* Assembler::CreateDiscreteObject
	(const DOModel* cpdoml, const DOStatus* cpdos) const
{
	return dofd.Create(cpdoml, cpdos);
};

Interaction* Assembler::CreateInteraction
	(DiscreteObject* pdo1, DiscreteObject* pdo2, const IactModel* cpiactml) const
{
  	DiscreteObject  *pDOslave;
	DiscreteObject  *pDOmaster;
	ImpactSolver    *pIS;
	ContactDetector *pCD;

	if (cpiactml == 0)
	{
		return 0;
	}

	if ((pdo1 == 0) || (pdo2 == 0) || (pdo1 == pdo2))
	{
		return 0;
	}

	if ( (pdo2->GetDOModel()->GetDensity() < 0.0) ||
		(pdo1->GetDOModel()->GetDensity() < 0.0)    )
	{
		return 0;
	}

	if ( (pdo2->GetDOModel()->GetDensity() == 0.0) &&
		(pdo1->GetDOModel()->GetDensity() == 0.0)    )
	{
		return 0;
	}

	if (  ((int) pdo1->GetDOModel()->GetShapeType())
		> ((int) pdo2->GetDOModel()->GetShapeType()) )
	{
		pDOslave  = pdo2;
		pDOmaster = pdo1;
	}
	else if
		( ((int) (pdo1->GetDOModel()->GetShapeType()))
		< ((int) (pdo2->GetDOModel()->GetShapeType())) )
	{
		pDOslave  = pdo1;
		pDOmaster = pdo2;
	}
	else if ( (pdo1->GetDOModel()->GetDensity()) == 0.0)
	{
		pDOslave  = pdo2;
		pDOmaster = pdo1;
	}
	else if ( (pdo2->GetDOModel()->GetDensity()) == 0.0)
	{
		pDOslave  = pdo1;
		pDOmaster = pdo2;
	}
	else
	{
		pDOslave  = pdo1;
		pDOmaster = pdo2;
	}

	pCD = cdfd.Create(pDOslave, pDOmaster, cpiactml);
	pIS = isfd.Create(pDOslave, pDOmaster, cpiactml);

	if ( (pCD == 0) || (pIS == 0) )
	{
		if( pCD != 0 )
		{
			delete pCD;
		}

		if( pIS != 0)
		{
			delete pIS;
		}

		return 0;
	}
	return new Interaction (pDOslave, pDOmaster, pCD, pIS);
};

};   // namespace vedo
