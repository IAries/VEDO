#include <Common/Interfaces/GSCylinder.h>
#include <cmath>

namespace VEDO
{

GSCylinder::GSCylinder(const std::string& nm, const double& r, const double& h):
	_dRadius(r), _dHeight(h)
{
	_sType = "Cylinder";
	_sName = nm;
	_Status = new DOStatus("No Name");
};

bool GSCylinder::Inside(const NJR::NJRvector3d& p)
{
	NJR::NJRvector3d localP(p);
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


};   // namespace VEDO



std::ostream& operator << (std::ostream& os, VEDO::GSCylinder& gs)
{
	std::cout << "Radius: " << gs.GetRadius() << '\n';
	std::cout << "Height: " << gs.GetHeight() << '\n';
	return os;
};
