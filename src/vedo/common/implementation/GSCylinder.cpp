#include <vedo/common/interfaces/GSCylinder.h>
#include <cmath>

namespace vedo
{

GSCylinder::GSCylinder(const std::string& nm, const vedo_float_t& r, const vedo_float_t& h): _dRadius(r), _dHeight(h)
{
	_sType  = "Cylinder";
	_sName  = nm;
	_Status = new DOStatus("No Name");
}

bool GSCylinder::Inside(const njr::Vector3d& p)
{
	njr::Vector3d localP(p);
	localP = localP - _Status->GetPosition();
	vedo_float_t localX = localP % (_Status->GetOrientationX());
	vedo_float_t localY = localP % (_Status->GetOrientationZ() * _Status->GetOrientationX());
	vedo_float_t localZ = localP % (_Status->GetOrientationZ());
	vedo_float_t localR = sqrt(localX * localX + localY * localY);
	if ((localR <= _dRadius) && (fabs(localZ) <= 0.5*_dHeight))
	{
		return true;
	}
	else
	{
		return false;
	}
}

}   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GSCylinder& gs)
{
	std::cout << "Radius: " << gs.GetRadius() << '\n';
	std::cout << "Height: " << gs.GetHeight() << '\n';
	return os;
}
