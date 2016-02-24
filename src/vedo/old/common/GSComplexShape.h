#ifndef _GS_COMPLEX_SHAPE_H
#define _GS_COMPLEX_SHAPE_H

#include <aries/utility/Vector3d.h>
#include <vedo/framework/GeometricShape.h>

namespace vedo
{

class GSComplexShape : public GeometricShape
{

public:

	GSComplexShape(const std::string&);

	bool Inside(const Vector3df&);
};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSComplexShape&);

#endif // _GS_COMPLEX_SHAPE_H
