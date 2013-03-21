#ifndef _GS_RECTANGLE_H
#define _GS_RECTANGLE_H

#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/GeometricShape.h>

namespace VEDO
{

class GSRectangle : public GeometricShape
{

public:

	GSRectangle(const std::string&, const double&, const double&, const double&);

	inline double GetWidth()
	{
		return _dWidth;
	};

	inline double GetLength()
	{
		return _dLength;
	};

	inline double GetHeight()
	{
		return _dHeight;
	};

	bool Inside(const NJR::NJRvector3d&);

protected:

	double _dWidth;
	double _dLength;
	double _dHeight;
};

};   // namespace VEDO



std::ostream& operator << (std::ostream&, VEDO::GSRectangle&);

#endif // _GS_RECTANGLE_H
