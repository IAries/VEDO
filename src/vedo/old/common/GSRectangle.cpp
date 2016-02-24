#include <vedo/common/GSRectangle.h>
#include <cmath>

namespace vedo
{

GSRectangle::GSRectangle(const std::string& nm, const _float_t& w, const _float_t& l, const _float_t& h):
	_dWidth(w), _dLength(l), _dHeight(h)
{
	_sType  = "Rectangle";
	_sName  = nm;
	_Status = new DOStatus("No Name");
}

bool GSRectangle::Inside(const Vector3df& p)
{
	Vector3df localP(p);
	              localP = localP - _Status->GetPosition();
	_float_t  localX = localP.dot(_Status->GetOrientationX());
	_float_t  localY = localP.dot(_Status->GetOrientationZ().cross(_Status->GetOrientationX()));
	_float_t  localZ = localP.dot(_Status->GetOrientationZ());
	if ((fabs(localX) <= 0.5*_dWidth) && (fabs(localY) <= 0.5*_dLength) && (fabs(localZ) <= 0.5*_dHeight))
	{
		return true;
	}
	else
	{
		return false;
	}
}

}   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GSRectangle& gs)
{
	std::cout << "Width : " << gs.GetWidth()  << '\n';
	std::cout << "Length: " << gs.GetLength() << '\n';
	std::cout << "Height: " << gs.GetHeight() << '\n';
	return os;
}
