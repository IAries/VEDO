#ifndef _GS_CYLINDER_H
#define _GS_CYLINDER_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
{

class GSCylinder: public GeometricShape
{

public:

	GSCylinder(const std::string&, const vedo_float_t&, const vedo_float_t&);

	inline vedo_float_t GetRadius()
	{
		return _dRadius;
	}

	inline vedo_float_t GetHeight()
	{
		return _dHeight;
	}

	bool Inside(const njr::Vector3d&);

protected:

	vedo_float_t _dRadius;

	vedo_float_t _dHeight;
};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSCylinder&);

#endif // _GS_CYLINDER_H
