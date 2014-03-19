#ifndef _GS_CYLINDER_H
#define _GS_CYLINDER_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
{

class GSCylinder : public GeometricShape
{

public:

	GSCylinder(const std::string&, const double&, const double&);

	inline double GetRadius()
	{
		return _dRadius;
	};

	inline double GetHeight()
	{
		return _dHeight;
	};

	bool Inside(const njr::Vector3d&);

protected:

	double _dRadius;
	double _dHeight;
};

};   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSCylinder&);

#endif // _GS_CYLINDER_H
