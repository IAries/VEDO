#ifndef _DO_QUASIPLATEWITHCIRCULARHOLE_H
#define _DO_QUASIPLATEWITHCIRCULARHOLE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOQuasiPlateWithCircularHole : public DiscreteObject
{

public:

	DOQuasiPlateWithCircularHole(const DOStatus* pdos, const DOModel* pdoml);

	void Response(vedo_float_t dt);

	inline vedo_float_t CrossAreaToSurface(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const
	{
		return 0.0;
	}

	inline vedo_float_t GetSudoContactRadius()
	{
//		return cpDOModel->GetShapeAttributes().quasiplatewithcircularhole.height;
		return 0.0;
	};

private:

	DOQuasiPlateWithCircularHole& operator = (const DOQuasiPlateWithCircularHole&);

	DOQuasiPlateWithCircularHole(const DOQuasiPlateWithCircularHole&);

	DOQuasiPlateWithCircularHole();
};

}   // namespace vedo

#endif // _DO_QUASIPLATEWITHCIRCULARHOLE_H
