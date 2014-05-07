#ifndef _NJR_HALFSPACE_H
#define _NJR_HALFSPACE_H

#include <vedo/Constants.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <fstream>

#include <stdint.h>

enum Sense
{
	E =  0,
	G = -1,
	L =  1,
	e =  0,
	g = -1,
	l =  1
};

namespace njr
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
        if(_sense > 0)
        {
            return L;
        }
        else if(_sense < 0)
        {
            return G;
        }
        else
        {
            return E;
        }
	};

	const HalfSpace& operator = (const HalfSpace &);

	void AbsRhs();

	void Translate(const Vector3d&);

	void Normalize();

	void RotateAround(const Vector3d&);

	void Set(double a, double b, double c, Sense sense, double d);

	void Set(double a, double b, double c, int sense, double d);

	inline void SetSense2E()
	{
		_sense = E;
	};

private:

	double           _a;
	double           _b;
	double           _c;
	double           _d;
	vedo::_VEDO_long _sense;

};

};   // namespace njr



std::ostream& operator << (std::ostream&, const njr::HalfSpace&);

std::istream& operator >> (std::istream&, njr::HalfSpace&);

#endif // _NJR_HALFSPACE_H
