#ifndef _DO_CONSTRAINEDQUASIPLATE_H
#define _DO_CONSTRAINEDQUASIPLATE_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOConstrainedQuasiPlate : public DiscreteObject
{

public:

	DOConstrainedQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

	void Response(_float_t dt);

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

	DOConstrainedQuasiPlate& operator = (const DOConstrainedQuasiPlate&);

	DOConstrainedQuasiPlate(const DOConstrainedQuasiPlate&);

	DOConstrainedQuasiPlate();
};

}   // namespace vedo

#endif // _DO_CONSTRAINEDQUASIPLATE_H
