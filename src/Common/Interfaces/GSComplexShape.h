#ifndef _GS_COMPLEX_SHAPE_H
#define _GS_COMPLEX_SHAPE_H

#include <NJR/Interfaces/Vector3d.h>
#include <FrameWork/Interfaces/GeometricShape.h>

namespace VEDO
{

class GSComplexShape : public GeometricShape
{

public:

	GSComplexShape(const std::string&);

	bool Inside(const NJR::Vector3d&);
};

};   // namespace VEDO



std::ostream& operator << (std::ostream&, VEDO::GSComplexShape&);

#endif // _GS_COMPLEX_SHAPE_H
