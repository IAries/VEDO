#include <vedo/framework/Boundary.h>
#include <cmath>

namespace vedo
{

Boundary::Boundary()
{
	sName       = "Boundary";
	vLowerPoint.set(0.0, 0.0, 0.0);
	vUpperPoint.set(0.0, 0.0, 0.0);
	for (_uint_t i=0; i<3; i++)
	{
		bSwitch[i] = false;
	}
	Correct();
}

Boundary::Boundary(std::string sN, const bool* bS, const aries::Vector3df vLP, const aries::Vector3df vUP): sName(sN)
{
	for (_uint_t i=0; i<3; i++)
	{
		bSwitch[i] = *(bS+i);
	}
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
}

Boundary::Boundary(const bool* bS, const aries::Vector3df vLP, const aries::Vector3df vUP)
{
	sName = "Boundary";
	for (_uint_t i=0; i<3; i++)
	{
		bSwitch[i] = *(bS+i);
	}
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
}

Boundary::Boundary(std::string sN, const aries::Vector3df vLP, const aries::Vector3df vUP): sName(sN)
{
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
}

Boundary::Boundary(const aries::Vector3df vLP, const aries::Vector3df vUP)
{
	sName       = "Boundary";
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
}

void Boundary::operator = (const Boundary& b)
{
	sName   = b.sName;
	bActive = b.bActive;
	for (_uint_t i=0; i<3; i++)
	{
		bSwitch[i] = b.bSwitch[i];
	}
	vLowerPoint = b.vLowerPoint;
	vUpperPoint = b.vUpperPoint;
	Correct();
}

Boundary::Boundary(const Boundary& b)
{
	*this = b;
}

Boundary::~Boundary()
{
}

void Boundary::Correct()
{
	_float_t x_min = std::min(vLowerPoint.x(), vUpperPoint.x());
	_float_t x_max = std::max(vLowerPoint.x(), vUpperPoint.x());
	_float_t y_min = std::min(vLowerPoint.y(), vUpperPoint.y());
	_float_t y_max = std::max(vLowerPoint.y(), vUpperPoint.y());
	_float_t z_min = std::min(vLowerPoint.z(), vUpperPoint.z());
	_float_t z_max = std::max(vLowerPoint.z(), vUpperPoint.z());

	vLowerPoint.set(x_min, y_min, z_min);
	vUpperPoint.set(x_max, y_max, z_max);
	vCenter = 0.5 * (vLowerPoint + vUpperPoint);
	vRange.set(x_max - x_min, y_max - y_min, z_max - z_min);
	dVolume = vRange.x() * vRange.y() * vRange.z();

	if (x_min == x_max)
	{
		bSwitch[0] = false;
	}
	else
	{
		bSwitch[0] = true;
	}

	if (y_min == y_max)
	{
		bSwitch[1] = false;
	}
	else
	{
		bSwitch[1] = true;
	}

	if (z_min == z_max)
	{
		bSwitch[2] = false;
	}
	else
	{
		bSwitch[2] = true;
	}

	if (bSwitch[0] || bSwitch[1] || bSwitch[2])
	{
		bActive = true;
	}
	else
	{
		bActive = false;
	}
}

void Boundary::SetLowerPoint(const aries::Vector3df* point)
{
	vLowerPoint = *point;
	Correct();
}

void Boundary::SetUpperPoint(const aries::Vector3df* point)
{
	vUpperPoint = *point;
	Correct();
}

void Boundary::SetCenter(const aries::Vector3df* cNewCenter)
{
	aries::Vector3df vShift = (*cNewCenter) - vCenter;
	vLowerPoint += vShift;
	vUpperPoint += vShift;
}

void Boundary::SetRange(const aries::Vector3df* vNewRange)
{
	_float_t dX = fabs(vNewRange->x());
	_float_t dY = fabs(vNewRange->y());
	_float_t dZ = fabs(vNewRange->z());
	vRange.set(dX, dY, dZ);
	vLowerPoint = vCenter - 0.5 * vRange;
	vUpperPoint = vCenter + 0.5 * vRange;
	Correct();
}

bool Boundary::InBoundary(const aries::Vector3df v) const
{
	if (Active())
	{
		if (bSwitch[0])
		{
			if ((v.x() < vLowerPoint.x()) || (v.x() > vUpperPoint.x()))
			{
				return false;
			}
		}

		if (bSwitch[1])
		{
			if ((v.y() < vLowerPoint.y()) || (v.y() > vUpperPoint.y()))
			{
				return false;
			}
		}

		if (bSwitch[2])
		{
			if ((v.z() < vLowerPoint.z()) || (v.z() > vUpperPoint.z()))
			{
				return false;
			}
		}
	}

	return true;
}

bool Boundary::InBoundary(const aries::Vector3df v, const _float_t r) const
{
	if (Active())
	{
		if (bSwitch[0])
		{
			if ((v.x() < (vLowerPoint.x()-r)) || (v.x() > (vUpperPoint.x()+r)))
			{
				return false;
			}
		}

		if (bSwitch[1])
		{
			if ((v.y() < (vLowerPoint.y()-r)) || (v.y() > (vUpperPoint.y()+r)))
			{
				return false;
			}
		}

		if (bSwitch[2])
		{
			if ((v.z() < (vLowerPoint.z()-r )) || (v.z() > (vUpperPoint.z()+r)))
			{
				return false;
			}
		}
	}

	return true;
}

void Boundary::EnforceBoundaryConditions(aries::Vector3df* vPosition) const
{
	_float_t dPx = vPosition->x();
	_float_t dPy = vPosition->y();
	_float_t dPz = vPosition->z();

	if (bSwitch[0])
	{
		if (dPx >= vUpperPoint.x())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPx -= vRange.x();
			if (dPx >= vUpperPoint.x())
			{
				dPx = vLowerPoint.x() + fmod(dPx - vLowerPoint.x(), vRange.x());
			}
		}

		if (dPx < vLowerPoint.x())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPx += vRange.x();
			if (dPx < vLowerPoint.x())
			{
				dPx	= vLowerPoint.x() + fmod(dPx - vLowerPoint.x(), vRange.x()) + vRange.x();
			}
		}
	}

	if (bSwitch[1])
	{
		if (dPy >= vUpperPoint.y())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPy -= vRange.y();
			if (dPy >= vUpperPoint.y())
			{
				dPy = vLowerPoint.y() + fmod(dPy - vLowerPoint.y(), vRange.y());
			}
		}

		if (dPy < vLowerPoint.y())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPy += vRange.y();
			if (dPy < vLowerPoint.y())
			{
				dPy = vLowerPoint.y() + fmod(dPy - vLowerPoint.y(), vRange.y()) + vRange.y();
			}
		}
	}

	if(bSwitch[2])
	{
		if (dPz >= vUpperPoint.z())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPz -= vRange.z();
			if (dPz >= vUpperPoint.z())
			{
				dPz = vLowerPoint.z() + fmod(dPz - vLowerPoint.z(), vRange.z());
			}
		}

		if (dPz < vLowerPoint.z())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPz += vRange.z();
			if (dPz < vLowerPoint.z())
			{
				dPz = vLowerPoint.z() + fmod(dPz - vLowerPoint.z(), vRange.z()) + vRange.z();
			}
		}
	}

	vPosition->set(dPx, dPy, dPz);
}

