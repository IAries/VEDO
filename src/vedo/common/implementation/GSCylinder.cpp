#include <vedo/common/interfaces/GSCylinder.h>
#include <cmath>

namespace vedo
{

GSCylinder::GSCylinder(const std::string& nm, const double& r, const double& h):
	_dRadius(r), _dHeight(h)
{
	_sType = "Cylinder";
	_sName = nm;
	_Status = new DOStatus("No Name");
};

bool GSCylinder::Inside(const njr::Vector3d& p)
{
	njr::Vector3d localP(p);
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


};   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GSCylinder& gs)
{
	std::cout << "Radius: " << gs.GetRadius() << '\n';
	std::cout << "Height: " << gs.GetHeight() << '\n';
	return os;
};
