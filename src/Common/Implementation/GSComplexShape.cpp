#include <Common/Interfaces/GSComplexShape.h>
#include <cmath>

namespace VEDO
{

bool GSComplexShape::Inside(const NJR::Vector3d& p)
{
	std::list<GeometricShape*>::iterator _lgsp;
	for (_lgsp=_ListGS.begin(); _lgsp!=_ListGS.end(); _lgsp++)
	{
		if(!(*_lgsp)->Inside(p))
		{
			return false;
		}
	}
	return true;
};

};   // namespace VEDO



std::ostream& operator << (std::ostream& os, VEDO::GSComplexShape& gs)
{
	return os;
};
