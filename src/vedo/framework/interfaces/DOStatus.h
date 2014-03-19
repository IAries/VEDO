#ifndef _DOSTATUS_H
#define _DOSTATUS_H

#include <vedo/Constants.h>
#include <vedo/njr/interfaces/Vector3d.h>
#include <fstream>
#include <string>

namespace vedo
{

// Data Type Class
class DOStatus
{

public:

    DOStatus();

	DOStatus
		(const std::string& DOName,
		const njr::Vector3d& Position,
		const njr::Vector3d& Velocity,
		const njr::Vector3d& OrientationX,
		const njr::Vector3d& OrientationZ,
		const njr::Vector3d& AngularVelocity,
		const njr::Vector3d& Impact,
		const njr::Vector3d& AngularImpact   );

	// Function for algorithm std::find_if
	explicit DOStatus(const std::string& DOname);

	DOStatus(const DOStatus& dos);

	const DOStatus& operator = (const DOStatus&);

	~DOStatus();

	DOStatus(std::ifstream& idof);

	bool operator () (const DOStatus* p) const;

	inline unsigned long GetID() const
	{
		return ulID;
	};

	inline njr::Vector3d GetPosition() const
	{
		return vPosition;
	};

	inline njr::Vector3d GetVelocity() const
	{
		return vVelocity;
	};

	inline njr::Vector3d GetOrientationX() const
	{
		return vOrientationX;
	};

	inline njr::Vector3d GetOrientationZ() const
	{
		return vOrientationZ;
	};

	inline njr::Vector3d GetAngularVelocity() const
	{
		return vAngularVelocity;
	};

	inline njr::Vector3d GetImpact() const
	{
		return vImpact;
	};

	inline njr::Vector3d GetAngularImpact() const
	{
		return vAngularImpact;
	};

	inline std::string GetDOName() const
	{
		return sDOName;
	};

	inline void SetID(const unsigned long id)
	{
		ulID = id;
	};

	inline void SetPosition(const njr::Vector3d& position)
	{
		vPosition = position;
	};

	inline void SetVelocity(const njr::Vector3d& velocity)
	{
		vVelocity = velocity;
	};

	void SetOrientation
		(const njr::Vector3d& OrientationX, const njr::Vector3d& OrientationZ);

	inline void SetOrientationX(const njr::Vector3d& OrientationX)
	{
		vOrientationX = OrientationX;
	};

	inline void SetOrientationZ(const njr::Vector3d& OrientationZ)
	{
		vOrientationZ = OrientationZ;
	};

	inline void SetAngularVelocity(const njr::Vector3d& angularvelocity)
	{
		vAngularVelocity = angularvelocity;
	};

	inline void SetImpact(const njr::Vector3d& impact)
	{
		vImpact = impact;
	};

	inline void SetAngularImpact(const njr::Vector3d& angularimpact)
	{
		vAngularImpact = angularimpact;
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
	inline njr::Vector3d GetFieldImpact() const
	{
		return vFieldImpact;
	};

	inline void SetFieldImpact(const njr::Vector3d vi)
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

	std::string	  sDOName;
	unsigned long ulID;   // ID during simulation (not equals to the serial number in DOContainer
	njr::Vector3d vPosition;
	njr::Vector3d vVelocity;
	njr::Vector3d vOrientationX;
	njr::Vector3d vOrientationZ;
	njr::Vector3d vAngularVelocity;
	njr::Vector3d vImpact;
	njr::Vector3d vAngularImpact;
	double        dGranularTemperatureV;    // Granular Temperature (Velocity)
	double        dGranularTemperatureAV;   // Granular Temperature (Angular Velocity)
	bool          beMonitored;              // Should be monotored or not;
	//njr::Vector3d vFieldImpact;           // Field Impact
//	double        dUDV[uNumUDDDOStatus];    // User-defined value
                                            // 0 ~ uNumUDDDOStatus: Accumulative user-defined value
                                            // uNumUDDDOStatus ~ 2*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value
                                            // 2*uNumUDDDOStatus ~ 3*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                            // 3*uNumUDDDOStatus ~ 4*uNumUDDDOStatus-1: User-defined value
};

};   // namespace vedo

#endif // _DOSTATUS_H
