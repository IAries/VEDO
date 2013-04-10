#include <NJR/Interfaces/GeoUtility.h>
#include <NJR/Interfaces/Vector3d.h>
#include <NJR/Interfaces/LinearProgramming.h>
#include <algorithm>
#include <cmath>

namespace njr
{

double Distance(const njr::Vector3d& v, const njr::HalfSpace& hf)
{
	return Distance(hf, v);
};

double Distance(const njr::HalfSpace& hf, const njr::Vector3d& v)
{
	return (fabs( hf.a()*v.x() + hf.b()*v.y() + hf.c()*v.z() - hf.d() ) /
			sqrt ( hf.a()*hf.a() + hf.b()*hf.b() + hf.c()*hf.c() )       );
};

double Volume(const njr::NJRpolygon& bottom, const njr::Vector3d& vertex)
{
	if ( (bottom.vertexes().size())<3 )
	{
		return 0;
	}

	std::vector<njr::Vector3d> vertexes = bottom.vertexes();

	njr::Vector3d n = (vertexes[1]-vertexes[0]) * (vertexes[2]-vertexes[0]);

	double d = Distance
		(njr::HalfSpace(n.x(), n.y(), n.z(), E, n%vertexes[0]), vertex);

	return d*bottom.area()/3.0;
};

double Volume
	(const njr::NJRpolygon& bottom,
	const njr::Vector3d& vertex,
	njr::Vector3d& MassCenter)
{

	if ( (bottom.vertexes().size()) < 3)
	{
		return 0;
	}

	njr::Vector3d center = bottom.center();

	std::vector<njr::Vector3d> vertexes = bottom.vertexes();

	njr::Vector3d mr(njrdxf::ZERO);

	double tm = 0;

	for (unsigned int i=0; i<vertexes.size(); ++i)
	{
		unsigned int j = (i==(vertexes.size()-1)) ? 0 : i+1;

		double Vol
			= fabs(	((vertexes[j]-center)*(vertexes[i]-center))
					%(vertex-center)
					*(1/6.0)                                   );

		mr += (0.25*Vol) * (vertexes[i]+vertexes[j]+vertex+center);
		tm += Vol;
	}

	MassCenter = mr * (1.0 / tm);
	return tm;
};


double Volume(const njr::NJRpolyhedra& poly)
{
	std::vector<njr::NJRpolygon> faces = poly.faces();

	double dVolume = 0;

	for (unsigned int i=0; i<faces.size(); ++i)
	{
		dVolume += Volume(faces[i], poly.center());
	}

	return dVolume;
};

double Volume(const njr::NJRpolyhedra& poly, njr::Vector3d& MassCenter)
{
	std::vector<njr::NJRpolygon> faces = poly.faces();

	double dVolume = 0;
	njr::Vector3d center;
	njr::Vector3d mr(njrdxf::ZERO);

	for (unsigned int i=1; i<faces.size(); ++i)
	{
		double vol = Volume(faces[i], faces[0].center(), center);
		mr += center*vol;
		dVolume += vol;
	}

	MassCenter = mr*(1.0/dVolume);
	return dVolume;
};



class maxCR
{
private:

	const njr::Vector3d center;

public:

	explicit maxCR(const njr::Vector3d& c) : center (c)
	{
	};

	bool operator() (const njr::Vector3d& a, const njr::Vector3d& b)
	{
		return ( ((a-center).length()) < ((b-center).length()) );
	};
};

double CoverRadius(const njr::NJRpolyhedra& p)
{

	std::vector<njr::NJRpolygon> faces = p.faces();
	std::vector<njr::Vector3d> vertexes;
	std::vector<double> MR(faces.size());
	njr::Vector3d center = p.center();

	for(unsigned int i=0; i<faces.size(); ++i)
	{
		vertexes = faces[i].vertexes();
		MR[i]
			= (*(max_element
				(vertexes.begin(),
				vertexes.end(),
				maxCR(center)     ) ) - center).length();
	}
	return *( max_element(MR.begin(), MR.end() ) );
};

njr::Vector3d InertiaTensor(const njr::NJRpolyhedra& p)
{
	njr::LinearProgramming lp;
	njr::NJRpolyhedra a,b;
	lp.Set(p);

	double Ux;
	double Lx;
	double Uy;
	double Ly;
	double Uz;
	double Lz;
	double Rx;
	double Ry;
	double Rz;
	double x;
	double y;
	double z;

	if ((lp.GetExtremeValue(Ux, Lx, Uy, Ly, Uz, Lz)) == false)
	{
		return njrdxf::ZERO;
	}

	double dVolume;
	njr::Vector3d MassCenter;
	double Ix=0.0, Iy=0.0, Iz=0.0;

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

				b.SetCenter(njr::Vector3d(x+0.5*Rx, y+0.5*Ry, z+0.5*Rz));

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

	return njr::Vector3d(Iy+Iz, Ix+Iz, Ix+Iy);
};

};   // namespace NJR
