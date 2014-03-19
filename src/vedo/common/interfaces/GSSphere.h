#ifndef _GS_SPHERE_H
#define _GS_SPHERE_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
{

class GSSphere : public GeometricShape
{

public:

	GSSphere(const std::string&, const double&);

	inline double GetRadius() const
	{
		return _dRadius;
	};

	inline bool Inside(const njr::Vector3d& p)
	{
		return ((p - _Status->GetPosition()).length() <= _dRadius);
	};

protected:

	double _dRadius;

private:

};

};   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSSphere&);

#endif // _GS_SPHERE_H
