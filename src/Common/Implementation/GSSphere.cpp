#include <Common/Interfaces/GSSphere.h>
#include <cmath>

using namespace std;

GSSphere::GSSphere(const string& nm, const double& r): _dRadius(r)
{
	_sType = "Sphere";
	_sName = nm;
	_Status = new DOStatus("No Name");
};

ostream& operator << (ostream& os, GSSphere& gs)
{
	cout << "Radius: " << gs.GetRadius() << '\n';
	return os;
};
