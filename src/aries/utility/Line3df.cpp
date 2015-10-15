#include <aries/utility/Line3df.h>

namespace aries
{

Line3df::Line3df()
{
}

Line3df::Line3df(const Vector3df& p1, const Vector3df& p2)
{
	_p1 = p1;
	_p2 = p2;
}

Line3df::Line3df(const Vector3df& center, const Vector3df& direction, const _float_t& length)
{
	_p1 = center - 0.5 * length * direction;
	_p2 = center + 0.5 * length * direction;
}

Line3df::Line3df(const Line3df& L)
{
	*this = L;
}

const Line3df& Line3df::operator = (const Line3df& L)
{
	_p1 = L.p1();
	_p2 = L.p2();
	return *this;
}

bool Line3df::OnLine(const Vector3df& point) const
{
	if (this->InCoverSpace(point))
	{
		Vector3df d1 = (point - _p1).direction();
		Vector3df d2 = this->direction();
		//if (d1 == d2)
		if ((d1-d2).length() <= 1.0e-12)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

Line3df CommonPerpendicular(const Line3df& L1, const Line3df& L2)
{
	Vector3df L1P1 = L1.p1();
	Vector3df L1P2 = L1.p2();
	Vector3df L2P1 = L2.p1();
	Vector3df L2P2 = L2.p2();
	if (L1P1 == L2P1)
	{
        return Line3df(L1P1, L2P1);
	}
	if (L1P1 == L2P2)
	{
        return Line3df(L1P1, L2P2);
	}
	if (L1P2 == L2P1)
	{
        return Line3df(L1P2, L2P1);
	}
	if (L1P2 == L2P2)
	{
        return Line3df(L1P2, L2P2);
	}

	Vector3df L1Center    = L1.center();
	Vector3df L1Direction = L1.direction();
	Vector3df L2Center    = L2.center();
	Vector3df L2Direction = L2.direction();

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
	Vector3df SzCrossMz = L1Direction.cross(L2Direction);
	_float_t Rx  = SzCrossMz.x();
	_float_t Ry  = SzCrossMz.y();
	_float_t Rz  = SzCrossMz.z();
	_float_t C1  = -Ry * Szx + Rx * Szy;
	_float_t C4  = -Rz * Szx + Rx * Szz;
	_float_t C7  = -Ry * Szz + Rz * Szy;
	_float_t C2  =  Ry * Mzx - Rx * Mzy;
	_float_t C5  =  Rz * Mzx - Rx * Mzz;
	_float_t C8  =  Ry * Mzz - Rz * Mzy;
	_float_t C3  =  Rx * (My - Sy) - Ry * (Mx - Sx);
	_float_t C6  =  Rx * (Mz - Sz) - Rz * (Mx - Sx);
	_float_t C9  =  Rz * (My - Sy) - Ry * (Mz - Sz);
	_float_t D1  =  C2 * C4 - C1 * C5;
	_float_t D2  =  C2 * C7 - C1 * C8;
	_float_t D3  =  C5 * C7 - C4 * C8;

	_float_t b;
	if (D1 != 0.0)
	{
		b = (C3 * C4 - C1 * C6) / D1;
	}
	else if (D2 != 0.0)
	{
		b = (C3 * C7 - C1 * C9) / D2;
	}
	else if (D3 != 0.0)
	{
		b = (C6 * C7 - C4 * C9) / D3;
	}
	else
	{
		std::cerr
			<< "Error!! Code: Line3df aries::CommonPerpendicular(const Line3df&, const Line3df&)" << std::endl
			<< "        Note: Cannot solve coefficient \"b\"."                                                               << std::endl;
		exit(-1);
	}

	_float_t a;
	if (C1 != 0)
	{
		a = (C3 - C2 * b) / C1;
	}
	else if (C4 != 0)
	{
		a = (C6 - C5 * b) / C4;
	}
	else if (C7 != 0)
	{
		a = (C9 - C8 * b) / C7;
	}
	else
	{
		std::cerr
			<< "Error!! Code: Line3df aries::CommonPerpendicular(const Line3df&, const Line3df&)" << std::endl
			<< "        Note: Cannot solve coefficient \"a\"."                                                               << std::endl;
		exit(-1);
	}

	Vector3df P(Sx + a * Szx, Sy + a * Szy, Sz + a * Szz);
	Vector3df Q(Mx + b * Mzx, My + b * Mzy, Mz + b * Mzz);
	return Line3df(P, Q);
}

}   // namespace aries
