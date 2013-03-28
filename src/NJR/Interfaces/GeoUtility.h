#ifndef _NJR_GEOUTILITY_H
#define _NJR_GEOUTILITY_H

#include <NJR/Interfaces/Polygon.h>
#include <NJR/Interfaces/Polyhedra.h>
#include <NJR/Interfaces/Vector3d.h>

namespace NJR
{
	double Distance(const NJR::Vector3d&, const NJR::HalfSpace&);

	double Distance(const NJR::HalfSpace&, const NJR::Vector3d&);

	double Volume(const NJR::NJRpolygon& bottom, const NJR::Vector3d& vertex);

	double Volume
		(const NJR::NJRpolygon& bottom,
		const NJR::Vector3d& vertex,
		NJR::Vector3d& MassCenter);

	double Volume(const NJR::NJRpolyhedra&);

	double Volume(const NJR::NJRpolyhedra&, NJR::Vector3d& MassCenter);

	double CoverRadius(const NJR::NJRpolyhedra&);

	NJR::Vector3d InertiaTensor(const NJR::NJRpolyhedra&);
};

#endif // _NJR_GEOUTILITY
