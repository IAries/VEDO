#include <vedo/njr/interfaces/Vector3d.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace njr
{

Vector3d::Vector3d()
{
	*this = ORIGIN;
}

Vector3d::Vector3d(const vedo::vedo_float_t& dx, const vedo::vedo_float_t& dy, const vedo::vedo_float_t& dz)
{
	this->Set(dx, dy, dz);
}

Vector3d::Vector3d(const Vector3d& vInit)
{
	*this = vInit;
}

/*
Vector3d::Vector3d(const char* cInit)
{
	*this = cInit;
}
*/

/*
Vector3d::Vector3d(const std::string& sInit)
{
	*this = sInit;
}
*/

Vector3d::Vector3d(const njr::ParticularVector& pvInit)
{
	*this = pvInit;
}

vedo::vedo_float_t Vector3d::length() const
{
	return sqrt((_v[0]*_v[0]) + (_v[1]*_v[1]) + (_v[2]*_v[2]));
}

Vector3d Vector3d::direction() const
{
	Vector3d v;
	vedo::vedo_float_t  len = this->length();

	if (len != 0)
	{
		v.Set(_v[0]/len, _v[1]/len, _v[2]/len);
	}

	return v;
}

bool Vector3d::normalized()
{
	vedo::vedo_float_t len = this->length();
	if (len != 0)
	{
		_v[0] /= len;
		_v[1] /= len;
		_v[2] /= len;
		return true;
	}
	else
	{
		return false;
	}
}

Vector3d Vector3d::RotateAround(const Vector3d& va) const
{
	Vector3d           u   = va.direction();
	vedo::vedo_float_t a   = va.length();
	vedo::vedo_float_t s   = cos (0.5*a);
	vedo::vedo_float_t t   = sin (0.5*a);
	Vector3d           v   = u*t;
	Vector3d           p   = *this;
	Vector3d           out = (p*s*s) + (v*(p%v)) + (2.0*s*(v*p)) + (v*(v*p));
	//	NJRvector3d u   = va.direction();
	//	vedo::vedo_float_t      a   = va.length();
	//	vedo::vedo_float_t      s   = cos (0.5*a);
	//	vedo::vedo_float_t      t   = sin (0.5*a);
	//vedo::vedo_float_t      cs   = cos (va.length());
	//	vedo::vedo_float_t      t   = sin (a);
	//	NJRvector3d v   = u*t;
	//	NJRvector3d p   = (*this);
	//	NJRvector3d out = (p*s*s) + (v*(p%v)) + (2.0*s*(v*p)) + (v*(v*p));
	//	M: another slightly quick rotation method
	//NJRvector3d out = ((*this)*cs) + va.Cross(*this) + 1.0/(1+cs)*va*(va.Dot(*this));

	return out;
}

Vector3d Vector3d::Trans(const Vector3d& nx, const Vector3d& ny, const Vector3d& nz) const
{
   return (_v[0]*nx + _v[1]*ny + _v[2]*nz);
}

Vector3d Vector3d::ProjectOn(const Vector3d& v) const
{
	return v.direction() * (( (*this)%v ) / v.length());
}

vedo::vedo_float_t Vector3d::Dot(const Vector3d& v) const
{
	return ( (_v[0]*v._v[0]) + (_v[1]*v._v[1]) + (_v[2]*v._v[2]) );
}

Vector3d Vector3d::Cross(const Vector3d& v) const
{
	Vector3d out((_v[1]*v._v[2])-(_v[2]*v._v[1]), (_v[2]*v._v[0])-(_v[0]*v._v[2]), (_v[0]*v._v[1])-(_v[1]*v._v[0]));
	return out;
}

const Vector3d& Vector3d::operator = (const Vector3d& vAval)
{
    memcpy(&_v, &vAval, 3*sizeof(vedo::vedo_float_t));
	return *this;
}

/*
const Vector3d& Vector3d::operator = (const char* cInit)
{
	char strv[256];
	strcpy (strv, cInit);
	if ( (strv[0] != '(') || (strv[strlen(strv) -1 ] != ')') )
	{
		std::cerr << "Error!! Code: Vector3d::operator = (const char*)" << std::endl;
		exit(-1);
	}

	strv[0] = ' ';
	strv[ strlen(strv) -1] = ' ';
	if ( sscanf (strv,"%lf, %lf, %lf",&_v[0],&_v[1],&_v[2]) != 3 )
	{
		std::cerr << "Error!! Code: Vector3d::operator = (const char*)" << std::endl;
		exit(-1);
	}
	return *this;
}
*/

const Vector3d& Vector3d::operator = (const njr::ParticularVector& pvAval)
{
	switch (pvAval)
	{
		case ORIGIN:
			this->Set(0.0, 0.0, 0.0);
			break;
		case AXIALX:
			this->Set(1.0, 0.0, 0.0);
			break;
		case AXIALY:
			this->Set(0.0, 1.0, 0.0);
			break;
		case AXIALZ:
			this->Set(0.0, 0.0, 1.0);
			break;
		case ZERO:
			this->Set(0.0, 0.0, 0.0);
			break;
		default:
			this->Set(0.0, 0.0, 0.0);
	}
	return *this;
}

/*
const Vector3d& Vector3d::operator = (const std::string& sAval)
{
	*this = sAval.c_str();
	return *this;
}
*/

const Vector3d& Vector3d::operator += (const Vector3d& v)
{
	*this = *this + v;
	return *this;
}

const Vector3d& Vector3d::operator -= (const Vector3d& v)
{
	*this = *this - v;
	return *this;
}

const Vector3d& Vector3d::Set(const vedo::vedo_float_t& dx, const vedo::vedo_float_t& dy, const vedo::vedo_float_t& dz)
{
	_v[0] = dx;
	_v[1] = dy;
	_v[2] = dz;
	return *this;
}

const Vector3d& Vector3d::SetX(const vedo::vedo_float_t& dx)
{
	_v[0] = dx;
	return *this;
}

const Vector3d& Vector3d::SetY(const vedo::vedo_float_t& dy)
{
	_v[1] = dy;
	return *this;
}

const Vector3d& Vector3d::SetZ(const vedo::vedo_float_t& dz)
{
	_v[2] = dz;
	return *this;
}

const Vector3d& Vector3d::SetCylinder
	(const vedo::vedo_float_t& dRadius, const vedo::vedo_float_t& dThita, const vedo::vedo_float_t& dz)
{
	_v[0] = dRadius * cos(dThita);
	_v[1] = dRadius * sin(dThita);
	_v[2] = dz;
	return *this;
}

const Vector3d& Vector3d::SetSphere
	(const vedo::vedo_float_t& dRadius, const vedo::vedo_float_t& dAlpha, const vedo::vedo_float_t& dThita)
{
	_v[0] = dRadius * sin(dAlpha) * cos(dThita);
	_v[1] = dRadius * sin(dAlpha) * sin(dThita);
	_v[2] = dRadius * cos(dAlpha);
	return * this;
}

Vector3d Vector3d::operator + (const Vector3d& v) const
{
	 Vector3d out ( _v[0]+v._v[0], _v[1]+v._v[1], _v[2]+v._v[2]);
	 return out;
}

Vector3d Vector3d::operator - (const Vector3d& v) const
{
	 Vector3d out (_v[0]-v._v[0], _v[1]-v._v[1], _v[2]-v._v[2]);
	 return out;
}

Vector3d Vector3d::operator * (const Vector3d& v) const
{
	Vector3d out((_v[1]*v._v[2])-(_v[2]*v._v[1]), (_v[2]*v._v[0])-(_v[0]*v._v[2]), (_v[0]*v._v[1])-(_v[1]*v._v[0]));
	return out;
}

Vector3d Vector3d::operator * (const vedo::vedo_float_t& s) const
{
	Vector3d out(_v[0]*s, _v[1]*s, _v[2]*s);
	return out;
}

vedo::vedo_float_t Vector3d::operator % (const Vector3d& v) const
{
	return ( (_v[0]*v._v[0]) + (_v[1]*v._v[1]) + (_v[2]*v._v[2]) );
}

Vector3d Vector3d::operator >> (const Vector3d& v) const
{
	return (this->ProjectOn(v));
}

bool Vector3d::operator == (Vector3d& vR) const
{
	return ((_v[0]==vR._v[0]) && (_v[1]==vR._v[1]) && (_v[2]==vR._v[2]));
}

void Vector3d::print() const
{
	std::cout << '(' << _v[0] << ',' << _v[1] << ',' << _v[2] << ")" << std::endl;
}

}   // namespace njr



njr::Vector3d operator - (const njr::Vector3d& v)
{
	 njr::Vector3d out(-v.x(), -v.y(), -v.z());
	 return out;
}

njr::Vector3d operator * (const vedo::vedo_float_t& ds , const njr::Vector3d& v)
{
	 njr::Vector3d out(v.x()*ds, v.y()*ds, v.z()*ds);
	 return out;
}

std::ostream& operator << (std::ostream& os, const njr::Vector3d& v)
{
	os << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')' << std::endl;
	return os;
}

std::istream& operator >> (std::istream& is, njr::Vector3d& v)
{
	vedo::vedo_float_t x;
	vedo::vedo_float_t y;
	vedo::vedo_float_t z;
	is >> x >> y >> z ;
	v.Set(x,y,z);
	return is;
}
