#ifndef _DOSTATUS_H
#define _DOSTATUS_H

#include <NJR/Interfaces/Vector3d.h>
#include <FrameWork/Interfaces/Constants.h>
#include <fstream>
#include <string>

namespace VEDO
{

// Data Type Class
class DOStatus
{

public:

    DOStatus();

	DOStatus
		(const std::string& DOName,
		const NJR::Vector3d& vPosition,
		const NJR::Vector3d& vVelocity,
		const NJR::Vector3d& vOrientationX,
		const NJR::Vector3d& vOrientationZ,
		const NJR::Vector3d& vAngularVelocity);

	// Function for algorithm std::find_if
	explicit DOStatus(const std::string& DOname);

	DOStatus(const DOStatus& dos);

	const DOStatus& operator = (const DOStatus&);

	~DOStatus();

	DOStatus(std::ifstream& idof);

	bool operator () (const DOStatus* p) const;

	inline NJR::Vector3d GetPosition() const
	{
		return vPosition;
	};

	inline NJR::Vector3d GetVelocity() const
	{
		return vVelocity;
	};

	inline NJR::Vector3d GetOrientationX() const
	{
		return vOrientationX;
	};

	inline NJR::Vector3d GetOrientationZ() const
	{
		return vOrientationZ;
	};

	inline NJR::Vector3d GetAngularVelocity() const
	{
		return vAngularVelocity;
	};

	inline std::string GetDOName() const
	{
		return sDOName;
	};

	inline void SetPosition(const NJR::Vector3d& position)
	{
		vPosition = position;
	};

	inline void SetVelocity(const NJR::Vector3d& velocity)
	{
		vVelocity = velocity;
	};

	void SetOrientation
		(const NJR::Vector3d& OrientationX, const NJR::Vector3d& OrientationZ);

	inline void SetAngularVelocity(const NJR::Vector3d& angularvelocity)
	{
		vAngularVelocity = angularvelocity;
	};

/*
	void SetUserDefinedValue(unsigned u, double d);

	double GetUserDefinedValue(unsigned u) const;

	void AddAccumulativeUserDefinedValue(unsigned u, double d);

	const double* RetrieveUserDefinedValue();

	const double* RetrieveAllUserDefinedValue() const;

	void SetAllUserDefinedValue(const double*);

	void CleanAllUserDefinedValue();

	void CleanUserDefinedValue();

	void CleanAccumulativeUserDefinedValue();

	void Clean();
*/

/*
	inline NJR::Vector3d GetFieldImpact() const
	{
		return vFieldImpact;
	};

	inline void SetFieldImpact(const NJR::Vector3d vi)
	{
		vFieldImpact = vi;
	};
*/

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

	// Binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	// Binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	NJR::Vector3d vPosition;
	NJR::Vector3d vVelocity;
	NJR::Vector3d vOrientationX;
	NJR::Vector3d vOrientationZ;
	NJR::Vector3d vAngularVelocity;
	std::string	  sDOName;
	double        dGranularTemperatureV;    // Granular Temperature (Velocity)
	double        dGranularTemperatureAV;   // Granular Temperature (Angular Velocity)
	bool          beMonitored;              // Should be monotored or not;
	//NJR::Vector3d vFieldImpact;           // Field Impact
//	double        dUDV[uNumUDDDOStatus];    // User-defined value
                                            // 0 ~ uNumUDDDOStatus: Accumulative user-defined value
                                            // uNumUDDDOStatus ~ 2*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value
                                            // 2*uNumUDDDOStatus ~ 3*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                            // 3*uNumUDDDOStatus ~ 4*uNumUDDDOStatus-1: User-defined value
};

};   // namespace VEDO

#endif // _DOSTATUS_H
