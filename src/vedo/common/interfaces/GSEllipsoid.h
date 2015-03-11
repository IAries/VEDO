#ifndef _GS_ELLIPSOID_H
#define _GS_ELLIPSOID_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
{

class GSEllipsoid: public GeometricShape
{

public:

	GSEllipsoid
		(const std::string&, const vedo_float_t&, const vedo_float_t&, const vedo_float_t&);

	inline vedo_float_t GetXLength()
	{
		return _dXLength;
	}

	inline vedo_float_t GetYLength()
	{
		return _dYLength;
	}

	inline vedo_float_t GetZLength()
	{
		return _dZLength;
	}

	bool Inside(const njr::Vector3d&);

protected:

	vedo_float_t _dXLength;

	vedo_float_t _dYLength;

	vedo_float_t _dZLength;
};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSEllipsoid&);

#endif // _GS_ELLIPSOID_H
