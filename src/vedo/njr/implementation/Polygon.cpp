#include <vedo/njr/interfaces/Polygon.h>

namespace njr
{

NJRpolygon::NJRpolygon() : _vertexes(0)
{
	_area = 0.0;
	_edge_length = 0.0;
	_center.Set(0, 0, 0);
}

NJRpolygon::NJRpolygon (const std::vector<Vector3d>& vertexes)
{
	_vertexes = vertexes;

	if ((_vertexes.size()) >= 3)
	{
		CalCenter();
		CalArea();
		CalEdgeLength();
	}
	else
	{
		_area = 0.0;
		_edge_length = 0.0;
		_center.Set(0, 0, 0);
	}

	//NJRpolygon::PureVertex();
}

NJRpolygon::NJRpolygon (const NJRpolygon &p)
{
	_area        = p._area;
	_center      = p._center;
	_edge_length = p._edge_length;
	_vertexes    = p._vertexes;
}

const NJRpolygon& NJRpolygon::operator = (const NJRpolygon &p)
{
	_area        = p._area;
	_center      = p._center;
	_edge_length = p._edge_length;
	_vertexes    = p._vertexes;
	return *this;
}

void NJRpolygon::CalCenter()
{
	register vedo::vedo_uint_t i;

	Vector3d w;

	for (i=0; i<_vertexes.size(); ++i)
	{
		w += _vertexes[i];
	}

	_center = w * ( 1.0 / ( (vedo::vedo_float_t)(_vertexes.size()) ) );
}

void NJRpolygon::CalEdgeLength()
{
	register vedo::vedo_uint_t i;
	register vedo::vedo_uint_t j;
	register vedo::vedo_uint_t size;
	_edge_length = 0.0;
	size = (vedo::vedo_uint_t) _vertexes.size();

	for (i=0; i<size; ++i)
	{
		j = (i==0) ? size-1 : i-1;
		_edge_length += (_vertexes[i]-_vertexes[j]).length();
	}
}

void NJRpolygon::CalArea()
{
	register vedo::vedo_uint_t i;
	register vedo::vedo_uint_t j;
	register vedo::vedo_uint_t size;
	Vector3d vec;
	_area = 0.0;

	size = (vedo::vedo_uint_t)(_vertexes.size()) - 2;
	j    = (vedo::vedo_uint_t)(_vertexes.size()) - 1;

	for (i=0; i<size; i++)
	{
		vec = (_vertexes[i]-_vertexes[j]) * (_vertexes[i+1]-_vertexes[j]);
		_area += (0.5*vec.length());
	}
}

void NJRpolygon::PureVertex()
{
	register vedo::vedo_uint_t i;
	register vedo::vedo_uint_t j;
	register vedo::vedo_uint_t numvertex;
    Vector3d v1;
	numvertex = (vedo::vedo_uint_t)(_vertexes.size());

	std::vector<Vector3d> tmpvec(0);

	if (numvertex < 2)
	{
		return;
	}

	std::vector<bool> mark(numvertex, true);

	for (i=0; i<(numvertex-1); ++i)
	{
		if (mark[i] != true)
		{
			continue;
		}

		for (j=i+1; j<numvertex; ++j)
		{
			if ( ((_vertexes[i]-_vertexes[j]).length()) < (0.001*_edge_length))
			{
                mark[j] = false;
			}
		}
	}
	for (i=0; i<numvertex; ++i)
	{
		if (mark[i])
		{
			tmpvec.push_back(_vertexes[i]);
		}
	}

	_vertexes = tmpvec;
}

void NJRpolygon::print() const
{
	register vedo::vedo_uint_t i;
	register vedo::vedo_uint_t numvertex;

	std::cout << "center     = " << _center      << std::endl;
	std::cout << "area       = " << _area        << std::endl;
	std::cout << "EdgeLength = " << _edge_length << std::endl;

	numvertex = (vedo::vedo_uint_t)(_vertexes.size());

	std::cout << "vertexes" << std::endl;

    for (i=0; i<numvertex; ++i)
	{
		std::cout << _vertexes[i];
	}

}

}   // namespace njr
