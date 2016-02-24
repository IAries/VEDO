#ifndef _DO_FIXEDQUASICYLINDER_H
#define _DO_FIXEDQUASICYLINDER_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOFixedQuasiCylinder : public DiscreteObject
{

public:

	DOFixedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

	inline void Response(_float_t dt)
	{
		ClearImpact();
	}

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

	DOFixedQuasiCylinder& operator = (const DOFixedQuasiCylinder&);

	DOFixedQuasiCylinder(const DOFixedQuasiCylinder&);

	DOFixedQuasiCylinder();

};

}   // namespace vedo

#endif // _DO_FIXEDQUASICYLINDER_H
