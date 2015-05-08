#include <njr/Polyhedra.h>
#include <njr/RandomGenerator.h>
#include <njr/LinearProgramming.h>
#include <algorithm>
#include <functional>

namespace njr
{

NJRpolyhedra::NJRpolyhedra(): _center(Vector3df()), _constrains(0)
{
}

NJRpolyhedra::NJRpolyhedra(const NJRpolyhedra& poly3d)
{
	_center = poly3d._center;
	_constrains = poly3d._constrains;
}

void NJRpolyhedra::AddConstrain(const njr::HalfSpace& poly3d)
{
	_constrains.push_back(poly3d);
}

void NJRpolyhedra::Clear()
{
	_constrains.clear();
}

void NJRpolyhedra::Purge()
{
	register _uint_t i;
	NJRpolyhedra poly;
	LinearProgramming lp;
	std::vector<njr::HalfSpace> NewConstrains(0);

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
}

bool NJRpolyhedra::Check()
{
	LinearProgramming lp;
	lp.Set(*this);
	return lp.Check();
}

NJRpolyhedra NJRpolyhedra::operator + (const NJRpolyhedra &poly3d) const
{
    NJRpolyhedra com(poly3d);
    com._center = (_center+poly3d._center) * 0.5;
	std::vector<njr::HalfSpace>::const_iterator ihf;

	for (ihf=_constrains.begin(); ihf!=_constrains.end(); ++ihf)
	{
		com._constrains.push_back(*ihf);
	}

    return (com);
}

NJRpolyhedra& NJRpolyhedra::operator = (const NJRpolyhedra & poly3d)
{
	_center = poly3d._center;
	_constrains = poly3d._constrains;
	return *this;
}

void NJRpolyhedra::Translate(const Vector3df &dp)
{
	register _uint_t i;

	for (i=0; i<_constrains.size(); ++i)
	{
		_constrains[i].Translate(dp);
	}

	_center += dp ;
}

NJRpolyhedra NJRpolyhedra::Mapping(const Vector3df& center, const Vector3df& vOX, const Vector3df& vOZ) const
{
	register _uint_t i;
	Vector3df LX = vOX.direction();
    Vector3df LZ = vOZ.direction();
    Vector3df LY = LZ.cross(LX);
	Vector3df g;

	NJRpolyhedra p(*this);

	for (i=0; i<p._constrains.size(); ++i)
	{
		p._constrains[i].Translate(-p._center);
		g.set(p._constrains[i].a(), p._constrains[i].b(), p._constrains[i].c());
		g = g.trans(LX,LY,LZ);
		p._constrains[i].Set(g.x(), g.y(), g.z(), p._constrains[i].sense(), p._constrains[i].d());
		p._constrains[i].Translate(center);
	}

	p._center = center;

	return p;
}

NJRpolyhedra NJRpolyhedra::CoverPolyhedra () const
{
	register _uint_t i;

	NJRpolyhedra p(*this);

	p.Translate(-p._center);

	for (i=0; i < p._constrains.size(); ++i)
	{
		p._constrains[i].Set
			(p._constrains[i].a()*0.99, p._constrains[i].b()*0.99, p._constrains[i].c()*0.99,
			 p._constrains[i].sense(), p._constrains[i].d()                                  );
	}

	p.Translate(_center);

	return p;
}

void NJRpolyhedra::Normalize()
{
	for_each(_constrains.begin(), _constrains.end(), std::mem_fun_ref(&njr::HalfSpace::Normalize));
}

void NJRpolyhedra::RotateAround(const Vector3df &dv)
{
	register _uint_t i;

	for (i=0; i<_constrains.size(); ++i)
	{
		_constrains[i].Translate(-_center);
		_constrains[i].RotateAround(dv);
		_constrains[i].Translate(_center);
	}
}

std::vector<NJRpolygon> NJRpolyhedra::faces() const
{
	register _uint_t i;
	NJRpolyhedra poly;
	LinearProgramming lp;
	std::vector<NJRpolygon> faces(0);

	for (i=0 ; i<_constrains.size(); ++i)
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
}

void NJRpolyhedra::SetCubic(Vector3df c, _float_t length)
{
	_center.set(0.0, 0.0, 0.0);
	_constrains.clear();

	_constrains.push_back(njr::HalfSpace(1, 0, 0, L,  0.5*length));
	_constrains.push_back(njr::HalfSpace(1, 0, 0, G, -0.5*length));
	_constrains.push_back(njr::HalfSpace(0, 1, 0, L,  0.5*length));
	_constrains.push_back(njr::HalfSpace(0, 1, 0, G, -0.5*length));
	_constrains.push_back(njr::HalfSpace(0, 0, 1, L,  0.5*length));
	_constrains.push_back(njr::HalfSpace(0, 0, 1, G, -0.5*length));

	NJRpolyhedra::Translate(c);
}

void NJRpolyhedra::SetPlane
	(_float_t a, _float_t b, _float_t c, Sense eqsense, _float_t d)
{
	_center.set(0.0, 0.0, 0.0);
	_constrains.clear();
    _constrains.push_back(njr::HalfSpace(a, b, c, eqsense, d));
}

void NJRpolyhedra::SetRetangular(Vector3df c, _float_t lx, _float_t ly, _float_t lz)
{
	_center.set(0.0, 0.0, 0.0);
	_constrains.clear();
	_constrains.push_back(njr::HalfSpace(1, 0, 0, L,  0.5*lx));
	_constrains.push_back(njr::HalfSpace(1, 0, 0, G, -0.5*lx));
	_constrains.push_back(njr::HalfSpace(0, 1, 0, L,  0.5*ly));
	_constrains.push_back(njr::HalfSpace(0, 1, 0, G, -0.5*ly));
	_constrains.push_back(njr::HalfSpace(0, 0, 1, L,  0.5*lz));
	_constrains.push_back(njr::HalfSpace(0, 0, 1, G, -0.5*lz));
	NJRpolyhedra::Translate(c);
}

void NJRpolyhedra::SetRandom(Vector3df c, _uint_t n, _float_t radius)
{
	//_float_t scale = radius/1.732050807;
	_center.set(0.0, 0.0, 0.0);
	_constrains.clear();

	njr::HalfSpace halfspace;
	RandomGenerator random(2004);
	_uint_t i;

	_constrains.push_back(njr::HalfSpace(1,0,0,L, radius));
	_constrains.push_back(njr::HalfSpace(1,0,0,G,-radius));
	_constrains.push_back(njr::HalfSpace(0,1,0,L, radius));
	_constrains.push_back(njr::HalfSpace(0,1,0,G,-radius));
	_constrains.push_back(njr::HalfSpace(0,0,1,L, radius));
	_constrains.push_back(njr::HalfSpace(0,0,1,G,-radius));

	for ( i = 6 ; i < n ; ++i)
	{
		if (random.GeneratingInteger(0,1))
		{
			halfspace.Set (1.0, 0.0, 0.0, L, radius);
			halfspace.RotateAround
				(Vector3df
					(random.GeneratingDouble(1.0, -1.0),
					 random.GeneratingDouble(1.0, -1.0),
					 random.GeneratingDouble(1.0, -1.0) ) );
		}
		else
		{
			halfspace.Set (1.0, 0.0, 0.0, G, -radius);
			halfspace.RotateAround
				(Vector3df
					(random.GeneratingDouble(1.0, -1.0),
					 random.GeneratingDouble(1.0, -1.0),
					 random.GeneratingDouble(1.0, -1.0) ) );
		}
		_constrains.push_back(halfspace);
	}

	NJRpolyhedra::Translate(c);
}

void NJRpolyhedra::SetIcosahedron(Vector3df c, _float_t radius)
{
	_float_t scale;
	scale = radius/11.18035;

	_center.set(0.0, 0.0, 0.0);
	_constrains.clear();

	_constrains.push_back(njr::HalfSpace( 58.778525/scale,   42.705098/scale,  95.105652/scale, G, -1063.313510));
	_constrains.push_back(njr::HalfSpace( 58.778525/scale,   42.705098/scale,  95.105652/scale, L,  1063.313510));
	_constrains.push_back(njr::HalfSpace(-22.451399/scale,   69.098301/scale,  95.105652/scale, G, -1063.313511));
	_constrains.push_back(njr::HalfSpace(-22.451399/scale,   69.098301/scale,  95.105652/scale, L,  1063.313511));
	_constrains.push_back(njr::HalfSpace(-72.654253/scale,    0.0           ,  95.105652/scale, G, -1063.313511));
	_constrains.push_back(njr::HalfSpace(-72.654253/scale,    0.0           ,  95.105652/scale, L,  1063.313511));
	_constrains.push_back(njr::HalfSpace(-22.451399/scale,  -69.098301/scale,  95.105652/scale, G, -1063.313510));
	_constrains.push_back(njr::HalfSpace(-22.451399/scale,  -69.098301/scale,  95.105652/scale, L,  1063.313511));
	_constrains.push_back(njr::HalfSpace( 58.778525/scale,  -42.705098/scale,  95.105652/scale, G, -1063.313510));
	_constrains.push_back(njr::HalfSpace( 58.778525/scale,  -42.705098/scale,  95.105652/scale, L,  1063.313510));
	_constrains.push_back(njr::HalfSpace(-95.105652/scale,  -69.098301/scale, -22.451399/scale, G, -1063.313510));
	_constrains.push_back(njr::HalfSpace(-95.105652/scale,  -69.098301/scale, -22.451399/scale, L,  1063.313510));
	_constrains.push_back(njr::HalfSpace( 36.327126/scale, -111.803399/scale, -22.451399/scale, G, -1063.313511));
	_constrains.push_back(njr::HalfSpace( 36.327126/scale, -111.803399/scale, -22.451399/scale, L,  1063.313511));
	_constrains.push_back(njr::HalfSpace(117.557051/scale,    0.0           , -22.451399/scale, G, -1063.313511));
	_constrains.push_back(njr::HalfSpace(117.557051/scale,    0.0           , -22.451399/scale, L,  1063.313511));
	_constrains.push_back(njr::HalfSpace( 36.327126/scale,  111.803399/scale, -22.451399/scale, G, -1063.313510));
	_constrains.push_back(njr::HalfSpace( 36.327126/scale,  111.803399/scale, -22.451399/scale, L,  1063.313510));
	_constrains.push_back(njr::HalfSpace(-95.105652/scale,   69.098300/scale, -22.451399/scale, G, -1063.313510));
	_constrains.push_back(njr::HalfSpace(-95.105652/scale,   69.098300/scale, -22.451399/scale, L,  1063.313510));

	NJRpolyhedra::Normalize();

	NJRpolyhedra::Translate(c);

}

void NJRpolyhedra::print() const
{
	register _uint_t i;
	std::cout << "_center = " << std::endl << _center;
	for (i=0; i<_constrains.size(); ++i)
	{
		std::cout << "[" << i << "]" << _constrains[i];
	}
}

}   // namespace njr
