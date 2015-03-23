#ifndef _NJR_POLYGON_H
#define _NJR_POLYGON_H

#include <njr/DataType.h>
#include <aries/utility/Vector3df.h>
#include <vector>

namespace njr
{

class NJRpolygon
{

public:

	NJRpolygon();

	explicit NJRpolygon(const std::vector<aries::Vector3df>&);

	NJRpolygon(const NJRpolygon &p);

	const NJRpolygon & operator = (const NJRpolygon &p);

	inline _float_t area() const
	{
		return _area;
	}

	inline _float_t edge_length() const
	{
		return _edge_length;
	}

	inline const std::vector<aries::Vector3df>& vertexes () const
	{
		return _vertexes;
	}

	inline aries::Vector3df center() const
	{
		return _center;
	}

	void print() const;

private:

	aries::Vector3df _center;

	_float_t _area ;

	_float_t _edge_length;

	std::vector<aries::Vector3df> _vertexes;

	// Remove the one of vertexs from two nearly vertexs
	void PureVertex();

	/**************************************************************************
	 * Use the vertexs of the polygon to calculate its center, area, and the
	 * legnth of edge; call by pure_vertex()
	 **************************************************************************/
	void CalArea();

	void CalEdgeLength();

	void CalCenter();
};

}   // namespace njr

#endif // _NJR_POLYGON_H
