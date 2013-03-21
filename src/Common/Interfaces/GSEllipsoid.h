#ifndef _GS_ELLIPSOID_H
#define _GS_ELLIPSOID_H

#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/GeometricShape.h>

namespace VEDO
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

	bool Inside(const NJR::NJRvector3d&);

protected:

	double _dXLength;
	double _dYLength;
	double _dZLength;
};

};   // namespace VEDO



std::ostream& operator << (std::ostream&, VEDO::GSEllipsoid&);

#endif // _GS_ELLIPSOID_H
