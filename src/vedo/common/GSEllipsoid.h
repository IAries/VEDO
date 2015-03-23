#ifndef _GS_ELLIPSOID_H
#define _GS_ELLIPSOID_H

#include <aries/utility/Vector3d.h>
#include <vedo/framework/GeometricShape.h>

namespace vedo
{

class GSEllipsoid: public GeometricShape
{

public:

	GSEllipsoid
		(const std::string&, const _float_t&, const _float_t&, const _float_t&);

	inline _float_t GetXLength()
	{
		return _dXLength;
	}

	inline _float_t GetYLength()
	{
		return _dYLength;
	}

	inline _float_t GetZLength()
	{
		return _dZLength;
	}

	bool Inside(const aries::Vector3df&);

protected:

	_float_t _dXLength;

	_float_t _dYLength;

	_float_t _dZLength;
};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSEllipsoid&);

#endif // _GS_ELLIPSOID_H
