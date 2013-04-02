#ifndef _GS_COMPLEX_SHAPE_H
#define _GS_COMPLEX_SHAPE_H

#include <NJR/Interfaces/Vector3d.h>
#include <Framework/Interfaces/GeometricShape.h>

namespace vedo
{

class GSComplexShape : public GeometricShape
{

public:

	GSComplexShape(const std::string&);

	bool Inside(const njr::Vector3d&);
};

};   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSComplexShape&);

#endif // _GS_COMPLEX_SHAPE_H
