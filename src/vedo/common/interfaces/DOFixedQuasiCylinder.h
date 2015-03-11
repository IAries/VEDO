#ifndef _DO_FIXEDQUASICYLINDER_H
#define _DO_FIXEDQUASICYLINDER_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOFixedQuasiCylinder : public DiscreteObject
{

public:

	DOFixedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

	inline void Response(vedo_float_t dt)
	{
		ClearImpact();
	}

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

	DOFixedQuasiCylinder& operator = (const DOFixedQuasiCylinder&);

	DOFixedQuasiCylinder(const DOFixedQuasiCylinder&);

	DOFixedQuasiCylinder();

};

}   // namespace vedo

#endif // _DO_FIXEDQUASICYLINDER_H
