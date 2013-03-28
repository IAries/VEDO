#ifndef _BOUNDARY_H
#define _BOUNDARY_H

#include <NJR/Interfaces/Vector3d.h>
#include <string>

namespace VEDO
{

class Boundary
{

public:

	Boundary();

	Boundary(std::string, const bool*, const NJR::Vector3d, const NJR::Vector3d);

	Boundary(const bool*, const NJR::Vector3d, const NJR::Vector3d);

	Boundary(std::string, const NJR::Vector3d, const NJR::Vector3d);

	Boundary(const NJR::Vector3d, const NJR::Vector3d);

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

	void SetCenter(const NJR::Vector3d*);

	inline void SetBoxCenter(const NJR::Vector3d* vNewCenter)
	{
		SetCenter(vNewCenter);
	};

	void SetRange(const NJR::Vector3d*);

	inline void SetLength(const NJR::Vector3d* vNewRange)
	{
		SetRange(vNewRange);
	};

	inline const NJR::Vector3d& GetLength() const
	{
		return vRange;
	}

	inline const NJR::Vector3d& GetRange() const
	{
		return vRange;
	}

	inline const NJR::Vector3d& GetCenter() const
	{
		return vCenter;
	};

	inline const double GetVolume() const
	{
		return dVolume;
	};

	void SetLowerPoint(const NJR::Vector3d*);

	inline void SetLowerEdge(const NJR::Vector3d* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	};

	void SetUpperPoint(const NJR::Vector3d*);

	inline void SetUpperEdge(const NJR::Vector3d* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	};

	inline const NJR::Vector3d& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const NJR::Vector3d& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const NJR::Vector3d) const;

	bool InBoundary(const NJR::Vector3d, const double) const;

	void EnforceBoundaryConditions(NJR::Vector3d*) const;

	void DifferenceBoundaryConditions(NJR::Vector3d*) const;

	void print() const;

	void Correct();

private:

	std::string sName;
	bool bActive;
	bool bSwitch[3];           // Open or close
	NJR::Vector3d	vLowerPoint;   // Lower boundary
	NJR::Vector3d	vUpperPoint;   // Upper boundary
	NJR::Vector3d	vCenter;       // Center
	double dVolume;            // Volume (space inside the boundary)
	NJR::Vector3d	vRange;        // Range (length in x, y, z directions)
};

};   // namespace VEDO

#endif // _BOUNDARY_H
