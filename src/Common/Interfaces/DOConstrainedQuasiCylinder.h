#ifndef _DO_CONSTRAINEDQUASICYLINDER_H
#define _DO_CONSTRAINEDQUASICYLINDER_H

#include <FrameWork/Interfaces/DiscreteObject.h>

namespace VEDO
{

class DOConstrainedQuasiCylinder : public DiscreteObject
{

public:

	DOConstrainedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

	void Response(double dt);

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

	DOConstrainedQuasiCylinder& operator = (const DOConstrainedQuasiCylinder&);

	DOConstrainedQuasiCylinder(const DOConstrainedQuasiCylinder&);

	DOConstrainedQuasiCylinder();

};

};   // namespace VEDO

#endif // _DO_CONSTRAINEDQUASICYLINDER_H
