#ifndef _GS_CYLINDER_H
#define _GS_CYLINDER_H

#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/GeometricShape.h>

namespace VEDO
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

	bool Inside(const NJR::NJRvector3d&);

protected:

	double _dRadius;
	double _dHeight;
};

};   // namespace VEDO



std::ostream& operator << (std::ostream&, VEDO::GSCylinder&);

#endif // _GS_CYLINDER_H
