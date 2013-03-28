#include <NJR/Interfaces/Polyhedra.h>
#include <NJR/Interfaces/RandomGenerator.h>
#include <NJR/Interfaces/LinearProgramming.h>
#include <algorithm>
#include <functional>

namespace NJR
{

NJRpolyhedra::NJRpolyhedra() : _center(NJRDXF::ORIGIN), _constrains(0)
{
};

NJRpolyhedra::NJRpolyhedra(const NJRpolyhedra& poly3d)
{
	_center = poly3d._center;
	_constrains = poly3d._constrains;
};

void NJRpolyhedra::AddConstrain(const NJR::HalfSpace& poly3d)
{
	_constrains.push_back(poly3d);
};

void NJRpolyhedra::Clear()
{
	_constrains.clear();
};

void NJRpolyhedra::Purge()
{
	register unsigned int i;
	NJRpolyhedra poly;
	LinearProgramming lp;
	std::vector<NJR::HalfSpace> NewConstrains(0);

	for (i=0; i<_constrains.size(); ++i)
	{
		poly = *this;
		poly._constrains[i].SetSense2E();
 		lp.Set(poly);

		if (lp.Check() == true)
		{
			NewConstrains.push_back(_constrains[i]);
		}
	}

	_constrains = NewConstrains;
};

bool NJRpolyhedra::Check()
{
	LinearProgramming lp;
	lp.Set(*this);
	return lp.Check();
};

NJRpolyhedra NJRpolyhedra::operator +
	(const NJRpolyhedra &poly3d) const
{
    NJRpolyhedra com(poly3d);
    com._center = (_center+poly3d._center) * 0.5;
	std::vector<NJR::HalfSpace>::const_iterator ihf;

	for (ihf=_constrains.begin(); ihf!=_constrains.end(); ++ihf)
	{
		com._constrains.push_back(*ihf);
	}

    return (com);
};

NJRpolyhedra& NJRpolyhedra::operator = (const NJRpolyhedra & poly3d)
{
	_center = poly3d._center;
	_constrains = poly3d._constrains;
	return *this;
};

void NJRpolyhedra::Translate (const Vector3d &dp)
{
	register unsigned int i;

	for (i=0; i<_constrains.size(); ++i)
	{
		_constrains[i].Translate(dp);
	}

	_center += dp ;
};

NJRpolyhedra NJRpolyhedra::Mapping
	(const Vector3d& center, const Vector3d& vOX, const Vector3d& vOZ)
	const
{
	register unsigned int i;
	Vector3d LX = vOX.direction();
    Vector3d LZ = vOZ.direction();
    Vector3d LY = LZ*LX;
	Vector3d g;

	NJRpolyhedra p(*this);

	for (i=0; i<p._constrains.size(); ++i)
	{
		p._constrains[i].Translate(-p._center);

		g.Set(p._constrains[i].a(), p._constrains[i].b(), p._constrains[i].c());

		g = g.Trans(LX,LY,LZ);

		p._constrains[i].Set
			(g.x(),
			g.y(),
			g.z(),
			p._constrains[i].sense(),
			p._constrains[i].d());

		p._constrains[i].Translate(center);
	}

	p._center = center;

	return p;
};

NJRpolyhedra NJRpolyhedra::CoverPolyhedra () const
{
	register unsigned int i;

	NJRpolyhedra p(*this);

	p.Translate(-p._center);

	for ( i=0; i < p._constrains.size(); ++i)
	{
		p._constrains[i].Set
			(p._constrains[i].a()*0.99,
			p._constrains[i].b()*0.99,
			p._constrains[i].c()*0.99,
			p._constrains[i].sense(),
			p._constrains[i].d());
	}

	p.Translate(_center);

	return p;
};

void NJRpolyhedra::Normalize()
{
	for_each
		(_constrains.begin(),
		_constrains.end(),
		std::mem_fun_ref(&NJR::HalfSpace::Normalize));
};

void NJRpolyhedra::RotateAround(const Vector3d &dv)
{
	register unsigned int i;

	for (i=0; i<_constrains.size(); ++i)
	{
		_constrains[i].Translate(-_center);
		_constrains[i].RotateAround(dv);
		_constrains[i].Translate(_center);
	}
};

std::vector<NJRpolygon> NJRpolyhedra::faces() const
{
	register unsigned int i;
	NJRpolyhedra poly;
	LinearProgramming lp;
	std::vector<NJRpolygon> faces(0);

	for ( i=0 ; i < _constrains.size() ; ++i)
	{
		poly = *this;
		poly._constrains[i].SetSense2E();
		lp.Set(poly);
		lp.Check();
		if (lp.ISPolygon() == true)
		{
			faces.push_back(lp.GetPolygon());
		}
	}
	return faces;
};

void NJRpolyhedra::SetCubic(Vector3d c, double length)
{
	_center = NJRDXF::ORIGIN;
	_constrains.clear();

	_constrains.push_back(NJR::HalfSpace(1, 0, 0, L,  length/2.0));
	_constrains.push_back(NJR::HalfSpace(1, 0, 0, G, -length/2.0));
	_constrains.push_back(NJR::HalfSpace(0, 1, 0, L,  length/2.0));
	_constrains.push_back(NJR::HalfSpace(0, 1, 0, G, -length/2.0));
	_constrains.push_back(NJR::HalfSpace(0, 0, 1, L,  length/2.0));
	_constrains.push_back(NJR::HalfSpace(0, 0, 1, G, -length/2.0));

	NJRpolyhedra::Translate(c);
};

void NJRpolyhedra::SetPlane
	(double a, double b, double c, Sense eqsense, double d)
{
	_center=NJRDXF::ORIGIN;
	_constrains.clear();
    _constrains.push_back(NJR::HalfSpace(a, b, c, eqsense, d));
};

void NJRpolyhedra::SetRetangular(Vector3d c, double lx, double ly, double lz)
{
	_center=NJRDXF::ORIGIN;
	_constrains.clear();
	_constrains.push_back(NJR::HalfSpace(1, 0, 0, L,  lx/2.0));
	_constrains.push_back(NJR::HalfSpace(1, 0, 0, G, -lx/2.0));
	_constrains.push_back(NJR::HalfSpace(0, 1, 0, L,  ly/2.0));
	_constrains.push_back(NJR::HalfSpace(0, 1, 0, G, -ly/2.0));
	_constrains.push_back(NJR::HalfSpace(0, 0, 1, L,  lz/2.0));
	_constrains.push_back(NJR::HalfSpace(0, 0, 1, G, -lz/2.0));
	NJRpolyhedra::Translate(c);
};

void NJRpolyhedra::SetRandom
	(Vector3d c,
	unsigned int n,
	double radius)
{
	double scale = radius/1.732050807;
	_center=NJRDXF::ORIGIN;
	_constrains.clear();

	NJR::HalfSpace halfspace;
	RandomGenerator random(2004);
	unsigned int i;

	_constrains.push_back(NJR::HalfSpace(1,0,0,L, radius));
	_constrains.push_back(NJR::HalfSpace(1,0,0,G,-radius));
	_constrains.push_back(NJR::HalfSpace(0,1,0,L, radius));
	_constrains.push_back(NJR::HalfSpace(0,1,0,G,-radius));
	_constrains.push_back(NJR::HalfSpace(0,0,1,L, radius));
	_constrains.push_back(NJR::HalfSpace(0,0,1,G,-radius));

	for ( i = 6 ; i < n ; ++i)
	{
		if (random(0,1))
		{
			halfspace.Set (1.0, 0.0, 0.0, L, radius);
			halfspace.RotateAround
				(Vector3d
					(random(1.0,-1.0), random(1.0,-1.0), random(1.0,-1.0) ) );
		}
		else
		{
			halfspace.Set (1.0, 0.0, 0.0, G, -radius);
			halfspace.RotateAround
				(Vector3d
					(random(1.0,-1.0), random(1.0,-1.0), random(1.0,-1.0) ) );
		}
		_constrains.push_back(halfspace);
	}

	NJRpolyhedra::Translate(c);
};

void NJRpolyhedra::SetIcosahedron(Vector3d c, double radius)
{
	double scale;
	scale = radius/11.18035;

	_center = NJRDXF::ORIGIN;
	_constrains.clear();

	_constrains.push_back
		(NJR::HalfSpace
			(58.778525/scale,
			42.705098/scale,
			95.105652/scale,
			G,
			-1063.313510     ) );

	_constrains.push_back
		(NJR::HalfSpace
			(58.778525/scale,
			42.705098/scale,
			95.105652/scale,
			L,
			1063.313510       ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-22.451399/scale,
			69.098301/scale,
			95.105652/scale,
			G,
			-1063.313511      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-22.451399/scale,
			69.098301/scale,
			95.105652/scale,
			L,
			1063.313511       ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-72.654253/scale,
			0.0,
			95.105652/scale,
			G,
			-1063.313511      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-72.654253/scale,
			0.0,
			95.105652/scale,
			L,
			1063.313511       ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-22.451399/scale,
			-69.098301/scale,
			95.105652/scale,
			G,
			-1063.313510      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-22.451399/scale,
			-69.098301/scale,
			95.105652/scale,
			L,
			1063.313511       ) );

	_constrains.push_back
		(NJR::HalfSpace
			(58.778525/scale,
			-42.705098/scale,
			95.105652/scale,
			G,
			-1063.313510     ) );

	_constrains.push_back
		(NJR::HalfSpace
			(58.778525/scale,
			-42.705098/scale,
			95.105652/scale,
			L,
			1063.313510      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-95.105652/scale,
			-69.098301/scale,
			-22.451399/scale,
			G,
			-1063.313510      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-95.105652/scale,
			-69.098301/scale,
			-22.451399/scale,
			L,
			1063.313510       ) );

	_constrains.push_back
		(NJR::HalfSpace
			(36.327126/scale,
			-111.803399/scale,
			-22.451399/scale,
			G,
			-1063.313511      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(36.327126/scale,
			-111.803399/scale,
			-22.451399/scale,
			L,
			1063.313511       ) );

	_constrains.push_back
		(NJR::HalfSpace
			(117.557051/scale,
			0.0,
			-22.451399/scale,
			G,
			-1063.313511      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(117.557051/scale,
			0.0,
			-22.451399/scale,
			L,
			1063.313511       ) );

	_constrains.push_back
		(NJR::HalfSpace
			(36.327126/scale,
			111.803399/scale,
			-22.451399/scale,
			G,
			-1063.313510     ) );

	_constrains.push_back
		(NJR::HalfSpace
			(36.327126/scale,
			111.803399/scale,
			-22.451399/scale,
			L,
			1063.313510      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-95.105652/scale,
			69.098300/scale,
			-22.451399/scale,
			G,
			-1063.313510      ) );

	_constrains.push_back
		(NJR::HalfSpace
			(-95.105652/scale,
			69.098300/scale,
			-22.451399/scale,
			L,
			1063.313510       ) );

	NJRpolyhedra::Normalize();

	NJRpolyhedra::Translate(c);

};

void NJRpolyhedra::print() const
{
	register unsigned int i;

	std::cout << "_center = " << std::endl << _center;

	for (i=0; i<_constrains.size(); ++i)
	{
		std::cout << "[" << i << "]" << _constrains[i];
	}
};

};   // namespace NJR