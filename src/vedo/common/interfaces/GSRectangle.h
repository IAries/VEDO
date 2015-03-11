#ifndef _GS_RECTANGLE_H
#define _GS_RECTANGLE_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
{

class GSRectangle: public GeometricShape
{

public:

	GSRectangle(const std::string&, const vedo_float_t&, const vedo_float_t&, const vedo_float_t&);

	inline vedo_float_t GetWidth()
	{
		return _dWidth;
	}

	inline vedo_float_t GetLength()
	{
		return _dLength;
	}

	inline vedo_float_t GetHeight()
	{
		return _dHeight;
	}

	bool Inside(const njr::Vector3d&);

protected:

	vedo_float_t _dWidth;

	vedo_float_t _dLength;

	vedo_float_t _dHeight;
};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSRectangle&);

#endif // _GS_RECTANGLE_H
