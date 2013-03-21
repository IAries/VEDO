#ifndef _BOUNDARY_H
#define _BOUNDARY_H

#include <NJR/Interfaces/vector3d.h>
#include <string>

namespace VEDO
{

class Boundary
{

public:

	Boundary();

	Boundary(std::string, const bool*, const NJR::NJRvector3d, const NJR::NJRvector3d);

	Boundary(const bool*, const NJR::NJRvector3d, const NJR::NJRvector3d);

	Boundary(std::string, const NJR::NJRvector3d, const NJR::NJRvector3d);

	Boundary(const NJR::NJRvector3d, const NJR::NJRvector3d);

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

	void SetCenter(const NJR::NJRvector3d*);

	inline void SetBoxCenter(const NJR::NJRvector3d* vNewCenter)
	{
		SetCenter(vNewCenter);
	};

	void SetRange(const NJR::NJRvector3d*);

	inline void SetLength(const NJR::NJRvector3d* vNewRange)
	{
		SetRange(vNewRange);
	};

	inline const NJR::NJRvector3d& GetLength() const
	{
		return vRange;
	}

	inline const NJR::NJRvector3d& GetRange() const
	{
		return vRange;
	}

	inline const NJR::NJRvector3d& GetCenter() const
	{
		return vCenter;
	};

	inline const double GetVolume() const
	{
		return dVolume;
	};

	void SetLowerPoint(const NJR::NJRvector3d*);

	inline void SetLowerEdge(const NJR::NJRvector3d* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	};

	void SetUpperPoint(const NJR::NJRvector3d*);

	inline void SetUpperEdge(const NJR::NJRvector3d* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	};

	inline const NJR::NJRvector3d& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const NJR::NJRvector3d& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const NJR::NJRvector3d) const;

	bool InBoundary(const NJR::NJRvector3d, const double) const;

	void EnforceBoundaryConditions(NJR::NJRvector3d*) const;

	void DifferenceBoundaryConditions(NJR::NJRvector3d*) const;

	void print() const;

	void Correct();

private:

	std::string sName;
	bool bActive;
	bool bSwitch[3];           // Open or close
	NJR::NJRvector3d	vLowerPoint;   // Lower boundary
	NJR::NJRvector3d	vUpperPoint;   // Upper boundary
	NJR::NJRvector3d	vCenter;       // Center
	double dVolume;            // Volume (space inside the boundary)
	NJR::NJRvector3d	vRange;        // Range (length in x, y, z directions)
};

};   // namespace VEDO

#endif // _BOUNDARY_H
