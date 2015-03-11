#ifndef _DO_ORBITAL_QUASICYLINDER_H
#define _DO_ORBITAL_QUASICYLINDER_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOOrbitalQuasiCylinder : public DiscreteObject
{

public:

	DOOrbitalQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

	void Response(vedo_float_t dt);

	inline vedo_float_t CrossAreaToSurface(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const
	{
		return 0.0;
	}

	inline vedo_float_t GetSudoContactRadius()
	{
		return cpDOModel->GetShapeAttributes().quasicylinder.radius;
//		return 0.0;
	}

private:

	DOOrbitalQuasiCylinder& operator = (const DOOrbitalQuasiCylinder&);

	DOOrbitalQuasiCylinder(const DOOrbitalQuasiCylinder&);

	DOOrbitalQuasiCylinder();

};

}   // namespace vedo

#endif // _DO_ORBITAL_QUASICYLINDER_H
