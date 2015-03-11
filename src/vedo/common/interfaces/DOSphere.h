#ifndef _DOSPHERE_H
#define _DOSPHERE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOSphere : public DiscreteObject
{

public:

	DOSphere(const DOStatus* pdos, const DOModel* pdoml);

	void Response(vedo_float_t dt);

	vedo_float_t CrossAreaToSurface(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const;

	inline vedo_float_t GetSudoContactRadius()
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
