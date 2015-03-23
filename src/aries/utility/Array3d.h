#ifndef _ARIES_ARRAY_3D_H
#define _ARIES_ARRAY_3D_H

#include <aries/utility/DataType.h>
#include <iostream>
#include <string>
#include <vector>

namespace aries
{

template <typename T>
class Array3d
{

public:

	Array3d()
	{
		this->resize(1, 1, 1);
		_ux = _uy = _uz = 1;
	}

	Array3d(const _uint_t& ux, const _uint_t& uy, const _uint_t& uz, const T& tValue): _ux(ux), _uy(uy), _uz(uz)
	{
		T tDefault;
		std::vector<T> t2;
		for (_uint_t u=0; u<_uz; u++)
		{
			t2.push_back(tDefault);
		}

		std::vector<std::vector<T> > t1;
		for (_uint_t u=0; u<_uy; u++)
		{
			t1.push_back(t2);
		}

		for (_uint_t u=0; u<_ux; u++)
		{
			_Component.push_back(t1);
		}

		this->fill(tValue);
	}

	Array3d(const Array3d& a)
	{
		*this = a;
	}

	const Array3d& operator = (const Array3d& a)
	{
		_ux        = a._ux;
		_uy        = a._uy;
		_uz        = a._uz;
		_Component = a.GetComponents();
		return *this;
	}

	~Array3d()
	{
	}

	T& operator () (const _uint_t& u0, const _uint_t& u1, const _uint_t& u2)
	{
		if((u0<_Component.size()) && (u1<_Component[0].size()) && (u2<_Component[0][0].size()))
		{
			return _Component[u0][u1][u2];
		}
		else
		{
			std::cerr << "Error!! Code: aries::Array3d::operator () (const _uint_t&, const _uint_t&)" << std::endl;
			exit(-1);
		}
	}

	T get(const _uint_t& u0, const _uint_t& u1, const _uint_t& u2) const
	{
		if((u0<_Component.size()) && (u1<_Component[0].size()) && (u2<_Component[0][0].size()))
		{
			return _Component[u0][u1][u2];
		}
		else
		{
			std::cerr << "Error!! Code: aries::Array3d::get(const _uint_t&, const _uint_t&) const" << std::endl;
			exit(-1);
		}
	}

	inline std::vector<std::vector<std::vector<T> > >* GetComponents() const
	{
		return &_Component;
	}

	inline _uint_t size_x() const
	{
		return _ux;
	}

	inline _uint_t size_y() const
	{
		return _uy;
	}

	inline _uint_t size_z() const
	{
		return _uz;
	}

	void resize(const _uint_t& ux, const _uint_t& uy, const _uint_t& uz)
	{
		_Component.clear();
		_ux = ux;
		_uy = uy;
		_uz = uz;

		T tDefault;

		std::vector<T> t2;
		for (_uint_t u=0; u<_uz; u++)
		{
			t2.push_back(tDefault);
		}

		std::vector<std::vector<T> > t1;
		for (_uint_t u=0; u<_uy; u++)
		{
			t1.push_back(t2);
		}

		for (_uint_t u=0; u<_ux; u++)
		{
			_Component.push_back(t1);
		}
	}

private:

	std::vector<std::vector<std::vector<T> > > _Component;

	_uint_t _ux;

	_uint_t _uy;

	_uint_t _uz;

	void fill(const T& tValue)
	{
		for (_uint_t u0=0; u0<_ux; u0++)
		{
			for (_uint_t u1=0; u1<_uy; u1++)
			{
				for (_uint_t u2=0; u2<_uz; u2++)
				{
					(*this)(u0, u1, u2) = tValue;
				}
			}
		}
	}
};

}   // namespace aries



template <typename T>
std::ostream& operator << (std::ostream& os, const aries::Array3d<T>& a)
{
	os << "aries::Array3d (" << a.size_x() << " x " << a.size_y() << " x " << a.size_z() << ")" << std::endl;
	for (aries::_uint_t u0=0; u0<a.size_x(); u0++)
	{
		for (aries::_uint_t u1=0; u1<a.size_y(); u1++)
		{
			for (aries::_uint_t u2=0; u2<a.size_z(); u2++)
			{
				std::cout << "(" << u0 << ", " << u1 << ", " << u2 << ") = " << a.get(u0, u1, u2) << std::endl;
			}
		}
	}
	return os;
}

#endif // _ARIES_ARRAY_3D_H
