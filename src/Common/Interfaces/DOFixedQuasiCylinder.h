#ifndef _DO_FIXEDQUASICYLINDER_H
#define _DO_FIXEDQUASICYLINDER_H

#include <FrameWork/Interfaces/DiscreteObject.h>

class DOFixedQuasiCylinder : public DiscreteObject
{

public:

	DOFixedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

	inline void Response(double dt)
	{
		ClearImpact();
	}

	inline double CrossAreaToSurface
		(double& a, double& b, double& c, double& d) const
	{
		return 0.0;
	};

	inline double GetSudoContactRadius()
	{
		return cpDOModel->GetShapeAttributes().quasicylinder.radius;
//		return 0.0;
	};

private:

	DOFixedQuasiCylinder& operator = (const DOFixedQuasiCylinder&);

	DOFixedQuasiCylinder(const DOFixedQuasiCylinder&);

	DOFixedQuasiCylinder();

};

#endif // _DO_FIXEDQUASICYLINDER_H
