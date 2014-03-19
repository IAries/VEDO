#ifndef _GS_ELLIPSOID_H
#define _GS_ELLIPSOID_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
{

class GSEllipsoid : public GeometricShape
{

public:

	GSEllipsoid
		(const std::string&, const double&, const double&, const double&);

	inline double GetXLength()
	{
		return _dXLength;
	};

	inline double GetYLength()
	{
		return _dYLength;
	};

	inline double GetZLength()
	{
		return _dZLength;
	};

	bool Inside(const njr::Vector3d&);

protected:

	double _dXLength;
	double _dYLength;
	double _dZLength;
};

};   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSEllipsoid&);

#endif // _GS_ELLIPSOID_H
