#ifndef _BOUNDARY_H
#define _BOUNDARY_H

#include <vedo/framework/DataType.h>
#include <string>

namespace vedo
{

class Boundary
{

public:

	Boundary();

	Boundary(std::string, const bool*, const aries::Vector3df, const aries::Vector3df);

	Boundary(const bool*, const aries::Vector3df, const aries::Vector3df);

	Boundary(std::string, const aries::Vector3df, const aries::Vector3df);

	Boundary(const aries::Vector3df, const aries::Vector3df);

	Boundary(const Boundary&);

	virtual ~Boundary();

	void operator = (const Boundary&);

	inline void SetName(std::string sN)
	{
		sName = sN;
	}

	inline const std::string GetName() const
	{
		return sName;
	}

	inline const bool Active() const
	{
		return bActive;
	}

	inline const bool& GetSwitch() const
	{
		return bSwitch[0];
	}

	inline const bool& GetSwitch(_uint_t i) const
	{
		return bSwitch[i];
	}

	void SetCenter(const aries::Vector3df*);

	inline void SetBoxCenter(const aries::Vector3df* vNewCenter)
	{
		SetCenter(vNewCenter);
	}

	void SetRange(const aries::Vector3df*);

	inline void SetLength(const aries::Vector3df* vNewRange)
	{
		SetRange(vNewRange);
	}

	inline const aries::Vector3df& GetLength() const
	{
		return vRange;
	}

	inline const aries::Vector3df& GetRange() const
	{
		return vRange;
	}

	inline const aries::Vector3df& GetCenter() const
	{
		return vCenter;
	};

	inline const _float_t GetVolume() const
	{
		return dVolume;
	};

	void SetLowerPoint(const aries::Vector3df*);

	inline void SetLowerEdge(const aries::Vector3df* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	}

	void SetUpperPoint(const aries::Vector3df*);

	inline void SetUpperEdge(const aries::Vector3df* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	}

	inline const aries::Vector3df& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const aries::Vector3df& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const aries::Vector3df) const;

	bool InBoundary(const aries::Vector3df, const _float_t) const;

	void EnforceBoundaryConditions(aries::Vector3df*) const;

	void DifferenceBoundaryConditions(aries::Vector3df*) const;

	void print() const;

	void Correct();

private:

	std::string   sName;

	bool          bActive;

	bool          bSwitch[3];    // Open or close

	aries::Vector3df vLowerPoint;   // Lower boundary

	aries::Vector3df vUpperPoint;   // Upper boundary

	aries::Vector3df vCenter;       // Center

	_float_t  dVolume;       // Volume (space inside the boundary)

	aries::Vector3df vRange;        // Range (length in x, y, z directions)
};

}   // namespace vedo

#endif // _BOUNDARY_H
