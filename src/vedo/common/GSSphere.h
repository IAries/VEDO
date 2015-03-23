#ifndef _GS_SPHERE_H
#define _GS_SPHERE_H

#include <aries/utility/Vector3d.h>
#include <vedo/framework/GeometricShape.h>

namespace vedo
{

class GSSphere : public GeometricShape
{

public:

	GSSphere(const std::string&, const _float_t&);

	inline _float_t GetRadius() const
	{
		return _dRadius;
	}

	inline bool Inside(const aries::Vector3df& p)
	{
		return ((p - _Status->GetPosition()).length() <= _dRadius);
	}

protected:

	_float_t _dRadius;

private:

};

}   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::GSSphere&);

#endif // _GS_SPHERE_H
