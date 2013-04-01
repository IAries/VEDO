#ifndef _NJR_ARRAY_3D_H
#define _NJR_ARRAY_3D_H

#include <NJR/Interfaces/Vector3d.h>
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
	};

	Array3d
		(const unsigned& uSize0,
		 const unsigned& uSize1,
		 const unsigned& uSize2,
		 const T& tValue        )
	{
		T tDefault;
		std::vector<T> vt2;
		for (unsigned u=0; u<uSize2; u++)
			vt2.push_back(tDefault);

		std::vector<std::vector<T> > vt1;
		for (unsigned u=0; u<uSize1; u++)
			vt1.push_back(vt2);

		for (unsigned u=0; u<uSize0; u++)
			_vvvdComponent.push_back(vt1);

		for (unsigned u0=0; u0<uSize0; u0++)
		{
			for (unsigned u1=0; u1<uSize1; u1++)
			{
				for (unsigned u2=0; u2<uSize2; u2++)
				{
					this->SetComponent(u0, u1, u2, tValue);
				}
			}
		}
	};

	Array3d(const Array3d& a)
	{
		*this = a;
	};

	const Array3d& operator = (const Array3d& a)
	{
		_vvvdComponent = a.GetComponents();
		return *this;
	};

	~Array3d()
	{
	};

	T GetComponent
		(const unsigned& u0, const unsigned& u1, const unsigned& u2) const
	{
		if (   (u0<_vvvdComponent      .size())
			&& (u1<_vvvdComponent[0]   .size())
			&& (u2<_vvvdComponent[0][0].size()))
			return _vvvdComponent[u0][u1][u2];
		else
			return 0.0;
	};

	void SetComponent
		(const unsigned& u0,
		 const unsigned& u1,
		 const unsigned& u2,
		 const T& tv   )
	{
		if (   (u0<_vvvdComponent.size()      )
			&& (u1<_vvvdComponent[0].size()   )
			&& (u2<_vvvdComponent[0][0].size()))
			_vvvdComponent[u0][u1][u2] = tv;
	};

	inline std::vector<std::vector<std::vector<double> > > GetComponents() const
	{
		return _vvvdComponent;
	};

	const unsigned GetSize(const unsigned& u) const
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
	};

	void ReSize
		(const unsigned& uSize0, const unsigned& uSize1, const unsigned& uSize2)
	{
		_vvvdComponent.clear();

		T tDefault;
		std::vector<T> vd2;
		for (unsigned u=0; u<uSize2; u++)
			vd2.push_back(tDefault);

		std::vector<std::vector<double> > vd1;
		for (unsigned u=0; u<uSize1; u++)
			vd1.push_back(vd2);

		for (unsigned u=0; u<uSize0; u++)
			_vvvdComponent.push_back(vd1);

		for (unsigned u0=0; u0<uSize0; u0++)
		{
			for (unsigned u1=0; u1<uSize1; u1++)
			{
				for (unsigned u2=0; u2<uSize2; u2++)
				{
					this->SetComponent(u0, u1, u2, 0.0);
				}
			}
		}
	};

	void Print()
	{
		for(unsigned u0=0; u0<_vvvdComponent.size(); u0++)
		{
			for(unsigned u1=0; u1<_vvvdComponent[0].size(); u1++)
			{
				for(unsigned u2=0; u2<_vvvdComponent[0][0].size(); u2++)
				{
					std::cout
						<< "(" << u0 << ", " << u1 << ", " << u2 << ") = "
						<< _vvvdComponent[u0][u1][u2] << std::endl;
				}
			}
		}
	};

private:

	std::vector<std::vector<std::vector<T> > > _vvvdComponent;
};

};   // namespace njr

#endif // _NJR_ARRAY_3D_H
