#ifndef _ARIES_LINE_3D_FLOAT_H
#define _ARIES_LINE_3D_FLOAT_H

#include <aries/utility/DataType.h>
#include <aries/utility/Vector3df.h>
#include <aries/utility/Line3d.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace aries
{

class Line3df: public Line3d<_float_t>
{

public:

	Line3df(const Vector3df&, const Vector3df&);

	Line3df(const Vector3df& center, const Vector3df& direction, const _float_t& length);

	Line3df(const Line3df&);

	const Line3df& operator = (const Line3df&);

	bool OnLine(const Vector3df& point) const;

private:

	Line3df();

};

Line3df CommonPerpendicular(const Line3df& L1, const Line3df& L2);

}   // namespace aries



#endif   // _ARIES_LINE_3D_FLOAT_H
