#ifndef _ARIES_VECTOR_3D_FLOAT_H
#define _ARIES_VECTOR_3D_FLOAT_H

#include <aries/utility/Vector3d.h>

namespace aries
{

// Predefine Praticular Vectors
#ifdef _STD_CPP_11
enum ParticularVector: _uint_t
#else
enum ParticularVector
#endif
{
	ORIGIN = 0x0000,
	AXIALX = 0x0001,
	AXIALY = 0x0002,
	AXIALZ = 0x0004,
	ZERO   = 0x0010,
};



class Vector3df: public Vector3d<_float_t>
{

public:

	Vector3df();

	Vector3df(const _float_t& x, const _float_t& y, const _float_t& z);

	Vector3df(const Vector3df&);

	Vector3df(const Vector3d<_float_t>& v);

	const Vector3df& operator = (const Vector3df& v);

	const Vector3df& operator = (const Vector3d<_float_t>& v);

	Vector3df(const ParticularVector& pv);

	const Vector3df& operator = (const ParticularVector& pv);
};

}   // namespace aries



/*
aries::Vector3df operator - (const aries::Vector3df&);

aries::Vector3df operator * (const aries::_float_t&, const aries::Vector3df&);

std::ostream& operator << (std::ostream&, const aries::Vector3df&);

std::istream& operator >> (std::istream&, aries::Vector3df&);
*/

#endif   // _ARIES_VECTOR_3D_FLOAT_H
