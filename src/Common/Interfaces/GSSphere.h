#ifndef _GS_SPHERE_H
#define _GS_SPHERE_H

#include <NJR/Interfaces/vector3d.h>
#include <FrameWork/Interfaces/GeometricShape.h>

namespace VEDO
{

class GSSphere : public GeometricShape
{

public:

	GSSphere(const std::string&, const double&);

	inline double GetRadius() const
	{
		return _dRadius;
	};

	inline bool Inside(const NJR::NJRvector3d& p)
	{
		return ((p - _Status->GetPosition()).length() <= _dRadius);
	};

protected:

	double _dRadius;

private:

};

};   // namespace VEDO



std::ostream& operator << (std::ostream&, VEDO::GSSphere&);

#endif // _GS_SPHERE_H
