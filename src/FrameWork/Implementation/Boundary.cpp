#include <Framework/Interfaces/Boundary.h>
#include <cmath>

namespace vedo
{

Boundary::Boundary()
{
	sName = "Boundary";
	vLowerPoint = njr::ZERO;
	vUpperPoint = njr::ZERO;
	for(unsigned int i=0; i<3; i++)
	{
		bSwitch[i] = false;
	};
	Correct();
};

Boundary::Boundary
	(std::string sN,
	 const bool* bS,
	 const njr::Vector3d vLP,
	 const njr::Vector3d vUP): sName(sN)
{
	for(unsigned int i=0; i<3; i++)
	{
		bSwitch[i] = *(bS+i);
	};
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
};

Boundary::Boundary
	(const bool* bS,
	 const njr::Vector3d vLP,
	 const njr::Vector3d vUP )
{
	sName = "Boundary";
	for(unsigned int i=0; i<3; i++)
	{
		bSwitch[i] = *(bS+i);
	};
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
};

Boundary::Boundary
	(std::string sN,
	 const njr::Vector3d vLP,
	 const njr::Vector3d vUP): sName(sN)
{
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
};

Boundary::Boundary(const njr::Vector3d vLP, const njr::Vector3d vUP)
{
	sName = "Boundary";
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
};

void Boundary::operator = (const Boundary& b)
{
	sName       = b.sName;
	bActive     = b.bActive;
	for(unsigned int i=0; i<3; i++)
	{
		bSwitch[i] = b.bSwitch[i];
	};
	vLowerPoint = b.vLowerPoint;
	vUpperPoint = b.vUpperPoint;
	Correct();
};

Boundary::Boundary(const Boundary& b)
{
	*this = b;
};

Boundary::~Boundary()
{
};

void Boundary::Correct()
{
	double x_min = std::min(vLowerPoint.x(), vUpperPoint.x());
	double x_max = std::max(vLowerPoint.x(), vUpperPoint.x());
	double y_min = std::min(vLowerPoint.y(), vUpperPoint.y());
	double y_max = std::max(vLowerPoint.y(), vUpperPoint.y());
	double z_min = std::min(vLowerPoint.z(), vUpperPoint.z());
	double z_max = std::max(vLowerPoint.z(), vUpperPoint.z());

	vLowerPoint.Set(x_min, y_min, z_min);
	vUpperPoint.Set(x_max, y_max, z_max);
	vCenter = 0.5 * (vLowerPoint + vUpperPoint);
	vRange.Set(x_max - x_min, y_max - y_min, z_max - z_min);
	dVolume = vRange.x() * vRange.y() * vRange.z();

	if(x_min == x_max)
	{
		bSwitch[0] = false;
	}
	else
	{
		bSwitch[0] = true;
	};

	if(y_min == y_max)
	{
		bSwitch[1] = false;
	}
	else
	{
		bSwitch[1] = true;
	};

	if(z_min == z_max)
	{
		bSwitch[2] = false;
	}
	else
	{
		bSwitch[2] = true;
	};

	if( bSwitch[0] || bSwitch[1] || bSwitch[2] )
	{
		bActive = true;
	}
	else
	{
		bActive = false;
	};
};

void Boundary::SetLowerPoint(const njr::Vector3d* point)
{
	vLowerPoint = *point;
	Correct();
};

void Boundary::SetUpperPoint(const njr::Vector3d* point)
{
	vUpperPoint = *point;
	Correct();
};

void Boundary::SetCenter(const njr::Vector3d* cNewCenter)
{
	njr::Vector3d vShift = (*cNewCenter) - vCenter;
	vLowerPoint += vShift;
	vUpperPoint += vShift;
};

void Boundary::SetRange(const njr::Vector3d* vNewRange)
{
	double dX = fabs(vNewRange->x());
	double dY = fabs(vNewRange->y());
	double dZ = fabs(vNewRange->z());
	vRange.Set(dX, dY, dZ);
	vLowerPoint = vCenter - 0.5 * vRange;
	vUpperPoint = vCenter + 0.5 * vRange;
	Correct();
};

bool Boundary::InBoundary(const njr::Vector3d v) const
{
	if(Active())
	{
		if(bSwitch[0])
		{
			if (   ( (v.x()) < (vLowerPoint.x() ) )
				|| ( (v.x()) > (vUpperPoint.x() ) ))
			{
				return false;
			};
		};

		if(bSwitch[1])
		{
			if (   ( (v.y()) < (vLowerPoint.y() ) )
				|| ( (v.y()) > (vUpperPoint.y() ) ))
			{
				return false;
			};
		};

		if(bSwitch[2])
		{
			if (   ( (v.z()) < (vLowerPoint.z() ) )
				|| ( (v.z()) > (vUpperPoint.z() ) ))
			{
				return false;
			};
		};
	};

	return true;
};

bool Boundary::InBoundary(const njr::Vector3d v, const double r) const
{
	if(Active())
	{
		if(bSwitch[0])
		{
			if (   ( (v.x()) < (vLowerPoint.x()-r ) )
				|| ( (v.x()) > (vUpperPoint.x()+r ) ))
			{
				return false;
			};
		};

		if(bSwitch[1])
		{
			if (   ( (v.y()) < (vLowerPoint.y()-r ) )
				|| ( (v.y()) > (vUpperPoint.y()+r ) ))
			{
				return false;
			};
		};

		if(bSwitch[2])
		{
			if (   ( (v.z()) < (vLowerPoint.z()-r ) )
				|| ( (v.z()) > (vUpperPoint.z()+r ) ))
			{
				return false;
			};
		};
	};

	return true;
};

void Boundary::EnforceBoundaryConditions(njr::Vector3d* vPosition) const
{
	double dPx = vPosition->x();
	double dPy = vPosition->y();
	double dPz = vPosition->z();

	if(bSwitch[0])
	{
		if(dPx >= vUpperPoint.x())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPx -= vRange.x();
			if (dPx >= vUpperPoint.x())
			{
				dPx
					= vLowerPoint.x() + fmod(dPx - vLowerPoint.x(), vRange.x());
			}
		}

		if(dPx < vLowerPoint.x())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPx += vRange.x();
			if(dPx < vLowerPoint.x())
			{
				dPx
					= vLowerPoint.x()
					+ fmod(dPx - vLowerPoint.x(), vRange.x())
					+ vRange.x();
			}
		}
	}

	if(bSwitch[1])
	{
		if(dPy >= vUpperPoint.y())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPy -= vRange.y();
			if (dPy >= vUpperPoint.y())
			{
				dPy
					= vLowerPoint.y() + fmod(dPy - vLowerPoint.y(), vRange.y());
			}
		}

		if(dPy < vLowerPoint.y())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPy += vRange.y();
			if(dPy < vLowerPoint.y())
			{
				dPy
					= vLowerPoint.y()
					+ fmod(dPy - vLowerPoint.y(), vRange.y())
					+ vRange.y();
			}
		}
	}

	if(bSwitch[2])
	{
		if(dPz >= vUpperPoint.z())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPz -= vRange.z();
			if (dPz >= vUpperPoint.z())
			{
				dPz
					= vLowerPoint.z() + fmod(dPz - vLowerPoint.z(), vRange.z());
			}
		}

		if(dPz < vLowerPoint.z())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPz += vRange.z();
			if(dPz < vLowerPoint.z())
			{
				dPz
					= vLowerPoint.z()
					+ fmod(dPz - vLowerPoint.z(), vRange.z())
					+ vRange.z();
			}
		}
	}

	vPosition->Set(dPx, dPy, dPz);
};

