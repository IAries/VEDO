#ifndef _GS_COMPLEX_SHAPE_H
#define _GS_COMPLEX_SHAPE_H

#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/GeometricShape.h>

class GSComplexShape : public GeometricShape
{

public:

	GSComplexShape(const std::string&);

	bool Inside(const NJRvector3d&);

protected:

private:

};

#endif // _GS_COMPLEX_SHAPE_H
