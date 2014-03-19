#ifndef _DO_ORBITAL_QUASICYLINDER_H
#define _DO_ORBITAL_QUASICYLINDER_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOOrbitalQuasiCylinder : public DiscreteObject
{

public:

	DOOrbitalQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

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

	DOOrbitalQuasiCylinder& operator = (const DOOrbitalQuasiCylinder&);

	DOOrbitalQuasiCylinder(const DOOrbitalQuasiCylinder&);

	DOOrbitalQuasiCylinder();

};

};   // namespace vedo

#endif // _DO_ORBITAL_QUASICYLINDER_H
