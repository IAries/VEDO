#ifndef _NJRPOLYGON_H
#define _NJRPOLYGON_H

#include <NJR/Interfaces/vector3d.h>
#include <vector>

class NJRpolygon
{

public:

	NJRpolygon();

	explicit NJRpolygon(const std::vector<NJRvector3d>&);

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

	inline const std::vector<NJRvector3d>& vertexes () const
	{
		return _vertexes;
	};

	inline NJRvector3d center() const
	{
		return _center;
	};

	void print() const;

private:

	NJRvector3d _center;
	double _area  ;
	double _edge_length;
	std::vector<NJRvector3d> _vertexes;

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

#endif // _NJRPOLYGON_H
