#ifndef _DO_FIXEDQUASIPLATE_H
#define _DO_FIXEDQUASIPLATE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOFixedQuasiPlate : public DiscreteObject
{

public:

	DOFixedQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

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
//		return cpDOModel->GetShapeAttributes().quasiplate.height;
		return 0.0;
	}

private:

	DOFixedQuasiPlate& operator = (const DOFixedQuasiPlate&);

	DOFixedQuasiPlate(const DOFixedQuasiPlate&);

	DOFixedQuasiPlate();
};

}   // namespace vedo

#endif // _DO_FIXEDQUASIPLATE_H
