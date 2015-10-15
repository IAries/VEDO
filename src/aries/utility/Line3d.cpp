#include <aries/utility/Line3d.h>

namespace aries
{

Line3d<_float_t> CommonPerpendicular(const Line3d<_float_t>& L1, const Line3d<_float_t>& L2)
{
	Vector3d<_float_t> L1P1 = L1.p1();
	Vector3d<_float_t> L1P2 = L1.p2();
	Vector3d<_float_t> L2P1 = L2.p1();
	Vector3d<_float_t> L2P2 = L2.p2();
	if (L1P1 == L2P1)
	{
        return Line3d<_float_t>(L1P1, L2P1);
	}
	if (L1P1 == L2P2)
	{
        return Line3d<_float_t>(L1P1, L2P2);
	}
	if (L1P2 == L2P1)
	{
        return Line3d<_float_t>(L1P2, L2P1);
	}
	if (L1P2 == L2P2)
	{
        return Line3d<_float_t>(L1P2, L2P2);
	}

	Vector3d<_float_t> L1Center    = L1.center();
	Vector3d<_float_t> L1Direction = L1.direction();
	Vector3d<_float_t> L2Center    = L2.center();
	Vector3d<_float_t> L2Direction = L2.direction();

	_float_t Sx  = L1Center.x();
	_float_t Sy  = L1Center.y();
	_float_t Sz  = L1Center.z();
	_float_t Mx  = L2Center.x();
	_float_t My  = L2Center.y();
	_float_t Mz  = L2Center.z();
	_float_t Szx = L1Direction.x();
	_float_t Szy = L1Direction.y();
	_float_t Szz = L1Direction.z();
	_float_t Mzx = L2Direction.x();
	_float_t Mzy = L2Direction.y();
	_float_t Mzz = L2Direction.z();
	Vector3d<_float_t> SzCrossMz = L1Direction.cross(L2Direction);
	_float_t PQzx = SzCrossMz.x();
	_float_t PQzy = SzCrossMz.y();
	_float_t PQzz = SzCrossMz.z();
	_float_t C1   = -PQzy * Szx + PQzx * Szy;
	_float_t C2   =  PQzy * Mzx - PQzx * Mzy;
	_float_t C3   = PQzx * (My - Sy) - PQzy * (Mx - Sx);
	_float_t C4   = -PQzy * Szz + PQzz * Szy;
	_float_t C5   = PQzy * Mzz - PQzz * Mzy;
	_float_t C6   = PQzz * (My - Sy) - PQzy * (Mz - Sz);
	_float_t A1   = C3 * C5 - C2 * C6;
	_float_t A2   = C1 * C5 - C2 * C4;
	Vector3d<_float_t> P, Q;
	if (A2 != 0.0)
	{
		_float_t A = A1 / A2;
		P.set(Sx + A * Szx, Sy + A * Szy, Sz + A * Szz);
		if (C2 != 0.0)
		{
			_float_t B = (C3 - C1 * A) / C2;
			Q.set(Mx + B * Mzx, My + B * Mzy, Mz + B * Mzz);
			return Line3d<_float_t>(P, Q);
		}
		else if (C5 != 0.0)
		{
			_float_t B = (C6 - C4 * A) / C5;
			Q.set(Mx + B * Mzx, My + B * Mzy, Mz + B * Mzz);
			return Line3d<_float_t>(P, Q);
		}
		else
		{
			// Line "PQ" parallel to "L2Direction"? It should be impossible.
			std::cerr
				<< "Error!! Code: Line3d<_float_t> aries::CommonPerpendicular(const Line3d<_float_t>&, const Line3d<_float_t>&)" << std::endl
				<< "        Note: Condition 1"                                                                                   << std::endl;
			exit(-1);
		}
	}
	else
	{
		// Two parallel lines?
		std::cerr << "L1 = " << L1;
		std::cerr << "L2 = " << L2;
		std::cerr << "SzCrossMz = " << SzCrossMz;
		std::cerr << "C1 = " << C1 << std::endl;
		std::cerr << "C5 = " << C5 << std::endl;
		std::cerr << "C2 = " << C2 << std::endl;
		std::cerr << "C4 = " << C4 << std::endl;
		std::cerr
			<< "Error!! Code: Line3d<_float_t> aries::CommonPerpendicular(const Line3d<_float_t>&, const Line3d<_float_t>&)" << std::endl
			<< "        Note: Condition 2"                                                                                   << std::endl;
		exit(-1);
	}
}

}   // namespace aries
