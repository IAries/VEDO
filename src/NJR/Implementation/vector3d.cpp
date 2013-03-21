#include <NJR/Interfaces/vector3d.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace NJR
{

NJRvector3d::NJRvector3d()
{
	*this = NJRDXF::ORIGIN;
};

NJRvector3d::NJRvector3d(const double& dx, const double& dy, const double& dz)
{
	this->Set(dx, dy, dz);
};

NJRvector3d::NJRvector3d(const NJRvector3d& vInit)
{
	*this = vInit;
};

NJRvector3d::NJRvector3d(const char* cInit)
{
	*this = cInit;
};

NJRvector3d::NJRvector3d(const std::string& sInit)
{
	*this = sInit;
};

NJRvector3d::NJRvector3d(const NJRDXF::ParticularVector& pvInit)
{
	*this = pvInit;
};

double NJRvector3d::length() const
{
	return ( sqrt((_x*_x) + (_y*_y) + (_z*_z)) );
};

NJRvector3d NJRvector3d::direction() const
{
	NJRvector3d v;
	double  len = this->length();

	if (len != 0)
	{
		v.Set(_x/len, _y/len, _z/len);
	}

	return v;
};

NJRvector3d NJRvector3d::RotateAround(const NJRvector3d& va) const
{
	NJRvector3d u   = va.direction();
	double      a   = va.length();
	double      s   = cos (0.5*a);
	double      t   = sin (0.5*a);
	NJRvector3d v   = u*t;
	NJRvector3d p   = *this;
	NJRvector3d out = (p*s*s) + (v*(p%v)) + (2.0*s*(v*p)) + (v*(v*p));
	return out;
};

NJRvector3d NJRvector3d::Trans
	(const NJRvector3d& nx, const NJRvector3d& ny, const NJRvector3d& nz) const
{
   return (_x*nx + _y*ny + _z*nz);
};

NJRvector3d NJRvector3d::ProjectOn(const NJRvector3d& v) const
{
	return v.direction() * (( (*this)%v ) / v.length());
};

double NJRvector3d::Dot(const NJRvector3d& v) const
{
	return ( (_x*v._x) + (_y*v._y) + (_z*v._z) );
};

NJRvector3d NJRvector3d::Cross(const NJRvector3d& v) const
{
	NJRvector3d out
		( (_y*v._z)-(_z*v._y), (_z*v._x)-(_x*v._z), (_x*v._y)-(_y*v._x) );
	return out;
};

const NJRvector3d& NJRvector3d::operator = (const NJRvector3d& vAval)
{
	_x = vAval._x;
	_y = vAval._y;
	_z = vAval._z;
	return *this;
};

const NJRvector3d& NJRvector3d::operator = (const char* cInit)
{
	char strv[256];
	strcpy (strv, cInit);
	if ( (strv[0] != '(') || (strv[strlen(strv) -1 ] != ')') )
	{
		std::cerr << "Error!! Code: NJRvector3d::operator = (const char*)" << std::endl;
		exit(-1);
	}

	strv[0] = ' ';
	strv[ strlen(strv) -1] = ' ';
	if ( sscanf (strv,"%lf, %lf, %lf",&_x,&_y,&_z) != 3 )
	{
		std::cerr << "Error!! Code: NJRvector3d::operator = (const char*)" << std::endl;
		exit(-1);
	}
	return *this;
};

const NJRvector3d& NJRvector3d::operator = (const NJRDXF::ParticularVector& pvAval)
{
	switch (pvAval)
	{
		case NJRDXF::ORIGIN:
			this->Set(0.0, 0.0, 0.0);
			break;
		case NJRDXF::AXIALX:
			this->Set(1.0, 0.0, 0.0);
			break;
		case NJRDXF::AXIALY:
			this->Set(0.0, 1.0, 0.0);
			break;
		case NJRDXF::AXIALZ:
			this->Set(0.0, 0.0, 1.0);
			break;
		case NJRDXF::ZERO:
			this->Set(0.0, 0.0, 0.0);
			break;
	};
	return *this;
};

const NJRvector3d& NJRvector3d::operator = (const std::string& sAval)
{
	*this = sAval.c_str();
	return *this;
};

const NJRvector3d& NJRvector3d::operator += (const NJRvector3d& v)
{
	*this = *this + v;
	return *this;
};

const NJRvector3d& NJRvector3d::operator -= (const NJRvector3d& v)
{
	*this = *this - v;
	return *this;
};

const NJRvector3d& NJRvector3d::Set
	(const double& dx, const double& dy, const double& dz)
{
	_x = dx;
	_y = dy;
	_z = dz;
	return *this;
};

const NJRvector3d& NJRvector3d::SetX(const double& dx)
{
	_x = dx;
	return *this;
};

const NJRvector3d& NJRvector3d::SetY(const double& dy)
{
	_y = dy;
	return *this;
};

const NJRvector3d& NJRvector3d::SetZ(const double& dz)
{
	_z = dz;
	return *this;
};

const NJRvector3d& NJRvector3d::SetCylinder
	(const double& dRadius, const double& dThita, const double& dz)
{
	_x = dRadius * cos(dThita);
	_y = dRadius * sin(dThita);
	_z = dz;
	return *this;
};

const NJRvector3d& NJRvector3d::SetSphere
	(const double& dRadius, const double& dAlpha, const double& dThita)
{
	_x = dRadius * sin(dAlpha) * cos(dThita);
	_y = dRadius * sin(dAlpha) * sin(dThita);
	_z = dRadius * cos(dAlpha);
	return * this;
};

NJRvector3d NJRvector3d::operator + (const NJRvector3d& v) const
{
	 NJRvector3d out ( _x+v._x, _y+v._y, _z+v._z);
	 return out;
};

NJRvector3d NJRvector3d::operator - (const NJRvector3d& v) const
{
	 NJRvector3d out (_x-v._x, _y-v._y, _z-v._z);
	 return out;
};

NJRvector3d NJRvector3d::operator * (const NJRvector3d& v) const
{
	NJRvector3d out
		( (_y*v._z)-(_z*v._y), (_z*v._x)-(_x*v._z), (_x*v._y)-(_y*v._x) );
	return out;
};

NJRvector3d NJRvector3d::operator * (const double& s) const
{
	NJRvector3d out(_x*s, _y*s, _z*s);
	return out;
};

double NJRvector3d::operator % (const NJRvector3d& v) const
{
	return ( (_x*v._x) + (_y*v._y) + (_z*v._z) );
};

NJRvector3d NJRvector3d::operator >> (const NJRvector3d& v) const
{
	return (this->ProjectOn(v));
};

bool NJRvector3d::operator == (NJRvector3d& vR) const
{
	return ( (_x==vR._x) && (_y==vR._y) && (_z==vR._z) );
}

void NJRvector3d::print() const
{
	std::cout << '(' << _x << ',' << _y << ',' << _z << ")" << std::endl;
};

};   // namespace NJR



NJR::NJRvector3d operator - (const NJR::NJRvector3d& v)
{
	 NJR::NJRvector3d out(-v.x(), -v.y(), -v.z());
	 return out;
};

NJR::NJRvector3d operator * (const double& ds , const NJR::NJRvector3d& v)
{
	 NJR::NJRvector3d out(v.x()*ds, v.y()*ds, v.z()*ds);
	 return out;
};

std::ostream& operator << (std::ostream& os, const NJR::NJRvector3d& v)
{
	os << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')' << std::endl;
	return os;
};

std::istream& operator >> (std::istream& is, NJR::NJRvector3d& v)
{
	double x;
	double y;
	double z;
	is >> x >> y >> z ;
	v.Set(x,y,z);
	return is;
};
