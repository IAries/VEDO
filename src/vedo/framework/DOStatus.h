#ifndef _DOSTATUS_H
#define _DOSTATUS_H

#include <aries/utility/Constants.h>
#include <vedo/framework/DataType.h>
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
		(const std::string& DOName, const aries::Vector3df& Position, const aries::Vector3df& Velocity,
		 const aries::Vector3df& OrientationX, const aries::Vector3df& OrientationZ,
		 const aries::Vector3df& AngularVelocity, const aries::Vector3df& Impact, const aries::Vector3df& AngularImpact);

	// Function for algorithm std::find_if
	explicit DOStatus(const std::string& DOname);

	DOStatus(const DOStatus& dos);

	const DOStatus& operator = (const DOStatus&);

	~DOStatus();

	DOStatus(std::ifstream& idof);

	bool operator () (const DOStatus* p) const;

	inline _uint_t GetID() const
	{
		return ulID;
	}

	inline aries::Vector3df GetPosition() const
	{
		return vPosition;
	}

	inline aries::Vector3df GetVelocity() const
	{
		return vVelocity;
	}

	inline aries::Vector3df GetOrientationX() const
	{
		return vOrientationX;
	}

	inline aries::Vector3df GetOrientationZ() const
	{
		return vOrientationZ;
	}

	inline aries::Vector3df GetAngularVelocity() const
	{
		return vAngularVelocity;
	}

	inline aries::Vector3df GetImpact() const
	{
		return vImpact;
	}

	inline aries::Vector3df GetAngularImpact() const
	{
		return vAngularImpact;
	}

	inline std::string GetDOName() const
	{
		return sDOName;
	}

	inline void SetID(const _uint_t id)
	{
		ulID = id;
	}

	inline void SetPosition(const aries::Vector3df& position)
	{
		vPosition = position;
	}

	inline void SetVelocity(const aries::Vector3df& velocity)
	{
		vVelocity = velocity;
	}

	void SetOrientation(const aries::Vector3df& OrientationX, const aries::Vector3df& OrientationZ);

	inline void SetOrientationX(const aries::Vector3df& OrientationX)
	{
		vOrientationX = OrientationX;
	}

	inline void SetOrientationZ(const aries::Vector3df& OrientationZ)
	{
		vOrientationZ = OrientationZ;
	}

	inline void SetAngularVelocity(const aries::Vector3df& angularvelocity)
	{
		vAngularVelocity = angularvelocity;
	}

	inline void SetImpact(const aries::Vector3df& impact)
	{
		vImpact = impact;
	}

	inline void SetAngularImpact(const aries::Vector3df& angularimpact)
	{
		vAngularImpact = angularimpact;
	}

/*
	void SetUserDefinedValue(_uint_t u, _float_t d);

	_float_t GetUserDefinedValue(_uint_t u) const;

	void AddAccumulativeUserDefinedValue(_uint_t u, _float_t d);

	const _float_t* RetrieveUserDefinedValue();

	const _float_t* RetrieveAllUserDefinedValue() const;

	void SetAllUserDefinedValue(const _float_t*);

	void CleanAllUserDefinedValue();

	void CleanUserDefinedValue();

	void CleanAccumulativeUserDefinedValue();

	void Clean();
*/

/*
	inline aries::Vector3df GetFieldImpact() const
	{
		return vFieldImpact;
	};

	inline void SetFieldImpact(const aries::Vector3df vi)
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

	inline _float_t GetGranularTemperatureV() const
	{
		return dGranularTemperatureV;
	}

	inline void SetGranularTemperatureV(_float_t& m)
	{
		dGranularTemperatureV = m;
	}

	inline _float_t GetGranularTemperatureAV() const
	{
		return dGranularTemperatureAV;
	}

	inline void SetGranularTemperatureAV(_float_t& m)
	{
		dGranularTemperatureAV = m;
	}

	// Binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	// Binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	std::string	  sDOName;

	_uint_t ulID;   // ID during simulation (not equals to the serial number in DOContainer

	aries::Vector3df vPosition;

	aries::Vector3df vVelocity;

	aries::Vector3df vOrientationX;

	aries::Vector3df vOrientationZ;

	aries::Vector3df vAngularVelocity;

	aries::Vector3df vImpact;

	aries::Vector3df vAngularImpact;

	_float_t  dGranularTemperatureV;    // Granular Temperature (Velocity)

	_float_t  dGranularTemperatureAV;   // Granular Temperature (Angular Velocity)

	bool          beMonitored;              // Should be monotored or not;

	//aries::Vector3df vFieldImpact;           // Field Impact

//	_float_t        dUDV[uNumUDDDOStatus];    // User-defined value
                                            // 0 ~ uNumUDDDOStatus: Accumulative user-defined value
                                            // uNumUDDDOStatus ~ 2*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value
                                            // 2*uNumUDDDOStatus ~ 3*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                            // 3*uNumUDDDOStatus ~ 4*uNumUDDDOStatus-1: User-defined value
};

}   // namespace vedo

#endif // _DOSTATUS_H
