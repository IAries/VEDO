#include <Framework/Interfaces/Boundary2d.h>
#include <cmath>

namespace vedo
{

Boundary2d::Boundary2d()
{
	sName = "Boundary2d";
	vLowerPoint = njrdxf::ZERO;
	vUpperPoint = njrdxf::ZERO;
	bSwitch[0] = false;
	bSwitch[1] = false;
	Correct();
};

Boundary2d::Boundary2d
	(std::string sN,
	 const bool* bS,
	 const njr::Vector2d vLP,
	 const njr::Vector2d vUP): sName(sN)
{
	bSwitch[0]  = *bS;
	bSwitch[1]  = *(bS+1);
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
};

Boundary2d::Boundary2d
	(const bool* bS,
	 const njr::Vector2d vLP,
	 const njr::Vector2d vUP )
{
	sName = "Boundary2d";
	bSwitch[0] = *bS;
	bSwitch[1] = *(bS+1);
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
};

Boundary2d::Boundary2d
	(std::string sN,
	 const njr::Vector2d vLP,
	 const njr::Vector2d vUP): sName(sN)
{
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
};

Boundary2d::Boundary2d(const njr::Vector2d vLP, const njr::Vector2d vUP)
{
	sName = "Boundary2d";
	vLowerPoint = vLP;
	vUpperPoint = vUP;
	Correct();
};

void Boundary2d::operator = (const Boundary2d& b)
{
	sName       = b.sName;
	bActive     = b.bActive;
	bSwitch[0]  = b.bSwitch[0];
	bSwitch[1]  = b.bSwitch[1];
	vLowerPoint = b.vLowerPoint;
	vUpperPoint = b.vUpperPoint;
	Correct();
};

Boundary2d::Boundary2d(const Boundary2d& b)
{
	*this = b;
};

Boundary2d::~Boundary2d()
{
};

void Boundary2d::Correct()
{
	double x_min = std::min(vLowerPoint.x(), vUpperPoint.x());
	double x_max = std::max(vLowerPoint.x(), vUpperPoint.x());
	double y_min = std::min(vLowerPoint.y(), vUpperPoint.y());
	double y_max = std::max(vLowerPoint.y(), vUpperPoint.y());

	vLowerPoint.Set(x_min, y_min);
	vUpperPoint.Set(x_max, y_max);
	vCenter = 0.5 * (vLowerPoint + vUpperPoint);
	vRange.Set(x_max - x_min, y_max - y_min);
	dArea = vRange.x() * vRange.y();

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

	if( bSwitch[0] || bSwitch[1] )
	{
		bActive = true;
	}
	else
	{
		bActive = false;
	};
};

void Boundary2d::SetLowerPoint(const njr::Vector2d* point)
{
	vLowerPoint = *point;
	Correct();
};

void Boundary2d::SetUpperPoint(const njr::Vector2d* point)
{
	vUpperPoint = *point;
	Correct();
};

void Boundary2d::SetCenter(const njr::Vector2d* cNewCenter)
{
	njr::Vector2d vShift = (*cNewCenter) - vCenter;
	vLowerPoint += vShift;
	vUpperPoint += vShift;
};

void Boundary2d::SetRange(const njr::Vector2d* vNewRange)
{
	double dX = fabs(vNewRange->x());
	double dY = fabs(vNewRange->y());
	vRange.Set(dX, dY);
	vLowerPoint = vCenter - 0.5 * vRange;
	vUpperPoint = vCenter + 0.5 * vRange;
	Correct();
};

bool Boundary2d::InBoundary(const njr::Vector2d* p) const
{
	if(Active())
	{
		if(bSwitch[0])
		{
			if (   ( (p->x()) < (vLowerPoint.x() ) )
				|| ( (p->x()) > (vUpperPoint.x() ) ))
			{
				return false;
			};
		};

		if(bSwitch[1])
		{
			if (   ( (p->y()) < (vLowerPoint.y() ) )
				|| ( (p->y()) > (vUpperPoint.y() ) ))
			{
				return false;
			};
		};
	};

	return true;
};

bool Boundary2d::InBoundary(const njr::Vector2d* p, const double r) const
{
	if(Active())
	{
		if(bSwitch[0])
		{
			if (   ( (p->x()) < (vLowerPoint.x()-r ) )
				|| ( (p->x()) > (vUpperPoint.x()+r ) ))
			{
				return false;
			};
		};

		if(bSwitch[1])
		{
			if (   ( (p->y()) < (vLowerPoint.y()-r ) )
				|| ( (p->y()) > (vUpperPoint.y()+r ) ))
			{
				return false;
			};
		};
	};

	return true;
};

void Boundary2d::EnforceBoundaryConditions(njr::Vector2d* vPosition) const
{
	double dPx = vPosition->x();
	double dPy = vPosition->y();

	if(bSwitch[0])
	{
		if(dPx >= vUpperPoint.x())
		{
			// Do this if fmod is more expensive than two "if"s.
			dPx -= vRange.x();
			if (dPx >= vUpperPoint.x())
			{
				dPx
					= vLowerPoint.x()
					+ fmod(dPx - vLowerPoint.x(), vRange.x());
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

	vPosition->Set(dPx, dPy);
};

void Boundary2d::DifferenceBoundaryConditions(njr::Vector2d* vDisplacement) const
{
	double dPx = vDisplacement->x();
	double dPy = vDisplacement->y();

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

	vDisplacement->Set(dPx, dPy);
};

void Boundary2d::print() const
{
	std::cout << "Type: " << sName << std::endl;

	if(!Active())
	{
		std::cout << "Boundary2d inactive" << std::endl;
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
	};
};

};   // namespace vedo



std::ostream& operator << (std::ostream& os, vedo::Boundary2d& b)
{
	if(!(b.Active()))
	{
		os << "Boundary2d inactive!!" << std::endl;
	}
	else
	{
		os << "Type: " << b.GetName() << std::endl;

		if(b.GetSwitch(0))
		{
			os
				<< "From x="
				<< b.GetLowerPoint().x()
				<< " to x= "
				<< b.GetUpperPoint().x()
				<< std::endl;
		};

		if(b.GetSwitch(1))
		{
			os
				<< "From y="
				<< b.GetLowerPoint().y()
				<< " to y= "
				<< b.GetUpperPoint().y()
				<< std::endl;
		};
	};
	return os;
};
