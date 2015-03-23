#include <vedo/common/GSEllipsoid.h>
#include <cmath>

namespace vedo
{

GSEllipsoid::GSEllipsoid(const std::string& nm, const _float_t& xl, const _float_t& yl, const _float_t& zl):
	_dXLength(xl), _dYLength(yl), _dZLength(zl)
{
	_sType  = "Ellipsoid";
	_sName  = nm;
	_Status = new DOStatus("No Name");
}

bool GSEllipsoid::Inside(const aries::Vector3df& p)
{
	// Aries: Need to be modified.
	//aries::Vector3df localP(p);
	//localP = localP - _Status->GetPosition();
	//_float_t localX = localP % (_Status->GetOrientationX());
	//_float_t localY = localP % (_Status->GetOrientationZ() * _Status->GetOrientationX());
	//_float_t localZ = localP % (_Status->GetOrientationZ());
	return true;
}

}   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GSEllipsoid& gs)
{
	std::cout << "XLength: " << gs.GetXLength() << '\n';
	std::cout << "YLength: " << gs.GetYLength() << '\n';
	std::cout << "ZLength: " << gs.GetZLength() << '\n';
	return os;
}
