#ifndef _ARIES_POLYGON_H
#define _ARIES_POLYGON_H

#include <aries/utility/DataType.h>
#include <aries/utility/Vector3df.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace aries
{

class Polygon
{

public:

	Polygon();

	const Polygon& operator = (const Polygon&);

	~Polygon();

	inline _uint_t VertexNumber() const
	{
		return (_uint_t)_Vertex.size();
	}

	const Vector3df* GetVertex(const _uint_t&) const;

	void AddVertex(const Vector3df& vPosition);

	void UpdateVertex(const _uint_t& polygon_sn, const Vector3df& position);

	inline _uint_t EdgeNumber() const
	{
		return (_uint_t)_Edge.size();
	}

	std::pair<const Vector3df*, const Vector3df* > GetEdge(const _uint_t&) const;

	std::pair<_uint_t, _uint_t> GetEdgeVertexSN(const _uint_t&) const;

	void AddEdge(const Vector3df* v1, const Vector3df* v2);

	void AddEdgeVertexSN(const _uint_t& ui, const _uint_t& uj);

	inline _uint_t FaceNumber() const
	{
		return (_uint_t)_Face.size();
	}

	const Vector3df* GetFace(const _uint_t&) const;

	const _uint_t* GetFaceVertexSN(const _uint_t&) const;

	void AddFace(const Vector3df* FirstVertex, const Vector3df* SecondVertex, const Vector3df* ThirdVertex);

	void AddFaceVertexSN(const _uint_t& ui, const _uint_t& uj, const _uint_t& uk);

	void CoordinateTransformation(const Vector3df& LocalX, const Vector3df& LocalY, const Vector3df& LocalZ);

	void CoordinateTransformation
		(const Vector3df& LocalX, const Vector3df& LocalY, const Vector3df& LocalZ, const Vector3df& Translation);

	void Clear();

protected:

	Polygon(const Polygon& p);

	std::vector<Vector3df> _Vertex;

	std::vector<std::pair<const Vector3df*, const Vector3df* > > _Edge;

	std::vector<std::pair<_uint_t, _uint_t> > _EdgeVertexSN;

	std::vector<std::vector<const Vector3df* > > _Face;

	std::vector<std::vector<_uint_t> > _FaceVertexSN;
};

}   // namespace aries



std::ostream& operator << (std::ostream& os, const aries::Polygon& p);

#endif   // _ARIES_POLYGON_H
