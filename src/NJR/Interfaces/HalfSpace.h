#ifndef _NJR_HALFSPACE_H
#define _NJR_HALFSPACE_H

#include <NJR/Interfaces/Vector3d.h>
#include <fstream>

enum Sense
{
	E =  0,
	G = -1,
	L =  1,
	e =  0,
	g = -1,
	l =  1
};

namespace NJR
{

class HalfSpace
{
public:

	explicit HalfSpace(double a, double b, double c, Sense sense, double d);

	HalfSpace(const HalfSpace &);

	HalfSpace();

	inline double a() const
	{
		return _a;
	};

	inline double b() const
	{
		return _b;
	};

	inline double c() const
	{
		return _c;
	};

	inline double d() const
	{
		return _d;
	};

	inline Sense sense() const
	{
		return _sense;
	};

	const HalfSpace& operator = (const HalfSpace &);

	void AbsRhs();

	void Translate(const Vector3d&);

	void Normalize();

	void RotateAround(const Vector3d&);

	void Set(double a, double b, double c, Sense sense, double d);

	inline void SetSense2E()
	{
		_sense = E;
	};

private:

	double _a ;
	double _b ;
	double _c ;
	double _d ;
	Sense _sense;

};

};   // namespace NJR



std::ostream& operator << (std::ostream&, const NJR::HalfSpace&);

std::istream& operator >> (std::istream&, NJR::HalfSpace&);

#endif // _NJR_HALFSPACE_H
