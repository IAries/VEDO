#ifndef _DO_ORBITAL_QUASIPLATE_H
#define _DO_ORBITAL_QUASIPLATE_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOOrbitalQuasiPlate : public DiscreteObject
{

public:

	DOOrbitalQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

	void Response(_float_t dt);

	inline _float_t CrossAreaToSurface(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const
	{
		return 0.0;
	}

	inline _float_t GetSudoContactRadius()
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
