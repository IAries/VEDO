#ifndef _NJR_GEOUTILITY_H
#define _NJR_GEOUTILITY_H

#include <vedo/njr/interfaces/Polygon.h>
#include <vedo/njr/interfaces/Polyhedra.h>
#include <vedo/njr/interfaces/Vector3d.h>

namespace njr
{
	vedo::vedo_float_t Distance(const njr::Vector3d&, const njr::HalfSpace&);

	vedo::vedo_float_t Distance(const njr::HalfSpace&, const njr::Vector3d&);

	vedo::vedo_float_t Volume(const njr::NJRpolygon& bottom, const njr::Vector3d& vertex);

	vedo::vedo_float_t Volume(const njr::NJRpolygon& bottom, const njr::Vector3d& vertex, njr::Vector3d& MassCenter);

	vedo::vedo_float_t Volume(const njr::NJRpolyhedra&);

	vedo::vedo_float_t Volume(const njr::NJRpolyhedra&, njr::Vector3d& MassCenter);

	vedo::vedo_float_t CoverRadius(const njr::NJRpolyhedra&);

	njr::Vector3d InertiaTensor(const njr::NJRpolyhedra&);
};

#endif // _NJR_GEOUTILITY
