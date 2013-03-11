#ifndef _BOUNDARY_H
#define _BOUNDARY_H

#include <NJR/Interfaces/vector3d.h>
#include <string>

class Boundary
{

public:

	Boundary();

	Boundary(std::string, const bool*, const NJRvector3d, const NJRvector3d);

	Boundary(const bool*, const NJRvector3d, const NJRvector3d);

	Boundary(std::string, const NJRvector3d, const NJRvector3d);

	Boundary(const NJRvector3d, const NJRvector3d);

	Boundary(const Boundary&);

	virtual ~Boundary();

	void operator = (const Boundary&);

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

	void SetCenter(const NJRvector3d*);

	inline void SetBoxCenter(const NJRvector3d* vNewCenter)
	{
		SetCenter(vNewCenter);
	};

	void SetRange(const NJRvector3d*);

	inline void SetLength(const NJRvector3d* vNewRange)
	{
		SetRange(vNewRange);
	};

	inline const NJRvector3d& GetLength() const
	{
		return vRange;
	}

	inline const NJRvector3d& GetRange() const
	{
		return vRange;
	}

	inline const NJRvector3d& GetCenter() const
	{
		return vCenter;
	};

	inline const double GetVolume() const
	{
		return dVolume;
	};

	void SetLowerPoint(const NJRvector3d*);

	inline void SetLowerEdge(const NJRvector3d* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	};

	void SetUpperPoint(const NJRvector3d*);

	inline void SetUpperEdge(const NJRvector3d* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	};

	inline const NJRvector3d& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const NJRvector3d& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const NJRvector3d) const;

	bool InBoundary(const NJRvector3d, const double) const;

	void EnforceBoundaryConditions(NJRvector3d*) const;

	void DifferenceBoundaryConditions(NJRvector3d*) const;

	void print() const;

	void Correct();

private:

	std::string sName;
	bool bActive;
	bool bSwitch[3];           // Open or close
	NJRvector3d	vLowerPoint;   // Lower boundary
	NJRvector3d	vUpperPoint;   // Upper boundary
	NJRvector3d	vCenter;       // Center
	double dVolume;            // Volume (space inside the boundary)
	NJRvector3d	vRange;        // Range (length in x, y, z directions)
};

#endif // _BOUNDARY_H
