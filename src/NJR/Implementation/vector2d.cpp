#include <NJR/Interfaces/Vector2d.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace NJR
{

Vector2d::Vector2d()
{
	*this = NJRDXF::ORIGIN;
};

Vector2d::Vector2d(const double& dx, const double& dy)
{
	this->Set(dx, dy);
};

Vector2d::Vector2d(const Vector2d& vInit)
{
	*this = vInit;
};

Vector2d::Vector2d(const char* cInit)
{
	*this = cInit;
};

Vector2d::Vector2d(const std::string& sInit)
{
	*this = sInit;
};

Vector2d::Vector2d(const NJRDXF::ParticularVector& pvInit)
{
	*this = pvInit;
};

double Vector2d::length() const
{
	return ( sqrt((_x*_x) + (_y*_y)) );
};

Vector2d Vector2d::direction() const
{
	Vector2d v;
	double  len = this->length();

	if (len != 0)
	{
		v.Set(_x/len, _y/len);
	}

	return v;
};

/*
Vector2d Vector2d::RotateAround(const Vector2d& va) const
{
	Vector2d u   = va.direction();
	double      a   = va.length();
	double      s   = cos (0.5*a);
	double      t   = sin (0.5*a);
	Vector2d v   = u*t;
	Vector2d p   = *this;
	Vector2d out = (p*s*s) + (v*(p%v)) + (2.0*s*(v*p)) + (v*(v*p));
	return out;
};
*/

Vector2d Vector2d::Trans
	(const Vector2d& nx, const Vector2d& ny) const
{
   return (_x*nx + _y*ny);
};

Vector2d Vector2d::ProjectOn(const Vector2d& v) const
{
	return v.direction() * (( (*this)%v ) / v.length());
};

double Vector2d::Dot(const Vector2d& v) const
{
	return ( (_x*v._x) + (_y*v._y) );
};

const Vector2d& Vector2d::operator = (const Vector2d& vAval)
{
	_x = vAval._x;
	_y = vAval._y;
	return *this;
};

const Vector2d& Vector2d::operator = (const char* cInit)
{
	char strv[256];
	strcpy(strv, cInit);
	if ( (strv[0] != '(') || (strv[strlen(strv) -1 ] != ')') )
	{
		std::cerr << "Error!! Code: Vector2d::operator = (const char*)" << std::endl;
		exit(-1);
	}

	strv[0] = ' ';
	strv[ strlen(strv) -1] = ' ';
	if ( sscanf (strv,"%lf, %lf",&_x,&_y) != 2 )
	{
		std::cerr << "Error!! Code: Vector2d::operator = (const char*)" << std::endl;
		exit(-1);
	}
	return *this;
};

const Vector2d& Vector2d::operator = (const NJRDXF::ParticularVector& pvAval)
{
	switch (pvAval)
	{
		case NJRDXF::ORIGIN:
			this->Set(0.0, 0.0);
			break;
		case NJRDXF::AXIALX:
			this->Set(1.0, 0.0);
			break;
		case NJRDXF::AXIALY:
			this->Set(0.0, 1.0);
			break;
		case NJRDXF::ZERO:
			this->Set(0.0, 0.0);
			break;
	};
	return *this;
};

const Vector2d& Vector2d::operator = (const std::string& sAval)
{
	*this = sAval.c_str();
	return *this;
};

const Vector2d& Vector2d::operator += (const Vector2d& v)
{
	*this = *this + v;
	return *this;
};

const Vector2d& Vector2d::operator -= (const Vector2d& v)
{
	*this = *this - v;
	return *this;
};

const Vector2d& Vector2d::Set
	(const double& dx, const double& dy)
{
	_x = dx;
	_y = dy;
	return *this;
};

const Vector2d& Vector2d::SetX(const double& dx)
{
	_x = dx;
	return *this;
};

const Vector2d& Vector2d::SetY(const double& dy)
{
	_y = dy;
	return *this;
};

const Vector2d& Vector2d::SetPolar
	(const double& dRadius, const double& dThita)
{
	_x = dRadius * cos(dThita);
	_y = dRadius * sin(dThita);
	return *this;
};

Vector2d Vector2d::operator + (const Vector2d& v) const
{
	 Vector2d out ( _x+v._x, _y+v._y);
	 return out;
};

Vector2d Vector2d::operator - (const Vector2d& v) const
{
	 Vector2d out (_x-v._x, _y-v._y);
	 return out;
};

Vector2d Vector2d::operator * (const double& s) const
{
	Vector2d out(_x*s, _y*s);
	return out;
};

double Vector2d::operator % (const Vector2d& v) const
{
	return ( (_x*v._x) + (_y*v._y) );
};

Vector2d Vector2d::operator >> (const Vector2d& v) const
{
	return (this->ProjectOn(v));
};

bool Vector2d::operator == (Vector2d& vR) const
{
	return ( (_x==vR._x) && (_y==vR._y) );
}

void Vector2d::print() const
{
	std::cout << '(' << _x << ", " << _y << ")" << std::endl;
};

};   // namespace NJR



NJR::Vector2d operator * (const double& ds, const NJR::Vector2d& v)
{
	 NJR::Vector2d out(v.x()*ds, v.y()*ds);
	 return out;
};

NJR::Vector2d operator - (const NJR::Vector2d& v)
{
	 NJR::Vector2d out(-v.x(), -v.y());
	 return out;
};

std::ostream& operator << (std::ostream& os, const NJR::Vector2d& v)
{
	os << '(' << v.x() << ", " << v.y() << ")" << std::endl;
	return os;
};

std::istream& operator >> (std::istream& is, NJR::Vector2d& v)
{
	double x;
	double y;
	is >> x >> y;
	v.Set(x,y);
	return is;
};
