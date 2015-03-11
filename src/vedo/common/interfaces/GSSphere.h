#ifndef _GS_SPHERE_H
#define _GS_SPHERE_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/GeometricShape.h>

namespace vedo
{

class GSSphere : public GeometricShape
{

public:

	GSSphere(const std::string&, const vedo_float_t&);

	inline vedo_float_t GetRadius() const
	{
		return _dRadius;
	}

	inline bool Inside(const njr::Vector3d& p)
	{
		return ((p - _Status->GetPosition()).length() <= _dRadius);
	}

protected:

	vedo_float_t _dRadius;

private:

};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSSphere&);

#endif // _GS_SPHERE_H
