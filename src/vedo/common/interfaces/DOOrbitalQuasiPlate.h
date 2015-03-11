#ifndef _DO_ORBITAL_QUASIPLATE_H
#define _DO_ORBITAL_QUASIPLATE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOOrbitalQuasiPlate : public DiscreteObject
{

public:

	DOOrbitalQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

	void Response(vedo_float_t dt);

	inline vedo_float_t CrossAreaToSurface(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const
	{
		return 0.0;
	}

	inline vedo_float_t GetSudoContactRadius()
	{
//		return cpDOModel->GetShapeAttributes().quasiplate.height;
		return 0.0;
	}

private:

	DOOrbitalQuasiPlate& operator = (const DOOrbitalQuasiPlate&);

	DOOrbitalQuasiPlate(const DOOrbitalQuasiPlate&);

	DOOrbitalQuasiPlate();
};

}   // namespace vedo

#endif // _DO_ORBITAL_QUASIPLATE_H
