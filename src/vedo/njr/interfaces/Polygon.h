#ifndef _NJR_POLYGON_H
#define _NJR_POLYGON_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vector>

namespace njr
{

class NJRpolygon
{

public:

	NJRpolygon();

	explicit NJRpolygon(const std::vector<Vector3d>&);

	NJRpolygon(const NJRpolygon &p);

	const NJRpolygon & operator = (const NJRpolygon &p);

	inline double area() const
	{
		return _area;
	};

	inline double edge_length() const
	{
		return _edge_length;
	};

	inline const std::vector<Vector3d>& vertexes () const
	{
		return _vertexes;
	};

	inline Vector3d center() const
	{
		return _center;
	};

	void print() const;

private:

	Vector3d _center;
	double _area  ;
	double _edge_length;
	std::vector<Vector3d> _vertexes;

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

};   // namespace njr

#endif // _NJR_POLYGON_H
