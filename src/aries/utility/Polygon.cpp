#include <aries/utility/Polygon.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace aries
{

Polygon::Polygon()
{
}

const Polygon& Polygon::operator = (const Polygon& p)
{
	_Vertex       = p._Vertex;
	_Edge         = p._Edge;
	_EdgeVertexSN = p._EdgeVertexSN;
	_Face         = p._Face;
	_FaceVertexSN = p._FaceVertexSN;
	return *this;
}

Polygon::~Polygon()
{
}

const Vector3df* Polygon::GetVertex(const _uint_t& u) const
{
	if (u < (_uint_t)_Vertex.size())
	{
		return &_Vertex[u];
	}
	else
	{
		std::cout
			<< "Caution!! Code: Vector3df aries::Polygon::GetVertex(const _uint_t&) const" << std::endl
			<< "          Condition: index error!!"                                        << std::endl;
		std::exit(-1);
	}
}

void Polygon::AddVertex(const Vector3df& vPosition)
{
	_Vertex.push_back(vPosition);
}

void Polygon::UpdateVertex(const _uint_t& polygon_sn, const Vector3df& position)
{
	if (polygon_sn < (_uint_t)_Vertex.size())
	{
		_Vertex[polygon_sn] = position;
	}
	else
	{
		std::cout
			<< "Caution!! Code: void aries::Polygon::UpdateVertex(const _uint_t&, const Vector3df&)" << std::endl
			<< "          Condition: index error!!"                                                           << std::endl;
		std::exit(-1);
	}
}

std::pair<const Vector3df*, const Vector3df* > Polygon::GetEdge(const _uint_t& u) const
{
	if (u < (_uint_t)_Edge.size())
	{
		return _Edge[u];
	}
	else
	{
		std::cout
			<< "Caution!! Code: std::pair<const Vector3df*, const Vector3df* > aries::Polygon::GetEdge(const _uint_t&) const" << std::endl
			<< "          Condition: index error!!"                                                                                             << std::endl;
		std::exit(-1);
	}
}

std::pair<_uint_t, _uint_t> Polygon::GetEdgeVertexSN(const _uint_t& u) const
{
	if (u < (_uint_t)_EdgeVertexSN.size())
	{
		return _EdgeVertexSN[u];
	}
	else
	{
		std::cout
			<< "Caution!! std::pair<_uint_t, _uint_t> aries::Polygon::GetEdgeVertexSN(const _uint_t&) const" << std::endl
			<< "          Condition: index error!!"                                                          << std::endl;
		std::exit(-1);
	}
}

void Polygon::AddEdge(const Vector3df* v1, const Vector3df* v2)
{
	_Edge.push_back(std::make_pair(v1, v2));
}

void Polygon::AddEdgeVertexSN(const _uint_t& ui, const _uint_t& uj)
{
	_EdgeVertexSN.push_back(std::make_pair(ui, uj));
}

const Vector3df* Polygon::GetFace(const _uint_t& u) const
{
	if (u < (_uint_t)_Face.size())
	{
		return _Face[u][0];
	}
	else
	{
		std::cout
			<< "Caution!! Code: Vector3df* aries::Polygon::GetFace(const _uint_t&) const" << std::endl
			<< "          Condition: index error!!"                                       << std::endl;
		std::exit(-1);
	}
}

const _uint_t* Polygon::GetFaceVertexSN(const _uint_t& u) const
{
	if (u < (_uint_t)_FaceVertexSN.size())
	{
		return &(_FaceVertexSN[u][0]);
	}
	else
	{
		std::cout
			<< "Caution!! Code: const _uint_t* aries::Polygon::GetFaceVertexSN(const _uint_t&) const" << std::endl
			<< "          Condition: index error!!"                                                    << std::endl;
		std::exit(-1);
	}
}

void Polygon::AddFace(const Vector3df* FirstVertex, const Vector3df* SecondVertex, const Vector3df* ThirdVertex)
{
	std::vector<const Vector3df* > SingleFace;
	SingleFace.push_back(FirstVertex );
	SingleFace.push_back(SecondVertex);
	SingleFace.push_back(ThirdVertex );
	_Face.push_back(SingleFace);
}

void Polygon::AddFaceVertexSN(const _uint_t& ui, const _uint_t& uj, const _uint_t& uk)
{
	std::vector<_uint_t> SingleFace;
	SingleFace.push_back(ui);
	SingleFace.push_back(uj);
	SingleFace.push_back(uk);
	_FaceVertexSN.push_back(SingleFace);
}

void Polygon::CoordinateTransformation(const Vector3df& LocalX, const Vector3df& LocalY, const Vector3df& LocalZ)
{
	for (_uint_t u=0; u<(_uint_t)_Vertex.size(); u++)
	{
		_Vertex[u].CoordinateTransformation(LocalX, LocalY, LocalZ);
	}
}

void Polygon::CoordinateTransformation
	(const Vector3df& LocalX, const Vector3df& LocalY, const Vector3df& LocalZ, const Vector3df& Translation)
{
	CoordinateTransformation(LocalX, LocalY, LocalZ);
	for (_uint_t u=0; u<(_uint_t)_Vertex.size(); u++)
	{
		_Vertex[u] += Translation;
	}
}

void Polygon::Clear()
{
	_Vertex.clear();
	_Edge.clear();
	_EdgeVertexSN.clear();
	_Face.clear();
	_FaceVertexSN.clear();
}

Polygon::Polygon(const Polygon& p)
{
	*this = p;
}

}   // namespace aries



std::ostream& operator << (std::ostream& os, const aries::Polygon& p)
{
	aries::_uint_t VertexNumber = p.VertexNumber();
	for (aries::_uint_t u=0; u<VertexNumber; u++)
	{
		os << "Vertex " << u << "/" << VertexNumber << ": " << *p.GetVertex(u);
	}

	aries::_uint_t EdgeNumber = p.EdgeNumber();
	std::pair<aries::_uint_t, aries::_uint_t> uuev;
	for (aries::_uint_t u=0; u<EdgeNumber; u++)
	{
		uuev = p.GetEdgeVertexSN(u);
		os << "Edge " << u << "/" << EdgeNumber << ": Vertex " << uuev.first << "-" << uuev.second << std::endl;
	}

	aries::_uint_t FaceNumber = p.FaceNumber();
	const aries::_uint_t* ufv;
	for (aries::_uint_t u=0; u<FaceNumber; u++)
	{
		ufv = p.GetFaceVertexSN(u);
		os << "Face " << u << "/" << FaceNumber << ": Vertex " << *ufv << "-" << *(ufv+1) << "-" << *(ufv+2) << std::endl;
	}

	return os;
}
