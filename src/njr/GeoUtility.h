#ifndef _NJR_GEOUTILITY_H
#define _NJR_GEOUTILITY_H

#include <njr/Polygon.h>
#include <njr/Polyhedra.h>
#include <aries/utility/Vector3df.h>

namespace njr
{
	_float_t Distance(const Vector3df&, const njr::HalfSpace&);

	_float_t Distance(const njr::HalfSpace&, const Vector3df&);

	_float_t Volume(const njr::NJRpolygon& bottom, const Vector3df& vertex);

	_float_t Volume(const njr::NJRpolygon& bottom, const Vector3df& vertex, Vector3df& MassCenter);

	_float_t Volume(const njr::NJRpolyhedra&);

	_float_t Volume(const njr::NJRpolyhedra&, Vector3df& MassCenter);

	_float_t CoverRadius(const njr::NJRpolyhedra&);

	Vector3df InertiaTensor(const njr::NJRpolyhedra&);
};

#endif // _NJR_GEOUTILITY
