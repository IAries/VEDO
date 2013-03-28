#include <NJR/Interfaces/HalfSpace.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace NJR
{

HalfSpace::HalfSpace()
{
	HalfSpace::Set(0.0, 0.0, 1.0, E, 1.0);
};

HalfSpace::HalfSpace(const HalfSpace &hf)
{
	HalfSpace::Set(hf._a, hf._b, hf._c ,hf._sense, hf._d);
};

HalfSpace::HalfSpace
	(double a, double b, double c, Sense sense, double d)
{
	HalfSpace::Set(a, b, c, sense, d);
};

const HalfSpace& HalfSpace::operator = (const HalfSpace &hf)
{
	HalfSpace::Set(hf.a(), hf.b(), hf.c(), hf.sense(), hf.d());
	return *this;
};

void HalfSpace::Set(double a, double b, double c, Sense sense, double d)
{
	_a = a;
	_b = b;
	_c = c;
	_d = d;
	_sense = sense;
};


void HalfSpace::Normalize()
{
	double length;
	length = sqrt (_a*_a + _b*_b + _c*_c);
	_a /= length;
	_b /= length;
	_c /= length;
	_d /= length;
};

void HalfSpace::AbsRhs()
{
	if (_d < 0)
	{
		_a *= -1.0;
		_b *= -1.0;
		_c *= -1.0;
		_d *= -1.0;

		if ((_sense==G) || (_sense==g))
		{
			_sense = L;
		}
        else if ((_sense==L) || (_sense==l))
		{
			_sense = G;
		}
	}
};

void HalfSpace::Translate(const Vector3d& dp)
{
	_d += (_a*dp.x() + _b*dp.y() + _c * dp.z());
};

void HalfSpace::RotateAround(const Vector3d &dw)
{

	Vector3d gradient(_a, _b, _c);

	gradient = gradient.RotateAround(dw);

	_a = gradient.x();
	_b = gradient.y();
	_c = gradient.z();
};

};   // namespace NJR



std::ostream& operator << (std::ostream& os, const NJR::HalfSpace& hf)
{
    os	<< "[" << hf.a() << " X " << hf.b() << " Y " << hf.c() << " Z ";

	switch (hf.sense())
	{
		case G:
			os << ">=";
			break;
		case E:
			os << "=";
			break;
		case L:
			os << "<=";
			break;
	}
	os << hf.d() << ']' << std::endl;
	return os;
};

std::istream& operator >> (std::istream& is, NJR::HalfSpace& hf)
{
	double a;
	double b;
	double c;
	double d;
	char sense;
	Sense se;
    is >> a >> b >> c >> sense >> d;

	switch (sense)
	{
		case 'E':
			se = E;
			break;
		case 'G':
			se = G;
			break;
		case 'L':
			se = L;
			break;
		default:
			std::cout
				<< "Error!! Code: halfspace.cpp" << std::endl
				<< "        Note: halfspace: ( a , b ,c , ~ ,d)" << std::endl;
			exit(0);
	}

	hf.Set(a, b, c, se, d);
	return is;
};   // namespace NJR

