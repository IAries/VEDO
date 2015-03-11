#ifndef _DO_QUASIPLATE_H
#define _DO_QUASIPLATE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOQuasiPlate : public DiscreteObject
{

public:

	DOQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

	void Response(vedo_float_t dt);

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

	DOQuasiPlate& operator = (const DOQuasiPlate&);

	DOQuasiPlate(const DOQuasiPlate&);

	DOQuasiPlate();
};

}   // namespace vedo

#endif // _DO_QUASIPLATE_H