void Boundary::DifferenceBoundaryConditions(aries::Vector3df* vDisplacement) const
{
	_float_t dPx = vDisplacement->x();
	_float_t dPy = vDisplacement->y();
	_float_t dPz = vDisplacement->z();

	if (bSwitch[0])
	{
		if (dPx >= 0.5 * vRange.x())
		{
			dPx -= vRange.x();
		}

		if (dPx < -0.5 * vRange.x())
		{
			dPx += vRange.x();
		}
	}

	if (bSwitch[1])
	{
		if (dPy >= 0.5 * vRange.y())
		{
			dPy -= vRange.y();
		}

		if (dPy < -0.5 * vRange.y())
		{
			dPy += vRange.y();
		}
	}

	if (bSwitch[2])
	{
		if (dPz >= 0.5 * vRange.z())
		{
			dPz -= vRange.z();
		}

		if (dPz < -0.5 * vRange.z())
		{
			dPz += vRange.z();
		}
	}

	vDisplacement->set(dPx, dPy, dPz);
}

void Boundary::print() const
{
	std::cout << "Type: " << sName << std::endl;

	if (!Active())
	{
		std::cout << "Boundary inactive" << std::endl;
	}
	else
	{
		if (bSwitch[0])
		{
			std::cout << "From X=" << vLowerPoint.x() << " to X= " << vUpperPoint.x() << std::endl;
		}

		if (bSwitch[1])
		{
			std::cout << "From Y=" << vLowerPoint.y() << " to Y= " << vUpperPoint.y() << std::endl;
		}

		if (bSwitch[2])
		{
			std::cout << "From Z=" << vLowerPoint.z() << " to Z= " << vUpperPoint.z() << std::endl;
		}
	}
}

std::ostream& operator << (std::ostream& os, Boundary& b)
{
	if (!(b.Active()))
	{
		os << "Boundary inactive!!" << std::endl;
	}
	else
	{
		os << "Type: " << b.GetName() << std::endl;

		if (b.GetSwitch(0))
		{
			os << "From X=" << b.GetLowerPoint().x() << " to X= " << b.GetUpperPoint().x() << std::endl;
		}

		if (b.GetSwitch(1))
		{
			os << "From Y=" << b.GetLowerPoint().y() << " to Y= " << b.GetUpperPoint().y() << std::endl;
		}

		if (b.GetSwitch(2))
		{
			os << "From Z=" << b.GetLowerPoint().z() << " to Z= " << b.GetUpperPoint().z() << std::endl;
		}
	}
	return os;
}

}   // namespace vedo
