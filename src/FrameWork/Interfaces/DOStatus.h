#ifndef _DOSTATUS_H
#define _DOSTATUS_H

#include <NJR/Interfaces/vector3d.h>
#include <fstream>
#include <string>

namespace VEDO
{

// Data Type Class
class DOStatus
{

public:

	DOStatus
		(const std::string& DOName,
		const NJR::NJRvector3d& vPosition,
		const NJR::NJRvector3d& vVelocity,
		const NJR::NJRvector3d& vOrientationX,
		const NJR::NJRvector3d& vOrientationZ,
		const NJR::NJRvector3d& vAngularVelocity);

	DOStatus(const DOStatus& dos);

	DOStatus(std::ifstream& idof);

	inline NJR::NJRvector3d GetPosition() const
	{
		return vPosition;
	};

	inline NJR::NJRvector3d GetVelocity() const
	{
		return vVelocity;
	};

	inline NJR::NJRvector3d GetOrientationX() const
	{
		return vOrientationX;
	};

	inline NJR::NJRvector3d GetOrientationZ() const
	{
		return vOrientationZ;
	};

	inline NJR::NJRvector3d GetAngularVelocity() const
	{
		return vAngularVelocity;
	};

	inline std::string GetDOName() const
	{
		return sDOName;
	};

	inline void SetPosition(const NJR::NJRvector3d& position)
	{
		vPosition = position;
	};

	inline void SetVelocity(const NJR::NJRvector3d& velocity)
	{
		vVelocity = velocity;
	};

	void SetOrientation
		(const NJR::NJRvector3d& OrientationX, const NJR::NJRvector3d& OrientationZ);

	inline void SetAngularVelocity(const NJR::NJRvector3d& angularvelocity)
	{
		vAngularVelocity = angularvelocity;
	};

/*
	inline NJR::NJRvector3d GetFieldImpact() const
	{
		return vFieldImpact;
	};

	inline void SetFieldImpact(const NJR::NJRvector3d vi)
	{
		vFieldImpact = vi;
	};
*/

	// Function for algorithm std::find_if
	explicit DOStatus(const std::string& DOname);

	inline bool ISMonitored() const
	{
		return beMonitored;
	}

	inline void SetMonitored(bool m)
	{
		beMonitored = m;
	}

	inline double GetGranularTemperatureV() const
	{
		return dGranularTemperatureV;
	}

	inline void SetGranularTemperatureV(double& m)
	{
		dGranularTemperatureV = m;
	}

	inline double GetGranularTemperatureAV() const
	{
		return dGranularTemperatureAV;
	}

	inline void SetGranularTemperatureAV(double& m)
	{
		dGranularTemperatureAV = m;
	}

	bool operator () (const DOStatus* p) const;

	const DOStatus& operator = (const DOStatus&);

	// Binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	// Binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	NJR::NJRvector3d vPosition;
	NJR::NJRvector3d vVelocity;
	NJR::NJRvector3d vOrientationX;
	NJR::NJRvector3d vOrientationZ;
	NJR::NJRvector3d vAngularVelocity;
	std::string	sDOName;
/******************************************************************************
 * Aries' Comment (2006/04/11)
 *
 *    Each discrete element should know its "potential" influential range. This
 * variables is modified usually and useful for system to determine if elements
 * may be contect with each other in "potentially".
 ******************************************************************************/
	double      dGranularTemperatureV;    // Granular Temperature (Velocity)
	double      dGranularTemperatureAV;   // Granular Temperature (Angular Velocity)
	bool        beMonitored;              // Should be monotored or not;
	//NJR::NJRvector3d vFieldImpact;             // Field Impact
};

};   // namespace VEDO

#endif // _DOSTATUS_H
