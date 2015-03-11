#ifndef _DO_FIXEDSPHERE_H
#define _DO_FIXEDSPHERE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOFixedSphere : public DiscreteObject
{

public:

	DOFixedSphere(const DOStatus* pdos, const DOModel* pdoml);

	inline void Response(vedo_float_t dt)
	{
		ClearImpact();
	}

	vedo_float_t CrossAreaToSurface(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const;

	inline vedo_float_t GetSudoContactRadius()
	{
		return cpDOModel->GetShapeAttributes().sphere.radius;
	}

private:

	DOFixedSphere& operator = (const DOFixedSphere&);

	DOFixedSphere(const DOFixedSphere&);

	DOFixedSphere();
};

}   // namespace vedo

#endif // _DO_FIXEDSPHERE_H
