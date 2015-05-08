#ifndef _GS_CYLINDER_H
#define _GS_CYLINDER_H

#include <aries/utility/Vector3d.h>
#include <vedo/framework/GeometricShape.h>

namespace vedo
{

class GSCylinder: public GeometricShape
{

public:

	GSCylinder(const std::string&, const _float_t&, const _float_t&);

	inline _float_t GetRadius()
	{
		return _dRadius;
	}

	inline _float_t GetHeight()
	{
		return _dHeight;
	}

	bool Inside(const Vector3df&);

protected:

	_float_t _dRadius;

	_float_t _dHeight;
};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSCylinder&);

#endif // _GS_CYLINDER_H
