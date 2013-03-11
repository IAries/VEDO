#include <NJR/Interfaces/polygon.h>

using namespace std;

NJRpolygon::NJRpolygon() : _vertexes(0)
{
	_area = 0.0;
	_edge_length = 0.0;
	_center.Set(0, 0, 0);
};

NJRpolygon::NJRpolygon (const vector<NJRvector3d>& vertexes)
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
	};

	//NJRpolygon::PureVertex();
};

NJRpolygon::NJRpolygon (const NJRpolygon &p)
{
	_area        = p._area;
	_center      = p._center;
	_edge_length = p._edge_length;
	_vertexes    = p._vertexes;
};

const NJRpolygon& NJRpolygon::operator = (const NJRpolygon &p)
{
	_area        = p._area;
	_center      = p._center;
	_edge_length = p._edge_length;
	_vertexes    = p._vertexes;
	return *this;
};

void NJRpolygon::CalCenter()
{
	register unsigned int i;

	NJRvector3d w(ZERO);

	for (i=0; i<_vertexes.size(); ++i)
	{
		w += _vertexes[i];
	}

	_center = w * ( 1.0 / ( (double)(_vertexes.size()) ) );
};


void NJRpolygon::CalEdgeLength()
{
	register unsigned int i;
	register unsigned int j;
	register unsigned int size;
	_edge_length = 0.0;
	size = (unsigned int) _vertexes.size();

	for (i=0; i<size; ++i)
	{
		j = (i==0) ? size-1 : i-1;
		_edge_length += (_vertexes[i]-_vertexes[j]).length();
	}
};

void NJRpolygon::CalArea()
{
	register unsigned int i;
	register unsigned int j;
	register unsigned int size;
	NJRvector3d vec;
	_area = 0.0;

	size = (unsigned int)(_vertexes.size()) - 2;
	j    = (unsigned int)(_vertexes.size()) - 1;

	for (i=0; i<size; i++)
	{
		vec = (_vertexes[i]-_vertexes[j]) * (_vertexes[i+1]-_vertexes[j]);
		_area += (0.5*vec.length());
	}
};

void NJRpolygon::PureVertex()
{
	register unsigned int i;
	register unsigned int j;
	register unsigned int numvertex;
    NJRvector3d v1;
	numvertex = (unsigned int)(_vertexes.size());

	vector<NJRvector3d> tmpvec(0);

	if (numvertex < 2)
	{
		return;
	}

	vector<bool> mark(numvertex, true);

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
};

void NJRpolygon::print() const
{
	register unsigned int i;
	register unsigned int numvertex;

	cout << "center     = " << _center      << endl;
	cout << "area       = " << _area        << endl;
	cout << "EdgeLength = " << _edge_length << endl;

	numvertex = (unsigned int)(_vertexes.size());

	cout << "vertexes" << endl;

    for (i=0; i<numvertex; ++i)
	{
		cout << _vertexes[i];
	}

};
