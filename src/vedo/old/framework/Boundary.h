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

	Boundary(std::string, const bool*, const Vector3df, const Vector3df);

	Boundary(const bool*, const Vector3df, const Vector3df);

	Boundary(std::string, const Vector3df, const Vector3df);

	Boundary(const Vector3df, const Vector3df);

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

	void SetCenter(const Vector3df*);

	inline void SetBoxCenter(const Vector3df* vNewCenter)
	{
		SetCenter(vNewCenter);
	}

	void SetRange(const Vector3df*);

	inline void SetLength(const Vector3df* vNewRange)
	{
		SetRange(vNewRange);
	}

	inline const Vector3df& GetLength() const
	{
		return vRange;
	}

	inline const Vector3df& GetRange() const
	{
		return vRange;
	}

	inline const Vector3df& GetCenter() const
	{
		return vCenter;
	};

	inline const _float_t GetVolume() const
	{
		return dVolume;
	};

	void SetLowerPoint(const Vector3df*);

	inline void SetLowerEdge(const Vector3df* vNewEdge)
	{
		SetLowerPoint(vNewEdge);
	}

	void SetUpperPoint(const Vector3df*);

	inline void SetUpperEdge(const Vector3df* vNewEdge)
	{
		SetUpperPoint(vNewEdge);
	}

	inline const Vector3df& GetLowerPoint() const
	{
		return vLowerPoint;
	}

	inline const Vector3df& GetUpperPoint() const
	{
		return vUpperPoint;
	}

	bool InBoundary(const Vector3df) const;

	bool InBoundary(const Vector3df, const _float_t) const;

	void EnforceBoundaryConditions(Vector3df*) const;

	void DifferenceBoundaryConditions(Vector3df*) const;

	void print() const;

	void Correct();

private:

	std::string   sName;

	bool          bActive;

	bool          bSwitch[3];    // Open or close

	Vector3df vLowerPoint;   // Lower boundary

	Vector3df vUpperPoint;   // Upper boundary

	Vector3df vCenter;       // Center

	_float_t  dVolume;       // Volume (space inside the boundary)

	Vector3df vRange;        // Range (length in x, y, z directions)
};

}   // namespace vedo

#endif // _BOUNDARY_H
