#include <NJR/Interfaces/GeoUtility.h>
#include <NJR/Interfaces/vector3d.h>
#include <NJR/Interfaces/LinearProgramming.h>
#include <algorithm>
#include <cmath>

using namespace std;

double NJRGEO::Distance(const NJRvector3d& v, const NJRhalfspace& hf)
{
	return NJRGEO::Distance(hf, v);
};

double NJRGEO::Distance(const NJRhalfspace& hf, const NJRvector3d& v)
{
	return (fabs( hf.a()*v.x() + hf.b()*v.y() + hf.c()*v.z() - hf.d() ) /
			sqrt ( hf.a()*hf.a() + hf.b()*hf.b() + hf.c()*hf.c() )       );
};

double NJRGEO::Volume(const NJRpolygon& bottom, const NJRvector3d& vertex)
{
	if ( (bottom.vertexes().size())<3 )
	{
		return 0;
	}

	vector<NJRvector3d> vertexes = bottom.vertexes();

	NJRvector3d n = (vertexes[1]-vertexes[0]) * (vertexes[2]-vertexes[0]);

	double d = NJRGEO::Distance
		(NJRhalfspace(n.x(), n.y(), n.z(), E, n%vertexes[0]), vertex);

	return d*bottom.area()/3.0;
};

double NJRGEO::Volume
	(const NJRpolygon& bottom,
	const NJRvector3d& vertex,
	NJRvector3d& MassCenter)
{

	if ( (bottom.vertexes().size()) < 3)
	{
		return 0;
	}

	NJRvector3d center = bottom.center();

	vector<NJRvector3d> vertexes = bottom.vertexes();

	NJRvector3d mr(ZERO);

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


double NJRGEO::Volume(const NJRpolyhedra& poly)
{
	vector<NJRpolygon> faces = poly.faces();

	double volume = 0;

	for (unsigned int i=0; i<faces.size(); ++i)
	{
		volume += NJRGEO::Volume(faces[i], poly.center());
	}

	return volume;
};

double NJRGEO::Volume(const NJRpolyhedra& poly, NJRvector3d& MassCenter)
{
	vector<NJRpolygon> faces = poly.faces();

	double volume = 0;
	NJRvector3d center;
	NJRvector3d mr(ZERO);

	for (unsigned int i=1; i<faces.size(); ++i)
	{
		double vol = NJRGEO::Volume(faces[i], faces[0].center(), center);
		mr += center*vol;
		volume += vol;
	}

	MassCenter = mr*(1.0/volume);
	return volume;
};



class maxCR
{
private:

	const NJRvector3d center;

public:

	explicit maxCR(const NJRvector3d& c) : center (c)
	{
	};

	bool operator() (const NJRvector3d& a, const NJRvector3d& b)
	{
		return ( ((a-center).length()) < ((b-center).length()) );
	};
};

double NJRGEO::CoverRadius(const NJRpolyhedra& p)
{

	vector<NJRpolygon> faces = p.faces();
	vector<NJRvector3d> vertexes;
	vector<double> MR(faces.size());
	NJRvector3d center = p.center();

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

NJRvector3d NJRGEO::InertiaTensor (const NJRpolyhedra& p)
{
	NJRLinearProgramming lp;
	NJRpolyhedra a,b;
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
		return ZERO;
	}

	double Volume;
	NJRvector3d MassCenter;
	double Ix=0.0, Iy=0.0, Iz=0.0;

	a = p;
	NJRGEO::Volume(a,MassCenter);
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

				b.AddConstrain(NJRhalfspace(1, 0, 0, G, x));
				b.AddConstrain(NJRhalfspace(1, 0, 0, L, x+Rx));
				b.AddConstrain(NJRhalfspace(0, 1, 0, G, y));
				b.AddConstrain(NJRhalfspace(0, 1, 0, L, y+Ry));
				b.AddConstrain(NJRhalfspace(0, 0, 1, G, z));
				b.AddConstrain(NJRhalfspace(0, 0, 1, L, z+Rz));

				b.SetCenter(NJRvector3d(x+0.5*Rx, y+0.5*Ry, z+0.5*Rz));

				Volume = NJRGEO::Volume(b,MassCenter);

				if (Volume != 0)
				{
					Ix += Volume*MassCenter.x()*MassCenter.x();
					Iy += Volume*MassCenter.y()*MassCenter.y();
					Iz += Volume*MassCenter.z()*MassCenter.z();
				}
			}
		}
	}

	return NJRvector3d(Iy+Iz, Ix+Iz, Ix+Iy);
};
