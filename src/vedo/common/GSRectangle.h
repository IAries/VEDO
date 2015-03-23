#ifndef _GS_RECTANGLE_H
#define _GS_RECTANGLE_H

#include <aries/utility/Vector3d.h>
#include <vedo/framework/GeometricShape.h>

namespace vedo
{

class GSRectangle: public GeometricShape
{

public:

	GSRectangle(const std::string&, const _float_t&, const _float_t&, const _float_t&);

	inline _float_t GetWidth()
	{
		return _dWidth;
	}

	inline _float_t GetLength()
	{
		return _dLength;
	}

	inline _float_t GetHeight()
	{
		return _dHeight;
	}

	bool Inside(const aries::Vector3df&);

protected:

	_float_t _dWidth;

	_float_t _dLength;

	_float_t _dHeight;
};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSRectangle&);

#endif // _GS_RECTANGLE_H
