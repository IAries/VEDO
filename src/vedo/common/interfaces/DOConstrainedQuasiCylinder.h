#ifndef _DO_CONSTRAINEDQUASICYLINDER_H
#define _DO_CONSTRAINEDQUASICYLINDER_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOConstrainedQuasiCylinder : public DiscreteObject
{

public:

	DOConstrainedQuasiCylinder(const DOStatus* cpdos, const DOModel* cpdoml);

	void Response(vedo_float_t dt);

	inline vedo_float_t CrossAreaToSurface(vedo_float_t& a, vedo_float_t& b, vedo_float_t& c, vedo_float_t& d) const
	{
		return 0.0;
	}

	inline vedo_float_t GetSudoContactRadius()
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
