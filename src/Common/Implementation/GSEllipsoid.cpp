#include <Common/Interfaces/GSEllipsoid.h>
#include <cmath>

namespace VEDO
{

GSEllipsoid::GSEllipsoid
	(const std::string& nm, const double& xl, const double& yl, const double& zl):
	_dXLength(xl), _dYLength(yl), _dZLength(zl)
{
	_sType = "Ellipsoid";
	_sName = nm;
	_Status = new DOStatus("No Name");
};

bool GSEllipsoid::Inside(const NJR::Vector3d& p)
{
	// Aries: Need to be modified.
	//NJR::Vector3d localP(p);
	//localP = localP - _Status->GetPosition();
	//double localX = localP % (_Status->GetOrientationX());
	//double localY = localP % (_Status->GetOrientationZ() * _Status->GetOrientationX());
	//double localZ = localP % (_Status->GetOrientationZ());
	return true;
};

};   // namespace VEDO



std::ostream& operator << (std::ostream& os, VEDO::GSEllipsoid& gs)
{
	std::cout << "XLength: " << gs.GetXLength() << '\n';
	std::cout << "YLength: " << gs.GetYLength() << '\n';
	std::cout << "ZLength: " << gs.GetZLength() << '\n';
	return os;
};
