#ifndef _DO_ORBITAL_QUASICYLINDER_H
#define _DO_ORBITAL_QUASICYLINDER_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOOrbitalQuasiCylinder : public DiscreteObject
{

public:

	DOOrbitalQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

	void Response(_float_t dt);

	inline _float_t CrossAreaToSurface(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const
	{
		return 0.0;
	}

	inline _float_t GetSudoContactRadius()
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
