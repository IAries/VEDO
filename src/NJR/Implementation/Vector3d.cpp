#include <NJR/Interfaces/Vector3d.h>
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
};

Vector3d::Vector3d(const double& dx, const double& dy, const double& dz)
{
	this->Set(dx, dy, dz);
};

Vector3d::Vector3d(const Vector3d& vInit)
{
	*this = vInit;
};

Vector3d::Vector3d(const char* cInit)
{
	*this = cInit;
};

Vector3d::Vector3d(const std::string& sInit)
{
	*this = sInit;
};

Vector3d::Vector3d(const njr::ParticularVector& pvInit)
{
	*this = pvInit;
};

double Vector3d::length() const
{
	return ( sqrt((_x*_x) + (_y*_y) + (_z*_z)) );
};

Vector3d Vector3d::direction() const
{
	Vector3d v;
	double  len = this->length();

	if (len != 0)
	{
		v.Set(_x/len, _y/len, _z/len);
	}

	return v;
};

Vector3d Vector3d::RotateAround(const Vector3d& va) const
{
	Vector3d u   = va.direction();
	double   a   = va.length();
	double   s   = cos (0.5*a);
	double   t   = sin (0.5*a);
	Vector3d v   = u*t;
	Vector3d p   = *this;
	Vector3d out = (p*s*s) + (v*(p%v)) + (2.0*s*(v*p)) + (v*(v*p));
	//	NJRvector3d u   = va.direction();
	//	double      a   = va.length();
	//	double      s   = cos (0.5*a);
	//	double      t   = sin (0.5*a);
	//double      cs   = cos (va.length());
	//	double      t   = sin (a);
	//	NJRvector3d v   = u*t;
	//	NJRvector3d p   = (*this);
	//	NJRvector3d out = (p*s*s) + (v*(p%v)) + (2.0*s*(v*p)) + (v*(v*p));
	//	M: another slightly quick rotation method
	//NJRvector3d out = ((*this)*cs) + va.Cross(*this) + 1.0/(1+cs)*va*(va.Dot(*this));

	return out;
};

Vector3d Vector3d::Trans
	(const Vector3d& nx, const Vector3d& ny, const Vector3d& nz) const
{
   return (_x*nx + _y*ny + _z*nz);
};

Vector3d Vector3d::ProjectOn(const Vector3d& v) const
{
	return v.direction() * (( (*this)%v ) / v.length());
};

double Vector3d::Dot(const Vector3d& v) const
{
	return ( (_x*v._x) + (_y*v._y) + (_z*v._z) );
};

Vector3d Vector3d::Cross(const Vector3d& v) const
{
	Vector3d out
		( (_y*v._z)-(_z*v._y), (_z*v._x)-(_x*v._z), (_x*v._y)-(_y*v._x) );
	return out;
};

const Vector3d& Vector3d::operator = (const Vector3d& vAval)
{
	_x = vAval._x;
	_y = vAval._y;
	_z = vAval._z;
	return *this;
};

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
	if ( sscanf (strv,"%lf, %lf, %lf",&_x,&_y,&_z) != 3 )
	{
		std::cerr << "Error!! Code: Vector3d::operator = (const char*)" << std::endl;
		exit(-1);
	}
	return *this;
};

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
	};
	return *this;
};

const Vector3d& Vector3d::operator = (const std::string& sAval)
{
	*this = sAval.c_str();
	return *this;
};

const Vector3d& Vector3d::operator += (const Vector3d& v)
{
	*this = *this + v;
	return *this;
};

const Vector3d& Vector3d::operator -= (const Vector3d& v)
{
	*this = *this - v;
	return *this;
};

const Vector3d& Vector3d::Set
	(const double& dx, const double& dy, const double& dz)
{
	_x = dx;
	_y = dy;
	_z = dz;
	return *this;
};

const Vector3d& Vector3d::SetX(const double& dx)
{
	_x = dx;
	return *this;
};

const Vector3d& Vector3d::SetY(const double& dy)
{
	_y = dy;
	return *this;
};

const Vector3d& Vector3d::SetZ(const double& dz)
{
	_z = dz;
	return *this;
};

const Vector3d& Vector3d::SetCylinder
	(const double& dRadius, const double& dThita, const double& dz)
{
	_x = dRadius * cos(dThita);
	_y = dRadius * sin(dThita);
	_z = dz;
	return *this;
};

const Vector3d& Vector3d::SetSphere
	(const double& dRadius, const double& dAlpha, const double& dThita)
{
	_x = dRadius * sin(dAlpha) * cos(dThita);
	_y = dRadius * sin(dAlpha) * sin(dThita);
	_z = dRadius * cos(dAlpha);
	return * this;
};

Vector3d Vector3d::operator + (const Vector3d& v) const
{
	 Vector3d out ( _x+v._x, _y+v._y, _z+v._z);
	 return out;
};

Vector3d Vector3d::operator - (const Vector3d& v) const
{
	 Vector3d out (_x-v._x, _y-v._y, _z-v._z);
	 return out;
};

Vector3d Vector3d::operator * (const Vector3d& v) const
{
	Vector3d out
		( (_y*v._z)-(_z*v._y), (_z*v._x)-(_x*v._z), (_x*v._y)-(_y*v._x) );
	return out;
};

Vector3d Vector3d::operator * (const double& s) const
{
	Vector3d out(_x*s, _y*s, _z*s);
	return out;
};

double Vector3d::operator % (const Vector3d& v) const
{
	return ( (_x*v._x) + (_y*v._y) + (_z*v._z) );
};

Vector3d Vector3d::operator >> (const Vector3d& v) const
{
	return (this->ProjectOn(v));
};

bool Vector3d::operator == (Vector3d& vR) const
{
	return ( (_x==vR._x) && (_y==vR._y) && (_z==vR._z) );
}

void Vector3d::print() const
{
	std::cout << '(' << _x << ',' << _y << ',' << _z << ")" << std::endl;
};

};   // namespace NJR



njr::Vector3d operator - (const njr::Vector3d& v)
{
	 njr::Vector3d out(-v.x(), -v.y(), -v.z());
	 return out;
};

njr::Vector3d operator * (const double& ds , const njr::Vector3d& v)
{
	 njr::Vector3d out(v.x()*ds, v.y()*ds, v.z()*ds);
	 return out;
};

std::ostream& operator << (std::ostream& os, const njr::Vector3d& v)
{
	os << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')' << std::endl;
	return os;
};

std::istream& operator >> (std::istream& is, njr::Vector3d& v)
{
	double x;
	double y;
	double z;
	is >> x >> y >> z ;
	v.Set(x,y,z);
	return is;
};
