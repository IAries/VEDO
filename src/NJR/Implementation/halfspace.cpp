#include <NJR/Interfaces/halfspace.h>
#include <cmath>
#include <cstdlib>
#include <iostream>

namespace NJR
{

NJRhalfspace::NJRhalfspace()
{
	NJRhalfspace::Set(0.0, 0.0, 1.0, E, 1.0);
};

NJRhalfspace::NJRhalfspace(const NJRhalfspace &hf)
{
	NJRhalfspace::Set(hf._a, hf._b, hf._c ,hf._sense, hf._d);
};

NJRhalfspace::NJRhalfspace
	(double a, double b, double c, Sense sense, double d)
{
	NJRhalfspace::Set(a, b, c, sense, d);
};

const NJRhalfspace& NJRhalfspace::operator = (const NJRhalfspace &hf)
{
	NJRhalfspace::Set(hf.a(), hf.b(), hf.c(), hf.sense(), hf.d());
	return *this;
};

void NJRhalfspace::Set(double a, double b, double c, Sense sense, double d)
{
	_a = a;
	_b = b;
	_c = c;
	_d = d;
	_sense = sense;
};


void NJRhalfspace::Normalize()
{
	double length;
	length = sqrt (_a*_a + _b*_b + _c*_c);
	_a /= length;
	_b /= length;
	_c /= length;
	_d /= length;
};

void NJRhalfspace::AbsRhs()
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

void NJRhalfspace::Translate(const NJRvector3d& dp)
{
	_d += (_a*dp.x() + _b*dp.y() + _c * dp.z());
};

void NJRhalfspace::RotateAround(const NJRvector3d &dw)
{

	NJRvector3d gradient(_a, _b, _c);

	gradient = gradient.RotateAround(dw);

	_a = gradient.x();
	_b = gradient.y();
	_c = gradient.z();
};

};   // namespace NJR



std::ostream& operator << (std::ostream& os, const NJR::NJRhalfspace& hf)
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

std::istream& operator >> (std::istream& is, NJR::NJRhalfspace& hf)
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

