#ifndef _ARIES_VECTOR_3D_H
#define _ARIES_VECTOR_3D_H

#include <aries/utility/DataType.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace aries
{

template <typename T>
class Vector3d
{

public:

	Vector3d()
	{
	}

	Vector3d(const T& x, const T& y, const T& z)
	{
		this->set(x, y, z);
	}

	Vector3d(const Vector3d& v)
	{
		*this = v;
	}

	const Vector3d& set(const T& x, const T& y, const T& z)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		return *this;
	}

	const Vector3d& Set(const T& x, const T& y, const T& z)
	{
		this->set(x, y, z);
	}

	const Vector3d& set_x(const T& x)
	{
		_v[0] = x;
		return *this;
	}

	const Vector3d& SetX(const T& x)
	{
		this->set_x(x);
	}

	const Vector3d& set_y(const T& y)
	{
		_v[1] = y;
		return *this;
	}

	const Vector3d& SetY(const T& y)
	{
		this->set_y(y);
	}

	const Vector3d& set_z(const T& z)
	{
		_v[2] = z;
		return *this;
	}

	const Vector3d& SetZ(const T& z)
	{
		this->set_z(z);
	}

	const Vector3d& set_cylinder(const T& radius, const T& thita, const T& z)
	{
		_v[0] = radius * std::cos(thita);
		_v[1] = radius * std::sin(thita);
		_v[2] = z;
		return *this;
	}

	const Vector3d& set_sphere(const T& radius, const T& alpha, const T& thita)
	{
		_v[0] = radius * std::sin(alpha) * std::cos(thita);
		_v[1] = radius * std::sin(alpha) * std::sin(thita);
		_v[2] = radius * std::cos(alpha);
		return *this;
	}

	inline T x() const
	{
		return _v[0];
	}

	inline T y() const
	{
		return _v[1];
	}

	inline T z() const
	{
		return _v[2];
	}

	T length() const
	{
		return std::sqrt((_v[0]*_v[0]) + (_v[1]*_v[1]) + (_v[2]*_v[2]));
	}

	Vector3d direction() const
	{
		Vector3d v;
		T len = this->length();
		if (len != 0)
		{
			v.set(_v[0]/len, _v[1]/len, _v[2]/len);
		}
		return v;
	}

	bool normalized()
	{
		T len = this->length();
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

	/***************************************************************************************************************************
	 * Retrieves the new position of this std::vector after this std::vector has been rotated around the given std::vector gv.
	 *
	 * Let u be the direction of gv and L be the length of gv. Rotates this std::vector along the unit std::vector u with
	 * rotation angle L about u and then retrieves the new position of this std::vector.
	 *
	 * EFERENCE: Computer Graphics 2th Donald. Hearn and M. Pauline Baker page 419-420
	 **************************************************************************************************************************/
	Vector3d rotate_around(const Vector3d& va) const
	{
		Vector3d u   = va.direction();
		T        a   = va.length();
		T        s   = std::cos((T)0.5 * a);
		T        t   = std::sin((T)0.5 * a);
		Vector3d v   = u * t;
		Vector3d p   = *this;
		//Vector3d out = (p * s * s) + (v * (p %   v)) + (2.0 * s * (v *     p)) + (v *    (v *     p ));
		//Vector3d out =  p * s * s  +  v * (p.dot(v)) +  2.0 * s * (v.cross(p)) + (v.cross(v.cross(p)));
		return (s * s * p + (p.dot(v)) * v + (T)2.0 * s * (v.cross(p)) + (v.cross(v.cross(p))));

		/*
		Vector3d u   = va.direction();
		T a   = va.length();
		T s   = std::cos(0.5*a);
		T t   = std::sin(0.5*a);
		T cs  = std::cos(va.length());
		T t   = std::sin(a);
		Vector3d v   = u*t;
		Vector3d p   = (*this);
		Vector3d out = (p*s*s) + (v*(p%v)) + (2.0*s*(v*p)) + (v*(v*p));
		// Another slightly quick rotation method
		Vvector3d out = ((*this)*cs) + va.cross(*this) + 1.0/(1+cs)*va*(va.dot(*this));
		return out;
		*/
	}

	Vector3d RotateAround(const Vector3d& va) const
	{
		return this->rotate_around(va);
	}

	// Retrieves the new position of this std::vector after axial x/z/y have been replaced by nx/nz/ny
	Vector3d trans(const Vector3d& nx, const Vector3d& ny, const Vector3d& nz) const
	{
	   return (_v[0] * nx + _v[1] * ny + _v[2] * nz);
	}

	Vector3d Trans(const Vector3d& nx, const Vector3d& ny, const Vector3d& nz) const
	{
		return this->trans(nx, ny, nz);
	}

	// Retrieves the projected Vector3d on v of this Vector3d
	Vector3d project_on(const Vector3d& v) const
	{
		return ((this->dot(v)) / v.length()) * v.direction();
	}

	Vector3d ProjectOn(const Vector3d& v) const
	{
		return this->project_on(v);
	}

	T dot(const Vector3d& v) const
	{
		return ( (_v[0]*v._v[0]) + (_v[1]*v._v[1]) + (_v[2]*v._v[2]) );
	}

	T Dot(const Vector3d& v) const
	{
		return this->dot(v);
	}

	T operator % (const Vector3d& v) const
	{
		return this->dot(v);
	}

	Vector3d cross(const Vector3d& v) const
	{
		return Vector3d((_v[1]*v._v[2])-(_v[2]*v._v[1]), (_v[2]*v._v[0])-(_v[0]*v._v[2]), (_v[0]*v._v[1])-(_v[1]*v._v[0]));
	}

	Vector3d Cross(const Vector3d& v) const
	{
		return this->cross(v);
	}

	Vector3d operator * (const Vector3d& v) const
	{
		return this->cross(v);
	}

	const Vector3d& operator = (const Vector3d& v)
	{
	    memcpy(&_v, &v, 3*sizeof(T));
		return *this;
	}

	const Vector3d& operator += (const Vector3d& v)
	{
		*this = *this + v;
		return *this;
	}

	const Vector3d& operator -= (const Vector3d& v)
	{
		*this = *this - v;
		return *this;
	}

	Vector3d operator + (const Vector3d& v) const
	{
		 Vector3d out ( _v[0]+v._v[0], _v[1]+v._v[1], _v[2]+v._v[2]);
		 return out;
	}

	Vector3d operator - (const Vector3d& v) const
	{
		 Vector3d out (_v[0]-v._v[0], _v[1]-v._v[1], _v[2]-v._v[2]);
		 return out;
	}

	Vector3d operator * (const T& s) const
	{
		return Vector3d(_v[0]*s, _v[1]*s, _v[2]*s);
	}

	bool operator == (Vector3d& vR) const
	{
		return ((_v[0]==vR._v[0]) && (_v[1]==vR._v[1]) && (_v[2]==vR._v[2]));
	}

	bool operator != (Vector3d& vR) const
	{
		return ((_v[0]!=vR._v[0]) || (_v[1]!=vR._v[1]) || (_v[2]!=vR._v[2]));
	}

	// Transform a Vector3d from local coordinate to global coordinate
	void CoordinateTransformation
		(const Vector3d& LocalOrentationX, const Vector3d& LocalOrentationY, const Vector3d& LocalOrentationZ)
	{
		Vector3d NewVector3d
			(LocalOrentationX.x() * _v[0] + LocalOrentationY.x() * _v[1] + LocalOrentationZ.x() * _v[2],
			 LocalOrentationX.y() * _v[0] + LocalOrentationY.y() * _v[1] + LocalOrentationZ.y() * _v[2],
			 LocalOrentationX.z() * _v[0] + LocalOrentationY.z() * _v[1] + LocalOrentationZ.z() * _v[2] );
		*this = NewVector3d;
	}

	void print() const
	{
		std::cout << '(' << _v[0] << ", " << _v[1] << ", " << _v[2] << ')' << std::endl;
	}

protected:

	T _v[3];
};

}   // namespace aries



template <typename T>
aries::Vector3d<T> operator - (const aries::Vector3d<T>& v)
{
	return aries::Vector3d<T>(-v.x(), -v.y(), -v.z());
}

template <typename T>
aries::Vector3d<T> operator * (const T s , const aries::Vector3d<T> v)
{
	return aries::Vector3d<T>(s * v.x(), s * v.y(), s * v.z());
}

template <typename T>
std::ostream& operator << (std::ostream& os, const aries::Vector3d<T>& v)
{
	os << '(' << v.x() << ", " << v.y() << ", " << v.z() << ')' << std::endl;
	return os;
}

template <typename T>
std::istream& operator >> (std::istream& is, aries::Vector3d<T>& v)
{
	T x, y, z;
	is >> x >> y >> z ;
	v.set(x, y, z);
	return is;
}

#endif   // _ARIES_VECTOR_3D_H
