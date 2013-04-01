#ifndef _NJR_GEOUTILITY_H
#define _NJR_GEOUTILITY_H

#include <NJR/Interfaces/Polygon.h>
#include <NJR/Interfaces/Polyhedra.h>
#include <NJR/Interfaces/Vector3d.h>

namespace njr
{
	double Distance(const njr::Vector3d&, const njr::HalfSpace&);

	double Distance(const njr::HalfSpace&, const njr::Vector3d&);

	double Volume(const njr::NJRpolygon& bottom, const njr::Vector3d& vertex);

	double Volume
		(const njr::NJRpolygon& bottom,
		const njr::Vector3d& vertex,
		njr::Vector3d& MassCenter);

	double Volume(const njr::NJRpolyhedra&);

	double Volume(const njr::NJRpolyhedra&, njr::Vector3d& MassCenter);

	double CoverRadius(const njr::NJRpolyhedra&);

	njr::Vector3d InertiaTensor(const njr::NJRpolyhedra&);
};

#endif // _NJR_GEOUTILITY
