#include <Common/Interfaces/GSEllipsoid.h>
#include <cmath>

using namespace std;

GSEllipsoid::GSEllipsoid
	(const string& nm, const double& xl, const double& yl, const double& zl):
	_dXLength(xl), _dYLength(yl), _dZLength(zl)
{
	_sType = "Ellipsoid";
	_sName = nm;
	_Status = new DOStatus("No Name");
};

bool GSEllipsoid::Inside(const NJRvector3d& p)
{
	// Aries: Need to be modified.
	//NJRvector3d localP(p);
	//localP = localP - _Status->GetPosition();
	//double localX = localP % (_Status->GetOrientationX());
	//double localY = localP % (_Status->GetOrientationZ() * _Status->GetOrientationX());
	//double localZ = localP % (_Status->GetOrientationZ());
	return true;
};

ostream& operator << (ostream& os, GSEllipsoid& gs)
{
	cout << "XLength: " << gs.GetXLength() << '\n';
	cout << "YLength: " << gs.GetYLength() << '\n';
	cout << "ZLength: " << gs.GetZLength() << '\n';
	return os;
};
