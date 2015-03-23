#include <aries/utility/Vector3df.h>

namespace aries
{

Vector3df::Vector3df()
{
	_v[0] = _v[1] = _v[2] = 0.0;
}

Vector3df::Vector3df(const _float_t& x, const _float_t& y, const _float_t& z)
{
	this->set(x, y, z);
}

Vector3df::Vector3df(const Vector3df& v)
{
	*this = v;
}

Vector3df::Vector3df(const Vector3d<_float_t>& v)
{
	*this = v;
}

const Vector3df& Vector3df::operator = (const Vector3df& v)
{
    memcpy(&_v, &v, 3*sizeof(_float_t));
	return *this;
}

const Vector3df& Vector3df::operator = (const Vector3d<_float_t>& v)
{
    memcpy(&_v, &v, 3*sizeof(_float_t));
	return *this;
}

Vector3df::Vector3df(const ParticularVector& pv)
{
	*this = pv;
}

const Vector3df& Vector3df::operator = (const ParticularVector& pv)
{
	switch (pv)
	{
		case ParticularVector::ORIGIN:
			this->set(0.0, 0.0, 0.0);
			break;
		case ParticularVector::AXIALX:
			this->set(1.0, 0.0, 0.0);
			break;
		case ParticularVector::AXIALY:
			this->set(0.0, 1.0, 0.0);
			break;
		case ParticularVector::AXIALZ:
			this->set(0.0, 0.0, 1.0);
			break;
		case ParticularVector::ZERO:
			this->set(0.0, 0.0, 0.0);
			break;
		default:
			this->set(0.0, 0.0, 0.0);
	}
	return *this;
}

}   // namespace aries



/*
aries::Vector3df operator - (const aries::Vector3df& v)
{
	return aries::Vector3df(-v.x(), -v.y(), -v.z());
}

aries::Vector3df operator * (const aries::_float_t& s , const aries::Vector3df& v)
{
	return aries::Vector3df(s * v.x(), s * v.y(), s * v.z());
}

std::ostream& operator << (std::ostream& os, const aries::Vector3df& v)
{
	os << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')' << std::endl;
	return os;
}

std::istream& operator >> (std::istream& is, aries::Vector3df& v)
{
	aries::_float_t x, y, z;
	is >> x >> y >> z ;
	v.set(x, y, z);
	return is;
}
*/
