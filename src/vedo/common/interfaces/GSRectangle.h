#ifndef _GS_RECTANGLE_H
#define _GS_RECTANGLE_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
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

	bool Inside(const njr::Vector3d&);

protected:

	double _dWidth;
	double _dLength;
	double _dHeight;
};

};   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSRectangle&);

#endif // _GS_RECTANGLE_H
