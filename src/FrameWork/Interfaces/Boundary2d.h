#ifndef _BOUNDARY2D_H
#define _BOUNDARY2D_H

#include <NJR/Interfaces/vector2d.h>
#include <string>

namespace VEDO
{

class Boundary2d
{

public:

	Boundary2d();

	Boundary2d(std::string, const bool*, const NJR::NJRvector2d, const NJR::NJRvector2d);

	Boundary2d(const bool*, const NJR::NJRvector2d, const NJR::NJRvector2d);

	Boundary2d(std::string, const NJR::NJRvector2d, const NJR::NJRvector2d);

	Boundary2d(const NJR::NJRvector2d, const NJR::NJRvector2d);

	Boundary2d(const Boundary2d&);

	virtual ~Boundary2d();

	void operator = (const Boundary2d&);

	inline void SetName(std::string sN)
	{
		sName = sN;
	};

	inline const std::string GetName() const
	{
		return sName;
	};

	inline const bool Active() const
	{
		return bActive;
	};

	inline const bool& GetSwitch() const
	{
		return bSwitch[0];
	};

	inline const bool& GetSwitch(unsigned int i) const
	{
		return bSwitch[i];
	};

	void SetCenter(const NJR::NJRvector2d*);

	inline void SetBoxCenter(const NJR::NJRvector2d* vNewCenter)
	{
		SetCenter(vNewCenter);
	};

	void SetRange(const NJR::NJRvector2d*);

	inline void SetLength(const NJR::NJRvector2d* vNewRange)
	{
		SetRange(vNewRange);
	};

	inline const NJR::NJRvector2d& GetLength() const
	{
		return vRange;
	}

	inline const NJR::NJRvector2d& GetRange() const
	{
		return vRange;
	}

	inline const NJR::NJRvector2d& GetCenter() const
	{
		return vCenter;
	};

	inline const double GetArea() const
	{
		return dArea;
	};

	void SetLowerPoint(const NJR::NJRvector2d*);

	inline void SetLowerEdge(const NJR::NJRvector2d* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	};

	void SetUpperPoint(const NJR::NJRvector2d*);

	inline void SetUpperEdge(const NJR::NJRvector2d* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	};

	inline const NJR::NJRvector2d& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const NJR::NJRvector2d& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const NJR::NJRvector2d*) const;

	bool InBoundary(const NJR::NJRvector2d*, const double) const;

	void EnforceBoundaryConditions(NJR::NJRvector2d*) const;

	void DifferenceBoundaryConditions(NJR::NJRvector2d*) const;

	void print() const;

	void Correct();

private:

	std::string sName;
	bool bActive;
	bool bSwitch[2];           // Open or close
	NJR::NJRvector2d	vLowerPoint;   // Lower boundary
	NJR::NJRvector2d	vUpperPoint;   // Upper boundary
	NJR::NJRvector2d	vCenter;       // Center
	double dArea;              // Area (inside the boundary)
	NJR::NJRvector2d	vRange;        // Range (length in x, y directions)
};

};   // namespace VEDO



std::ostream& operator << (std::ostream&, VEDO::Boundary2d&);

#endif // _BOUNDARY2D_H
