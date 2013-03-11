#include <Common/Interfaces/GSComplexShape.h>
#include <cmath>

using namespace std;

bool GSComplexShape::Inside(const NJRvector3d& p)
{
	list<GeometricShape*>::iterator _lgsp;
	for (_lgsp=_ListGS.begin(); _lgsp!=_ListGS.end(); _lgsp++)
	{
		if(!(*_lgsp)->Inside(p))
		{
			return false;
		}
	}
	return true;
};

ostream& operator << (ostream& os, GSComplexShape& gs)
{
	return os;
};
