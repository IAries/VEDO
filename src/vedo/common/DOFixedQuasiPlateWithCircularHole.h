#ifndef _DO_FIXEDQUASIPLATEWITHCIRCULARHOLE_H
#define _DO_FIXEDQUASIPLATEWITHCIRCULARHOLE_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOFixedQuasiPlateWithCircularHole : public DiscreteObject
{

public:

	DOFixedQuasiPlateWithCircularHole(const DOStatus* pdos, const DOModel* pdoml);

	inline void Response(_float_t dt)
	{
		ClearImpact();
	}

	inline _float_t CrossAreaToSurface
		(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const
	{
		return 0.0;
	}

	inline _float_t GetSudoContactRadius()
	{
//		return cpDOModel->GetShapeAttributes().quasiplatewithcircularhole.height;
		return 0.0;
	}

private:

	DOFixedQuasiPlateWithCircularHole& operator = (const DOFixedQuasiPlateWithCircularHole&);

	DOFixedQuasiPlateWithCircularHole(const DOFixedQuasiPlateWithCircularHole&);

	DOFixedQuasiPlateWithCircularHole();
};

}   // namespace vedo

#endif // _DO_FIXEDQUASIPLATEWITHCIRCULARHOLE_H
