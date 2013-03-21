#include <NJR/Interfaces/vector2d.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace NJR
{

NJRvector2d::NJRvector2d()
{
	*this = NJRDXF::ORIGIN;
};

NJRvector2d::NJRvector2d(const double& dx, const double& dy)
{
	this->Set(dx, dy);
};

NJRvector2d::NJRvector2d(const NJRvector2d& vInit)
{
	*this = vInit;
};

NJRvector2d::NJRvector2d(const char* cInit)
{
	*this = cInit;
};

NJRvector2d::NJRvector2d(const std::string& sInit)
{
	*this = sInit;
};

NJRvector2d::NJRvector2d(const NJRDXF::ParticularVector& pvInit)
{
	*this = pvInit;
};

double NJRvector2d::length() const
{
	return ( sqrt((_x*_x) + (_y*_y)) );
};

NJRvector2d NJRvector2d::direction() const
{
	NJRvector2d v;
	double  len = this->length();

	if (len != 0)
	{
		v.Set(_x/len, _y/len);
	}

	return v;
};

/*
NJRvector2d NJRvector2d::RotateAround(const NJRvector2d& va) const
{
	NJRvector2d u   = va.direction();
	double      a   = va.length();
	double      s   = cos (0.5*a);
	double      t   = sin (0.5*a);
	NJRvector2d v   = u*t;
	NJRvector2d p   = *this;
	NJRvector2d out = (p*s*s) + (v*(p%v)) + (2.0*s*(v*p)) + (v*(v*p));
	return out;
};
*/

NJRvector2d NJRvector2d::Trans
	(const NJRvector2d& nx, const NJRvector2d& ny) const
{
   return (_x*nx + _y*ny);
};

NJRvector2d NJRvector2d::ProjectOn(const NJRvector2d& v) const
{
	return v.direction() * (( (*this)%v ) / v.length());
};

double NJRvector2d::Dot(const NJRvector2d& v) const
{
	return ( (_x*v._x) + (_y*v._y) );
};

const NJRvector2d& NJRvector2d::operator = (const NJRvector2d& vAval)
{
	_x = vAval._x;
	_y = vAval._y;
	return *this;
};

const NJRvector2d& NJRvector2d::operator = (const char* cInit)
{
	char strv[256];
	strcpy(strv, cInit);
	if ( (strv[0] != '(') || (strv[strlen(strv) -1 ] != ')') )
	{
		std::cerr << "Error!! Code: NJRvector2d::operator = (const char*)" << std::endl;
		exit(-1);
	}

	strv[0] = ' ';
	strv[ strlen(strv) -1] = ' ';
	if ( sscanf (strv,"%lf, %lf",&_x,&_y) != 2 )
	{
		std::cerr << "Error!! Code: NJRvector2d::operator = (const char*)" << std::endl;
		exit(-1);
	}
	return *this;
};

const NJRvector2d& NJRvector2d::operator = (const NJRDXF::ParticularVector& pvAval)
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

const NJRvector2d& NJRvector2d::operator = (const std::string& sAval)
{
	*this = sAval.c_str();
	return *this;
};

const NJRvector2d& NJRvector2d::operator += (const NJRvector2d& v)
{
	*this = *this + v;
	return *this;
};

const NJRvector2d& NJRvector2d::operator -= (const NJRvector2d& v)
{
	*this = *this - v;
	return *this;
};

const NJRvector2d& NJRvector2d::Set
	(const double& dx, const double& dy)
{
	_x = dx;
	_y = dy;
	return *this;
};

const NJRvector2d& NJRvector2d::SetX(const double& dx)
{
	_x = dx;
	return *this;
};

const NJRvector2d& NJRvector2d::SetY(const double& dy)
{
	_y = dy;
	return *this;
};

const NJRvector2d& NJRvector2d::SetPolar
	(const double& dRadius, const double& dThita)
{
	_x = dRadius * cos(dThita);
	_y = dRadius * sin(dThita);
	return *this;
};

NJRvector2d NJRvector2d::operator + (const NJRvector2d& v) const
{
	 NJRvector2d out ( _x+v._x, _y+v._y);
	 return out;
};

NJRvector2d NJRvector2d::operator - (const NJRvector2d& v) const
{
	 NJRvector2d out (_x-v._x, _y-v._y);
	 return out;
};

NJRvector2d NJRvector2d::operator * (const double& s) const
{
	NJRvector2d out(_x*s, _y*s);
	return out;
};

double NJRvector2d::operator % (const NJRvector2d& v) const
{
	return ( (_x*v._x) + (_y*v._y) );
};

NJRvector2d NJRvector2d::operator >> (const NJRvector2d& v) const
{
	return (this->ProjectOn(v));
};

bool NJRvector2d::operator == (NJRvector2d& vR) const
{
	return ( (_x==vR._x) && (_y==vR._y) );
}

void NJRvector2d::print() const
{
	std::cout << '(' << _x << ", " << _y << ")" << std::endl;
};

};   // namespace NJR



NJR::NJRvector2d operator * (const double& ds, const NJR::NJRvector2d& v)
{
	 NJR::NJRvector2d out(v.x()*ds, v.y()*ds);
	 return out;
};

NJR::NJRvector2d operator - (const NJR::NJRvector2d& v)
{
	 NJR::NJRvector2d out(-v.x(), -v.y());
	 return out;
};

std::ostream& operator << (std::ostream& os, const NJR::NJRvector2d& v)
{
	os << '(' << v.x() << ", " << v.y() << ")" << std::endl;
	return os;
};

std::istream& operator >> (std::istream& is, NJR::NJRvector2d& v)
{
	double x;
	double y;
	is >> x >> y;
	v.Set(x,y);
	return is;
};
