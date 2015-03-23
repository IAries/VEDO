#ifndef _DO_CONSTRAINEDQUASICYLINDER_H
#define _DO_CONSTRAINEDQUASICYLINDER_H

#include <vedo/framework/DiscreteObject.h>

namespace vedo
{

class DOConstrainedQuasiCylinder : public DiscreteObject
{

public:

	DOConstrainedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

	void Response(_float_t dt);

	inline _float_t CrossAreaToSurface(_float_t& a, _float_t& b, _float_t& c, _float_t& d) const
	{
		return 0.0;
	}

	inline _float_t GetSudoContactRadius()
	{
		return cpDOModel->GetShapeAttributes().quasicylinder.radius;
//		return 0.0;
	}

private:

	DOConstrainedQuasiCylinder& operator = (const DOConstrainedQuasiCylinder&);

	DOConstrainedQuasiCylinder(const DOConstrainedQuasiCylinder&);

	DOConstrainedQuasiCylinder();

};

}   // namespace vedo

#endif // _DO_CONSTRAINEDQUASICYLINDER_H
