#ifndef _DO_QUASICYLINDER_H
#define _DO_QUASICYLINDER_H

#include <FrameWork/Interfaces/DiscreteObject.h>

namespace VEDO
{

class DOQuasiCylinder : public DiscreteObject
{

public:

	DOQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

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

	DOQuasiCylinder& operator = (const DOQuasiCylinder&);

	DOQuasiCylinder(const DOQuasiCylinder&);

	DOQuasiCylinder();

};

};   // namespace VEDO

#endif // _DO_QUASICYLINDER_H
