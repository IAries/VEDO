#ifndef _BOUNDARY2D_H
#define _BOUNDARY2D_H

#include <NJR/Interfaces/Vector2d.h>
#include <string>

namespace vedo
{

class Boundary2d
{

public:

	Boundary2d();

	Boundary2d(std::string, const bool*, const njr::Vector2d, const njr::Vector2d);

	Boundary2d(const bool*, const njr::Vector2d, const njr::Vector2d);

	Boundary2d(std::string, const njr::Vector2d, const njr::Vector2d);

	Boundary2d(const njr::Vector2d, const njr::Vector2d);

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

	void SetCenter(const njr::Vector2d*);

	inline void SetBoxCenter(const njr::Vector2d* vNewCenter)
	{
		SetCenter(vNewCenter);
	};

	void SetRange(const njr::Vector2d*);

	inline void SetLength(const njr::Vector2d* vNewRange)
	{
		SetRange(vNewRange);
	};

	inline const njr::Vector2d& GetLength() const
	{
		return vRange;
	}

	inline const njr::Vector2d& GetRange() const
	{
		return vRange;
	}

	inline const njr::Vector2d& GetCenter() const
	{
		return vCenter;
	};

	inline const double GetArea() const
	{
		return dArea;
	};

	void SetLowerPoint(const njr::Vector2d*);

	inline void SetLowerEdge(const njr::Vector2d* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	};

	void SetUpperPoint(const njr::Vector2d*);

	inline void SetUpperEdge(const njr::Vector2d* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	};

	inline const njr::Vector2d& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const njr::Vector2d& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const njr::Vector2d*) const;

	bool InBoundary(const njr::Vector2d*, const double) const;

	void EnforceBoundaryConditions(njr::Vector2d*) const;

	void DifferenceBoundaryConditions(njr::Vector2d*) const;

	void print() const;

	void Correct();

private:

	std::string sName;
	bool bActive;
	bool bSwitch[2];           // Open or close
	njr::Vector2d	vLowerPoint;   // Lower boundary
	njr::Vector2d	vUpperPoint;   // Upper boundary
	njr::Vector2d	vCenter;       // Center
	double dArea;              // Area (inside the boundary)
	njr::Vector2d	vRange;        // Range (length in x, y directions)
};

};   // namespace vedo



std::ostream& operator << (std::ostream&, vedo::Boundary2d&);

#endif // _BOUNDARY2D_H
