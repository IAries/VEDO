#ifndef _DO_FIXEDSPHERE_H
#define _DO_FIXEDSPHERE_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOFixedSphere : public DiscreteObject
{

public:

	DOFixedSphere(const DOStatus* pdos, const DOModel* pdoml);

	inline void Response(_float_t dt)
	{
		ClearImpact();
	}

	_float_t CrossAreaToSurface(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const;

	inline _float_t GetSudoContactRadius()
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
