#ifndef _BOUNDARY2D_H
#define _BOUNDARY2D_H

#include <NJR/Interfaces/vector2d.h>
#include <string>

class Boundary2d
{

public:

	Boundary2d();

	Boundary2d(std::string, const bool*, const NJRvector2d, const NJRvector2d);

	Boundary2d(const bool*, const NJRvector2d, const NJRvector2d);

	Boundary2d(std::string, const NJRvector2d, const NJRvector2d);

	Boundary2d(const NJRvector2d, const NJRvector2d);

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

	void SetCenter(const NJRvector2d*);

	inline void SetBoxCenter(const NJRvector2d* vNewCenter)
	{
		SetCenter(vNewCenter);
	};

	void SetRange(const NJRvector2d*);

	inline void SetLength(const NJRvector2d* vNewRange)
	{
		SetRange(vNewRange);
	};

	inline const NJRvector2d& GetLength() const
	{
		return vRange;
	}

	inline const NJRvector2d& GetRange() const
	{
		return vRange;
	}

	inline const NJRvector2d& GetCenter() const
	{
		return vCenter;
	};

	inline const double GetArea() const
	{
		return dArea;
	};

	void SetLowerPoint(const NJRvector2d*);

	inline void SetLowerEdge(const NJRvector2d* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	};

	void SetUpperPoint(const NJRvector2d*);

	inline void SetUpperEdge(const NJRvector2d* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	};

	inline const NJRvector2d& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const NJRvector2d& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const NJRvector2d*) const;

	bool InBoundary(const NJRvector2d*, const double) const;

	void EnforceBoundaryConditions(NJRvector2d*) const;

	void DifferenceBoundaryConditions(NJRvector2d*) const;

	void print() const;

	void Correct();

private:

	std::string sName;
	bool bActive;
	bool bSwitch[2];           // Open or close
	NJRvector2d	vLowerPoint;   // Lower boundary
	NJRvector2d	vUpperPoint;   // Upper boundary
	NJRvector2d	vCenter;       // Center
	double dArea;              // Area (inside the boundary)
	NJRvector2d	vRange;        // Range (length in x, y directions)
};

#endif // _BOUNDARY2D_H
