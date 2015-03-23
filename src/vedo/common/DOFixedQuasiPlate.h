#ifndef _DO_FIXEDQUASIPLATE_H
#define _DO_FIXEDQUASIPLATE_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOFixedQuasiPlate : public DiscreteObject
{

public:

	DOFixedQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

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
