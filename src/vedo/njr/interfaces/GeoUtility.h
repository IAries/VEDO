#ifndef _NJR_GEOUTILITY_H
#define _NJR_GEOUTILITY_H

#include <vedo/njr/interfaces/Polygon.h>
#include <vedo/njr/interfaces/Polyhedra.h>
#include <vedo/njr/interfaces/Vector3d.h>

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
