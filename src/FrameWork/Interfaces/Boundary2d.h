#ifndef _BOUNDARY2D_H
#define _BOUNDARY2D_H

#include <NJR/Interfaces/Vector2d.h>
#include <string>

namespace VEDO
{

class Boundary2d
{

public:

	Boundary2d();

	Boundary2d(std::string, const bool*, const NJR::Vector2d, const NJR::Vector2d);

	Boundary2d(const bool*, const NJR::Vector2d, const NJR::Vector2d);

	Boundary2d(std::string, const NJR::Vector2d, const NJR::Vector2d);

	Boundary2d(const NJR::Vector2d, const NJR::Vector2d);

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

	void SetCenter(const NJR::Vector2d*);

	inline void SetBoxCenter(const NJR::Vector2d* vNewCenter)
	{
		SetCenter(vNewCenter);
	};

	void SetRange(const NJR::Vector2d*);

	inline void SetLength(const NJR::Vector2d* vNewRange)
	{
		SetRange(vNewRange);
	};

	inline const NJR::Vector2d& GetLength() const
	{
		return vRange;
	}

	inline const NJR::Vector2d& GetRange() const
	{
		return vRange;
	}

	inline const NJR::Vector2d& GetCenter() const
	{
		return vCenter;
	};

	inline const double GetArea() const
	{
		return dArea;
	};

	void SetLowerPoint(const NJR::Vector2d*);

	inline void SetLowerEdge(const NJR::Vector2d* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	};

	void SetUpperPoint(const NJR::Vector2d*);

	inline void SetUpperEdge(const NJR::Vector2d* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	};

	inline const NJR::Vector2d& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const NJR::Vector2d& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const NJR::Vector2d*) const;

	bool InBoundary(const NJR::Vector2d*, const double) const;

	void EnforceBoundaryConditions(NJR::Vector2d*) const;

	void DifferenceBoundaryConditions(NJR::Vector2d*) const;

	void print() const;

	void Correct();

private:

	std::string sName;
	bool bActive;
	bool bSwitch[2];           // Open or close
	NJR::Vector2d	vLowerPoint;   // Lower boundary
	NJR::Vector2d	vUpperPoint;   // Upper boundary
	NJR::Vector2d	vCenter;       // Center
	double dArea;              // Area (inside the boundary)
	NJR::Vector2d	vRange;        // Range (length in x, y directions)
};

};   // namespace VEDO



std::ostream& operator << (std::ostream&, VEDO::Boundary2d&);

#endif // _BOUNDARY2D_H
