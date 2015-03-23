#include <njr/Polygon.h>

namespace njr
{

NJRpolygon::NJRpolygon() : _vertexes(0)
{
	_area = 0.0;
	_edge_length = 0.0;
	_center.set(0.0, 0.0, 0.0);
}

NJRpolygon::NJRpolygon (const std::vector<aries::Vector3df>& vertexes)
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
		_center.set(0, 0, 0);
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
	register _uint_t i;

	aries::Vector3df w;

	for (i=0; i<_vertexes.size(); ++i)
	{
		w += _vertexes[i];
	}

	_center = w * ( 1.0 / ( (_float_t)(_vertexes.size()) ) );
}

void NJRpolygon::CalEdgeLength()
{
	register _uint_t i;
	register _uint_t j;
	register _uint_t size;
	_edge_length = 0.0;
	size = (_uint_t) _vertexes.size();

	for (i=0; i<size; ++i)
	{
		j = (i==0) ? size-1 : i-1;
		_edge_length += (_vertexes[i]-_vertexes[j]).length();
	}
}

void NJRpolygon::CalArea()
{
	register _uint_t i;
	register _uint_t j;
	register _uint_t size;
	aries::Vector3df vec;
	_area = 0.0;

	size = (_uint_t)(_vertexes.size()) - 2;
	j    = (_uint_t)(_vertexes.size()) - 1;

	for (i=0; i<size; i++)
	{
		vec = (_vertexes[i]-_vertexes[j]).cross(_vertexes[i+1]-_vertexes[j]);
		_area += (0.5*vec.length());
	}
}

void NJRpolygon::PureVertex()
{
	register _uint_t i;
	register _uint_t j;
	register _uint_t numvertex;
    aries::Vector3df v1;
	numvertex = (_uint_t)(_vertexes.size());

	std::vector<aries::Vector3df> tmpvec(0);

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
	register _uint_t i;
	register _uint_t numvertex;

	std::cout << "center     = " << _center      << std::endl;
	std::cout << "area       = " << _area        << std::endl;
	std::cout << "EdgeLength = " << _edge_length << std::endl;

	numvertex = (_uint_t)(_vertexes.size());

	std::cout << "vertexes" << std::endl;

    for (i=0; i<numvertex; ++i)
	{
		std::cout << _vertexes[i];
	}

}

}   // namespace njr
