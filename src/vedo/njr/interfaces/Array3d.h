#ifndef _NJR_ARRAY_3D_H
#define _NJR_ARRAY_3D_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <string>
#include <iostream>
#include <vector>

namespace njr
{

template <typename T>
class Array3d
{

public:

	Array3d()
	{
		this->ReSize(1, 1, 1);
	}

	Array3d(const vedo_uint_t& uSize0, const vedo_uint_t& uSize1, const vedo_uint_t& uSize2, const T& tValue)
	{
		T tDefault;
		std::vector<T> vt2;
		for (vedo_uint_t u=0; u<uSize2; u++)
		{
			vt2.push_back(tDefault);
		}

		std::vector<std::vector<T> > vt1;
		for (vedo_uint_t u=0; u<uSize1; u++)
		{
			vt1.push_back(vt2);
		}

		for (vedo_uint_t u=0; u<uSize0; u++)
		{
			_vvvdComponent.push_back(vt1);
		}

		for (vedo_uint_t u0=0; u0<uSize0; u0++)
		{
			for (vedo_uint_t u1=0; u1<uSize1; u1++)
			{
				for (vedo_uint_t u2=0; u2<uSize2; u2++)
				{
					this->SetComponent(u0, u1, u2, tValue);
				}
			}
		}
	}

	Array3d(const Array3d& a)
	{
		*this = a;
	}

	const Array3d& operator = (const Array3d& a)
	{
		_vvvdComponent = a.GetComponents();
		return *this;
	}

	~Array3d()
	{
	}

	T GetComponent(const vedo_uint_t& u0, const vedo_uint_t& u1, const vedo_uint_t& u2) const
	{
		if((u0<_vvvdComponent.size()) && (u1<_vvvdComponent[0].size()) && (u2<_vvvdComponent[0][0].size()))
		{
			return _vvvdComponent[u0][u1][u2];
		}
		else
		{
			return 0.0;
		}
	}

	void SetComponent(const vedo_uint_t& u0, const vedo_uint_t& u1, const vedo_uint_t& u2, const T& tv)
	{
		if((u0<_vvvdComponent.size()) && (u1<_vvvdComponent[0].size()) && (u2<_vvvdComponent[0][0].size()))
		{
			_vvvdComponent[u0][u1][u2] = tv;
		}
	}

	inline std::vector<std::vector<std::vector<vedo::vedo_float_t> > > GetComponents() const
	{
		return _vvvdComponent;
	}

	const vedo_uint_t GetSize(const vedo_uint_t& u) const
	{
		switch(u)
		{
			case 0:
				return _vvvdComponent.size();
				break;
			case 1:
				return _vvvdComponent[0].size();
				break;
			case 2:
				return _vvvdComponent[0][0].size();
				break;
			default:
				return 0;
				break;
		}
	}

	void ReSize(const vedo_uint_t& uSize0, const vedo_uint_t& uSize1, const vedo_uint_t& uSize2)
	{
		_vvvdComponent.clear();

		T tDefault;
		std::vector<T> vd2;
		for (vedo_uint_t u=0; u<uSize2; u++)
		{
			vd2.push_back(tDefault);
		}

		std::vector<std::vector<vedo::vedo_float_t> > vd1;
		for (vedo_uint_t u=0; u<uSize1; u++)
		{
			vd1.push_back(vd2);
		}

		for (vedo_uint_t u=0; u<uSize0; u++)
		{
			_vvvdComponent.push_back(vd1);
		}

		for (vedo_uint_t u0=0; u0<uSize0; u0++)
		{
			for (vedo_uint_t u1=0; u1<uSize1; u1++)
			{
				for (vedo_uint_t u2=0; u2<uSize2; u2++)
				{
					this->SetComponent(u0, u1, u2, 0.0);
				}
			}
		}
	}

	void Print()
	{
		for (vedo_uint_t u0=0; u0<_vvvdComponent.size(); u0++)
		{
			for (vedo_uint_t u1=0; u1<_vvvdComponent[0].size(); u1++)
			{
				for (vedo_uint_t u2=0; u2<_vvvdComponent[0][0].size(); u2++)
				{
					std::cout << "(" << u0 << ", " << u1 << ", " << u2 << ") = " << _vvvdComponent[u0][u1][u2] << std::endl;
				}
			}
		}
	}

private:

	std::vector<std::vector<std::vector<T> > > _vvvdComponent;
};

}   // namespace njr

#endif // _NJR_ARRAY_3D_H
