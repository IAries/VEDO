#include <vedo/common/interfaces/GSRectangle.h>
#include <cmath>

namespace vedo
{

GSRectangle::GSRectangle
	(const std::string& nm, const double& w, const double& l, const double& h):
	_dWidth(w), _dLength(l), _dHeight(h)
{
	_sType = "Rectangle";
	_sName = nm;
	_Status = new DOStatus("No Name");
};

bool GSRectangle::Inside(const njr::Vector3d& p)
{
	njr::Vector3d localP(p);
	localP = localP - _Status->GetPosition();
	double localX = localP % (_Status->GetOrientationX());
	double localY = localP % (_Status->GetOrientationZ() * _Status->GetOrientationX());
	double localZ = localP % (_Status->GetOrientationZ());
	if (   (fabs(localX) <= 0.5*_dWidth)
		&& (fabs(localY) <= 0.5*_dLength)
		&& (fabs(localZ) <= 0.5*_dHeight))
	{
		return true;
	}
	else
	{
		return false;
	}
};

};   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GSRectangle& gs)
{
	std::cout << "Width : " << gs.GetWidth()  << '\n';
	std::cout << "Length: " << gs.GetLength() << '\n';
	std::cout << "Height: " << gs.GetHeight() << '\n';
	return os;
};