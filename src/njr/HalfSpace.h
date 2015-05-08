#ifndef _NJR_HALFSPACE_H
#define _NJR_HALFSPACE_H

#include <njr/DataType.h>
#include <aries/utility/Vector3df.h>
#include <fstream>

#include <stdint.h>



namespace njr
{

	#ifdef _STD_CPP_11
	enum Sense: _int_t
	#else
	enum Sense
	#endif
	{
		E =  0,
		G = -1,
		L =  1,
		e =  0,
		g = -1,
		l =  1
	};



class HalfSpace
{

public:

	explicit HalfSpace(_float_t a, _float_t b, _float_t c, Sense sense, _float_t d);

	HalfSpace(const HalfSpace &);

	HalfSpace();

	inline _float_t a() const
	{
		return _a;
	}

	inline _float_t b() const
	{
		return _b;
	}

	inline _float_t c() const
	{
		return _c;
	}

	inline _float_t d() const
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

	void Translate(const Vector3df&);

	void Normalize();

	void RotateAround(const Vector3df&);

	void Set(_float_t a, _float_t b, _float_t c, Sense sense, _float_t d);

	void Set(_float_t a, _float_t b, _float_t c, _int_t sense, _float_t d);

	inline void SetSense2E()
	{
		_sense = E;
	}

private:

	_float_t _a;
	_float_t _b;
	_float_t _c;
	_float_t _d;
	_int_t   _sense;

};

}   // namespace njr



std::ostream& operator << (std::ostream&, const njr::HalfSpace&);

std::istream& operator >> (std::istream&, njr::HalfSpace&);

#endif // _NJR_HALFSPACE_H
