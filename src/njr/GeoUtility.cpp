#include <njr/GeoUtility.h>
#include <njr/LinearProgramming.h>
#include <aries/utility/Vector3df.h>
#include <algorithm>
#include <cmath>

namespace njr
{

_float_t Distance(const aries::Vector3df& v, const njr::HalfSpace& hf)
{
	return Distance(hf, v);
}

_float_t Distance(const njr::HalfSpace& hf, const aries::Vector3df& v)
{
	return fabs(hf.a()*v.x() + hf.b()*v.y() + hf.c()*v.z() - hf.d()) / sqrt(hf.a()*hf.a() + hf.b()*hf.b() + hf.c()*hf.c());
}

_float_t Volume(const njr::NJRpolygon& bottom, const aries::Vector3df& vertex)
{
	if ((bottom.vertexes().size()) < 3)
	{
		return 0;
	}

	std::vector<aries::Vector3df> vertexes = bottom.vertexes();

	aries::Vector3df n = (vertexes[1]-vertexes[0]).cross((vertexes[2]-vertexes[0]));

	_float_t d = Distance(njr::HalfSpace(n.x(), n.y(), n.z(), E, n.dot(vertexes[0])), vertex);

	return d*bottom.area()/3.0;
}

_float_t Volume(const njr::NJRpolygon& bottom, const aries::Vector3df& vertex, aries::Vector3df& MassCenter)
{

	if ((bottom.vertexes().size()) < 3)
	{
		return 0;
	}

	aries::Vector3df center = bottom.center();

	std::vector<aries::Vector3df> vertexes = bottom.vertexes();

	aries::Vector3df mr;

	_float_t tm = 0;

	for (_uint_t i=0; i<vertexes.size(); ++i)
	{
		_uint_t j = (i==(vertexes.size()-1)) ? 0 : i+1;
		_float_t Vol = fabs(((vertexes[j]-center).cross(vertexes[i]-center)).dot(vertex-center) * (1/6.0));
		mr += (_float_t)(0.25*Vol) * (vertexes[i]+vertexes[j]+vertex+center);
		tm += Vol;
	}

	MassCenter = mr * (1.0 / tm);
	return tm;
}


_float_t Volume(const njr::NJRpolyhedra& poly)
{
	std::vector<njr::NJRpolygon> faces = poly.faces();
	_float_t dVolume = 0;
	for (_uint_t i=0; i<faces.size(); ++i)
	{
		dVolume += Volume(faces[i], poly.center());
	}
	return dVolume;
}

_float_t Volume(const njr::NJRpolyhedra& poly, aries::Vector3df& MassCenter)
{
	std::vector<njr::NJRpolygon> faces = poly.faces();
	_float_t dVolume = 0;
	aries::Vector3df center, mr;
	for (_uint_t i=1; i<faces.size(); ++i)
	{
		_float_t vol = Volume(faces[i], faces[0].center(), center);
		mr += center*vol;
		dVolume += vol;
	}
	MassCenter = mr*(1.0/dVolume);
	return dVolume;
}



class maxCR
{
private:

	const aries::Vector3df center;

public:

	explicit maxCR(const aries::Vector3df& c): center (c)
	{
	}

	bool operator() (const aries::Vector3df& a, const aries::Vector3df& b)
	{
		return (((a-center).length()) < ((b-center).length()));
	}
};



_float_t CoverRadius(const njr::NJRpolyhedra& p)
{

	std::vector<njr::NJRpolygon> faces = p.faces();
	std::vector<aries::Vector3df> vertexes;
	std::vector<_float_t> MR(faces.size());
	aries::Vector3df center = p.center();

	for (_uint_t i=0; i<faces.size(); ++i)
	{
		vertexes = faces[i].vertexes();
		MR[i] = (*(max_element(vertexes.begin(), vertexes.end(), maxCR(center))) - center).length();
	}
	return *( max_element(MR.begin(), MR.end() ) );
}

aries::Vector3df InertiaTensor(const njr::NJRpolyhedra& p)
{
	njr::LinearProgramming lp;
	njr::NJRpolyhedra a,b;
	lp.Set(p);

	_float_t Ux;
	_float_t Lx;
	_float_t Uy;
	_float_t Ly;
	_float_t Uz;
	_float_t Lz;
	_float_t Rx;
	_float_t Ry;
	_float_t Rz;
	_float_t x;
	_float_t y;
	_float_t z;

	if ((lp.GetExtremeValue(Ux, Lx, Uy, Ly, Uz, Lz)) == false)
	{
		return aries::Vector3df();
	}

	_float_t dVolume;
	aries::Vector3df MassCenter;
	_float_t Ix=0.0, Iy=0.0, Iz=0.0;

	a = p;
	Volume(a,MassCenter);
	a.SetCenter(MassCenter);
	a.Translate(-a.center());

	Rx = (Ux-Lx) * 0.15;
	Ry = (Uy-Ly) * 0.15;
	Rz = (Uz-Lz) * 0.15;

	for (x=Lx; x<Ux; x+=Rx)
	{
		for (y=Ly; y<Uy; y+=Ry)
		{
			for (z=Lz; z<Uz; z+=Rz)
			{
				b = a;

				b.AddConstrain(njr::HalfSpace(1, 0, 0, G, x));
				b.AddConstrain(njr::HalfSpace(1, 0, 0, L, x+Rx));
				b.AddConstrain(njr::HalfSpace(0, 1, 0, G, y));
				b.AddConstrain(njr::HalfSpace(0, 1, 0, L, y+Ry));
				b.AddConstrain(njr::HalfSpace(0, 0, 1, G, z));
				b.AddConstrain(njr::HalfSpace(0, 0, 1, L, z+Rz));

				b.SetCenter(aries::Vector3df(x+0.5*Rx, y+0.5*Ry, z+0.5*Rz));

				dVolume = Volume(b,MassCenter);

				if (dVolume != 0)
				{
					Ix += dVolume*MassCenter.x()*MassCenter.x();
					Iy += dVolume*MassCenter.y()*MassCenter.y();
					Iz += dVolume*MassCenter.z()*MassCenter.z();
				}
			}
		}
	}

	return aries::Vector3df(Iy+Iz, Ix+Iz, Ix+Iy);
}

}   // namespace njr
