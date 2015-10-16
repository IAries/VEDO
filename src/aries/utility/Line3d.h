#ifndef _ARIES_LINE_3D_H
#define _ARIES_LINE_3D_H

#include <aries/utility/DataType.h>
#include <aries/utility/Vector3d.h>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

namespace aries
{

template <typename T>
class Line3d
{

public:

	Line3d(const Vector3d<T>& p1, const Vector3d<T>& p2): _p1(p1), _p2(p2)
	{
	}

	Line3d(const Vector3d<T>& center, const Vector3d<T>& direction, const T& length)
	{
		_p1 = center - 0.5 * length * direction;
		_p2 = center + 0.5 * length * direction;
	}

	Line3d(const Line3d& L)
	{
		*this = L;
	}

	void set_p1(const Vector3d<T>& p1)
	{
		_p1 = p1;
	}

	void set_p2(const Vector3d<T>& p2)
	{
		_p2 = p2;
	}

	inline Vector3d<T> p1() const
	{
		return _p1;
	}

	inline Vector3d<T> p2() const
	{
		return _p2;
	}

	Vector3d<T> center() const
	{
		return 0.5 * (_p1 + _p2);
	}

	T length() const
	{
		return (_p2 - _p1).length();
	}

	Vector3d<T> direction() const
	{
		return (_p2 - _p1).direction();
	}

	const Line3d<T>& operator = (const Line3d<T>& L)
	{
		_p1 = L.p1();
		_p2 = L.p2();
		return *this;
	}

	bool operator == (Line3d<T>& L) const
	{
		return ((_p1==L.p1()) && (_p2==L.p2()));
	}

	bool operator != (Line3d<T>& L) const
	{
		return ((_p1!=L.p1()) || (_p2!=L.p2()));
	}

	bool InCoverSpace(const Vector3d<T>& p) const
	{
		Vector3d<T> dir = this->direction();
		T tStart = _p1.dot(dir);
		T tEnd   = _p2.dot(dir);
		T tPoint = p.dot(dir);
		if (tStart <= tEnd)
		{
			return ((tPoint >= tStart) && (tEnd >= tPoint));
		}
		else
		{
			return ((tPoint >= tEnd) && (tStart >= tPoint));
		}
	}

protected:

	Vector3d<T> _p1;

	Vector3d<T> _p2;

	Line3d()
	{
	}
};

}   // namespace aries



template <typename T>
std::ostream& operator << (std::ostream& os, const aries::Line3d<T>& L)
{
	os
		<< '(' << L.p1().x() << ", " << L.p1().y() << ", " << L.p1().z() << ')' << " -> "
		<< '(' << L.p2().x() << ", " << L.p2().y() << ", " << L.p2().z() << ')' << std::endl
		<< "Direction: " << L.direction()
		<< "Length   : " << L.length()                                          << std::endl
		<< "Center   : " << L.center()                                          << std::endl;
	return os;
}

#endif   // _ARIES_LINE_3D_H
