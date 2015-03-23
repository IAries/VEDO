#ifndef _DOSPHERE_H
#define _DOSPHERE_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOSphere : public DiscreteObject
{

public:

	DOSphere(const DOStatus* pdos, const DOModel* pdoml);

	void Response(_float_t dt);

	_float_t CrossAreaToSurface(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const;

	inline _float_t GetSudoContactRadius()
	{
		return cpDOModel->GetShapeAttributes().sphere.radius;
	}

private:

	DOSphere& operator = (const DOSphere&);

	DOSphere(const DOSphere&);

	DOSphere();
};

}   // namespace vedo

#endif // _DOSPHERE_H
