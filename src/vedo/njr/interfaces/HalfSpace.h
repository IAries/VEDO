#ifndef _NJR_HALFSPACE_H
#define _NJR_HALFSPACE_H

#include <vedo/constants/interfaces/Constants.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <fstream>

#include <stdint.h>

#ifdef _STD_CPP_11
	enum Sense: vedo::vedo_int_t
	{
		E =  0,
		G = -1,
		L =  1,
		e =  0,
		g = -1,
		l =  1
	};
#else
	enum Sense
	{
		E =  0,
		G = -1,
		L =  1,
		e =  0,
		g = -1,
		l =  1
	};
#endif




namespace njr
{

class HalfSpace
{

public:

	explicit HalfSpace(vedo::vedo_float_t a, vedo::vedo_float_t b, vedo::vedo_float_t c, Sense sense, vedo::vedo_float_t d);

	HalfSpace(const HalfSpace &);

	HalfSpace();

	inline vedo::vedo_float_t a() const
	{
		return _a;
	}

	inline vedo::vedo_float_t b() const
	{
		return _b;
	}

	inline vedo::vedo_float_t c() const
	{
		return _c;
	}

	inline vedo::vedo_float_t d() const
	{
		return _d;
	}

	inline Sense sense() const
	{
        if (_sense > 0)
        {
            return L;
        }
        else if (_sense < 0)
        {
            return G;
        }
        else
        {
            return E;
        }
	}

	const HalfSpace& operator = (const HalfSpace &);

	void AbsRhs();

	void Translate(const Vector3d&);

	void Normalize();

	void RotateAround(const Vector3d&);

	void Set(vedo::vedo_float_t a, vedo::vedo_float_t b, vedo::vedo_float_t c, Sense sense, vedo::vedo_float_t d);

	void Set(vedo::vedo_float_t a, vedo::vedo_float_t b, vedo::vedo_float_t c, vedo::vedo_int_t sense, vedo::vedo_float_t d);

	inline void SetSense2E()
	{
		_sense = E;
	}

private:

	vedo::vedo_float_t _a;
	vedo::vedo_float_t _b;
	vedo::vedo_float_t _c;
	vedo::vedo_float_t _d;
	vedo::vedo_int_t   _sense;

};

}   // namespace njr



std::ostream& operator << (std::ostream&, const njr::HalfSpace&);

std::istream& operator >> (std::istream&, njr::HalfSpace&);

#endif // _NJR_HALFSPACE_H
