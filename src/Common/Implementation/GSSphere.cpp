#include <Common/Interfaces/GSSphere.h>
#include <cmath>

namespace VEDO
{

GSSphere::GSSphere(const std::string& nm, const double& r): _dRadius(r)
{
	_sType = "Sphere";
	_sName = nm;
	_Status = new DOStatus("No Name");
};


};   // namespace VEDO



std::ostream& operator << (std::ostream& os, VEDO::GSSphere& gs)
{
	std::cout << "Radius: " << gs.GetRadius() << '\n';
	return os;
};
