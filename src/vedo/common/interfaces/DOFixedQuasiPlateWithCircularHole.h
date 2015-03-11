#ifndef _DO_FIXEDQUASIPLATEWITHCIRCULARHOLE_H
#define _DO_FIXEDQUASIPLATEWITHCIRCULARHOLE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOFixedQuasiPlateWithCircularHole : public DiscreteObject
{

public:

	DOFixedQuasiPlateWithCircularHole(const DOStatus* pdos, const DOModel* pdoml);

	inline void Response(vedo_float_t dt)
	{
		ClearImpact();
	}

	inline vedo_float_t CrossAreaToSurface
		(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const
	{
		return 0.0;
	}

	inline vedo_float_t GetSudoContactRadius()
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
