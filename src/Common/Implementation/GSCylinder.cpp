#include <Common/Interfaces/GSCylinder.h>
#include <cmath>

using namespace std;

GSCylinder::GSCylinder(const string& nm, const double& r, const double& h):
	_dRadius(r), _dHeight(h)
{
	_sType = "Cylinder";
	_sName = nm;
	_Status = new DOStatus("No Name");
};

bool GSCylinder::Inside(const NJRvector3d& p)
{
	NJRvector3d localP(p);
	localP = localP - _Status->GetPosition();
	double localX = localP % (_Status->GetOrientationX());
	double localY = localP % (_Status->GetOrientationZ() * _Status->GetOrientationX());
	double localZ = localP % (_Status->GetOrientationZ());
	double localR = sqrt(localX * localX + localY * localY);
	if ((localR <= _dRadius) && (fabs(localZ) <= 0.5*_dHeight))
	{
		return true;
	}
	else
	{
		return false;
	}
};

ostream& operator << (ostream& os, GSCylinder& gs)
{
	cout << "Radius: " << gs.GetRadius() << '\n';
	cout << "Height: " << gs.GetHeight() << '\n';
	return os;
};
