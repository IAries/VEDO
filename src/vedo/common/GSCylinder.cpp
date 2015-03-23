#include <vedo/common/GSCylinder.h>
#include <cmath>

namespace vedo
{

GSCylinder::GSCylinder(const std::string& nm, const _float_t& r, const _float_t& h): _dRadius(r), _dHeight(h)
{
	_sType  = "Cylinder";
	_sName  = nm;
	_Status = new DOStatus("No Name");
}

bool GSCylinder::Inside(const aries::Vector3df& p)
{
	aries::Vector3df localP(p);
	localP = localP - _Status->GetPosition();
	_float_t localX = localP.dot(_Status->GetOrientationX());
	_float_t localY = localP.dot(_Status->GetOrientationZ().cross(_Status->GetOrientationX()));
	_float_t localZ = localP.dot(_Status->GetOrientationZ());
	_float_t localR = sqrt(localX * localX + localY * localY);
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
