#include <vedo/common/GSSphere.h>
#include <cmath>

namespace vedo
{

GSSphere::GSSphere(const std::string& nm, const _float_t& r): _dRadius(r)
{
	_sType = "Sphere";
	_sName = nm;
	_Status = new DOStatus("No Name");
}

}   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::GSSphere& gs)
{
	std::cout << "Radius: " << gs.GetRadius() << '\n';
	return os;
}