void Boundary::DifferenceBoundaryConditions(njr::Vector3d* vDisplacement) const
{
	double dPx = vDisplacement->x();
	double dPy = vDisplacement->y();
	double dPz = vDisplacement->z();

	if(bSwitch[0])
	{
		if(dPx >= 0.5 * vRange.x())
		{
			dPx -= vRange.x();
		}

		if(dPx < -0.5 * vRange.x())
		{
			dPx += vRange.x();
		}
	};

	if(bSwitch[1])
	{
		if(dPy >= 0.5 * vRange.y())
		{
			dPy -= vRange.y();
		}

		if(dPy < -0.5 * vRange.y())
		{
			dPy += vRange.y();
		}
	};

	if(bSwitch[2])
	{
		if(dPz >= 0.5 * vRange.z())
		{
			dPz -= vRange.z();
		}

		if(dPz < -0.5 * vRange.z())
		{
			dPz += vRange.z();
		}
	};

	vDisplacement->Set(dPx, dPy, dPz);
};

void Boundary::print() const
{
	std::cout << "Type: " << sName << std::endl;

	if(!Active())
	{
		std::cout << "Boundary inactive" << std::endl;
	}
	else
	{
		if(bSwitch[0])
		{
			std::cout
				<< "From X="
				<< vLowerPoint.x()
				<< " to X= "
				<< vUpperPoint.x()
				<< std::endl;
		};

		if(bSwitch[1])
		{
			std::cout
				<< "From Y="
				<< vLowerPoint.y()
				<< " to Y= "
				<< vUpperPoint.y()
				<< std::endl;
		};

		if(bSwitch[2])
		{
			std::cout
				<< "From Z="
				<< vLowerPoint.z()
				<< " to Z= "
				<< vUpperPoint.z()
				<< std::endl;
		};
	};
};

std::ostream& operator << (std::ostream& os, Boundary& b)
{
	if(!(b.Active()))
	{
		os << "Boundary inactive!!" << std::endl;
	}
	else
	{
		os << "Type: " << b.GetName() << std::endl;

		if(b.GetSwitch(0))
		{
			os
				<< "From X="
				<< b.GetLowerPoint().x()
				<< " to X= "
				<< b.GetUpperPoint().x()
				<< std::endl;
		};

		if(b.GetSwitch(1))
		{
			os
				<< "From Y="
				<< b.GetLowerPoint().y()
				<< " to Y= "
				<< b.GetUpperPoint().y()
				<< std::endl;
		};

		if(b.GetSwitch(2))
		{
			os
				<< "From Z="
				<< b.GetLowerPoint().z()
				<< " to Z= "
				<< b.GetUpperPoint().z()
				<< std::endl;
		};
	};
	return os;
};

};   // namespace vedo
