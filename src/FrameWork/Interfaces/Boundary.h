#ifndef _BOUNDARY_H
#define _BOUNDARY_H

#include <NJR/Interfaces/Vector3d.h>
#include <string>

namespace vedo
{

class Boundary
{

public:

	Boundary();

	Boundary(std::string, const bool*, const njr::Vector3d, const njr::Vector3d);

	Boundary(const bool*, const njr::Vector3d, const njr::Vector3d);

	Boundary(std::string, const njr::Vector3d, const njr::Vector3d);

	Boundary(const njr::Vector3d, const njr::Vector3d);

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

	void SetCenter(const njr::Vector3d*);

	inline void SetBoxCenter(const njr::Vector3d* vNewCenter)
	{
		SetCenter(vNewCenter);
	};

	void SetRange(const njr::Vector3d*);

	inline void SetLength(const njr::Vector3d* vNewRange)
	{
		SetRange(vNewRange);
	};

	inline const njr::Vector3d& GetLength() const
	{
		return vRange;
	}

	inline const njr::Vector3d& GetRange() const
	{
		return vRange;
	}

	inline const njr::Vector3d& GetCenter() const
	{
		return vCenter;
	};

	inline const double GetVolume() const
	{
		return dVolume;
	};

	void SetLowerPoint(const njr::Vector3d*);

	inline void SetLowerEdge(const njr::Vector3d* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	};

	void SetUpperPoint(const njr::Vector3d*);

	inline void SetUpperEdge(const njr::Vector3d* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	};

	inline const njr::Vector3d& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const njr::Vector3d& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const njr::Vector3d) const;

	bool InBoundary(const njr::Vector3d, const double) const;

	void EnforceBoundaryConditions(njr::Vector3d*) const;

	void DifferenceBoundaryConditions(njr::Vector3d*) const;

	void print() const;

	void Correct();

private:

	std::string   sName;
	bool          bActive;
	bool          bSwitch[3];    // Open or close
	njr::Vector3d vLowerPoint;   // Lower boundary
	njr::Vector3d vUpperPoint;   // Upper boundary
	njr::Vector3d vCenter;       // Center
	double        dVolume;       // Volume (space inside the boundary)
	njr::Vector3d vRange;        // Range (length in x, y, z directions)
};

};   // namespace vedo

#endif // _BOUNDARY_H
