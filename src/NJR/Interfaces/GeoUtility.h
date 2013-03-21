#ifndef _NJR_GEOUTILITY_H
#define _NJR_GEOUTILITY_H

#include <NJR/Interfaces/polygon.h>
#include <NJR/Interfaces/polyhedra.h>
#include <NJR/Interfaces/vector3d.h>

namespace NJR
{
	double Distance(const NJR::NJRvector3d&, const NJR::NJRhalfspace&);

	double Distance(const NJR::NJRhalfspace&, const NJR::NJRvector3d&);

	double Volume(const NJR::NJRpolygon& bottom, const NJR::NJRvector3d& vertex);

	double Volume
		(const NJR::NJRpolygon& bottom,
		const NJR::NJRvector3d& vertex,
		NJR::NJRvector3d& MassCenter);

	double Volume(const NJR::NJRpolyhedra&);

	double Volume(const NJR::NJRpolyhedra&, NJR::NJRvector3d& MassCenter);

	double CoverRadius(const NJR::NJRpolyhedra&);

	NJR::NJRvector3d InertiaTensor(const NJR::NJRpolyhedra&);
};

#endif // _NJR_GEOUTILITY
