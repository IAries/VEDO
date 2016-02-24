#ifndef _DO_QUASIPLATEWITHCIRCULARHOLE_H
#define _DO_QUASIPLATEWITHCIRCULARHOLE_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOQuasiPlateWithCircularHole : public DiscreteObject
{

public:

	DOQuasiPlateWithCircularHole(const DOStatus* pdos, const DOModel* pdoml);

	void Response(_float_t dt);

	inline _float_t CrossAreaToSurface(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const
	{
		return 0.0;
	}

	inline _float_t GetSudoContactRadius()
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
