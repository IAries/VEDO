#ifndef _DOSTATUS_H
#define _DOSTATUS_H

#include <vedo/constants/interfaces/Constants.h>
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
		(const std::string& DOName, const njr::Vector3d& Position, const njr::Vector3d& Velocity,
		 const njr::Vector3d& OrientationX, const njr::Vector3d& OrientationZ,
		 const njr::Vector3d& AngularVelocity, const njr::Vector3d& Impact, const njr::Vector3d& AngularImpact);

	// Function for algorithm std::find_if
	explicit DOStatus(const std::string& DOname);

	DOStatus(const DOStatus& dos);

	const DOStatus& operator = (const DOStatus&);

	~DOStatus();

	DOStatus(std::ifstream& idof);

	bool operator () (const DOStatus* p) const;

	inline vedo_uint_t GetID() const
	{
		return ulID;
	}

	inline njr::Vector3d GetPosition() const
	{
		return vPosition;
	}

	inline njr::Vector3d GetVelocity() const
	{
		return vVelocity;
	}

	inline njr::Vector3d GetOrientationX() const
	{
		return vOrientationX;
	}

	inline njr::Vector3d GetOrientationZ() const
	{
		return vOrientationZ;
	}

	inline njr::Vector3d GetAngularVelocity() const
	{
		return vAngularVelocity;
	}

	inline njr::Vector3d GetImpact() const
	{
		return vImpact;
	}

	inline njr::Vector3d GetAngularImpact() const
	{
		return vAngularImpact;
	}

	inline std::string GetDOName() const
	{
		return sDOName;
	}

	inline void SetID(const vedo_uint_t id)
	{
		ulID = id;
	}

	inline void SetPosition(const njr::Vector3d& position)
	{
		vPosition = position;
	}

	inline void SetVelocity(const njr::Vector3d& velocity)
	{
		vVelocity = velocity;
	}

	void SetOrientation(const njr::Vector3d& OrientationX, const njr::Vector3d& OrientationZ);

	inline void SetOrientationX(const njr::Vector3d& OrientationX)
	{
		vOrientationX = OrientationX;
	}

	inline void SetOrientationZ(const njr::Vector3d& OrientationZ)
	{
		vOrientationZ = OrientationZ;
	}

	inline void SetAngularVelocity(const njr::Vector3d& angularvelocity)
	{
		vAngularVelocity = angularvelocity;
	}

	inline void SetImpact(const njr::Vector3d& impact)
	{
		vImpact = impact;
	}

	inline void SetAngularImpact(const njr::Vector3d& angularimpact)
	{
		vAngularImpact = angularimpact;
	}

/*
	void SetUserDefinedValue(vedo_uint_t u, vedo_float_t d);

	vedo_float_t GetUserDefinedValue(vedo_uint_t u) const;

	void AddAccumulativeUserDefinedValue(vedo_uint_t u, vedo_float_t d);

	const vedo_float_t* RetrieveUserDefinedValue();

	const vedo_float_t* RetrieveAllUserDefinedValue() const;

	void SetAllUserDefinedValue(const vedo_float_t*);

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

	inline vedo_float_t GetGranularTemperatureV() const
	{
		return dGranularTemperatureV;
	}

	inline void SetGranularTemperatureV(vedo_float_t& m)
	{
		dGranularTemperatureV = m;
	}

	inline vedo_float_t GetGranularTemperatureAV() const
	{
		return dGranularTemperatureAV;
	}

	inline void SetGranularTemperatureAV(vedo_float_t& m)
	{
		dGranularTemperatureAV = m;
	}

	// Binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	// Binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	std::string	  sDOName;

	vedo_uint_t ulID;   // ID during simulation (not equals to the serial number in DOContainer

	njr::Vector3d vPosition;

	njr::Vector3d vVelocity;

	njr::Vector3d vOrientationX;

	njr::Vector3d vOrientationZ;

	njr::Vector3d vAngularVelocity;

	njr::Vector3d vImpact;

	njr::Vector3d vAngularImpact;

	vedo_float_t  dGranularTemperatureV;    // Granular Temperature (Velocity)

	vedo_float_t  dGranularTemperatureAV;   // Granular Temperature (Angular Velocity)

	bool          beMonitored;              // Should be monotored or not;

	//njr::Vector3d vFieldImpact;           // Field Impact

//	vedo_float_t        dUDV[uNumUDDDOStatus];    // User-defined value
                                            // 0 ~ uNumUDDDOStatus: Accumulative user-defined value
                                            // uNumUDDDOStatus ~ 2*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value
                                            // 2*uNumUDDDOStatus ~ 3*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                            // 3*uNumUDDDOStatus ~ 4*uNumUDDDOStatus-1: User-defined value
};

}   // namespace vedo

#endif // _DOSTATUS_H
