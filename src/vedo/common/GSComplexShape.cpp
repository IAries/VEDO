#include <vedo/common/GSComplexShape.h>
#include <cmath>

namespace vedo
{

bool GSComplexShape::Inside(const aries::Vector3df& p)
{
	std::list<GeometricShape*>::iterator _lgsp;
	for (_lgsp=_ListGS.begin(); _lgsp!=_ListGS.end(); _lgsp++)
	{
		if (!(*_lgsp)->Inside(p))
		{
			return false;
		}
	}
	return true;
}

}   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GSComplexShape& gs)
{
	return os;
}
