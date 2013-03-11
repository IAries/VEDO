#ifndef _NJR_GEOUTILITY_H
#define _NJR_GEOUTILITY_H

#include <NJR/Interfaces/polygon.h>
#include <NJR/Interfaces/polyhedra.h>
#include <NJR/Interfaces/vector3d.h>

namespace NJRGEO
{
	double Distance(const NJRvector3d&, const NJRhalfspace&);

	double Distance(const NJRhalfspace&, const NJRvector3d&);

	double Volume(const NJRpolygon& bottom, const NJRvector3d& vertex);

	double Volume
		(const NJRpolygon& bottom,
		const NJRvector3d& vertex,
		NJRvector3d& MassCenter);

	double Volume(const NJRpolyhedra&);

	double Volume(const NJRpolyhedra&, NJRvector3d& MassCenter);

	double CoverRadius(const NJRpolyhedra&);

	NJRvector3d InertiaTensor (const NJRpolyhedra&);

};

#endif // _NJR_GEOUTILITY
