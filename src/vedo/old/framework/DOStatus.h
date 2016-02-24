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
		(const std::string& DOName, const Vector3df& Position, const Vector3df& Velocity,
		 const Vector3df& OrientationX, const Vector3df& OrientationZ,
		 const Vector3df& AngularVelocity, const Vector3df& Impact, const Vector3df& AngularImpact);

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

	inline Vector3df GetPosition() const
	{
		return vPosition;
	}

	inline Vector3df GetVelocity() const
	{
		return vVelocity;
	}

	inline Vector3df GetOrientationX() const
	{
		return vOrientationX;
	}

	inline Vector3df GetOrientationZ() const
	{
		return vOrientationZ;
	}

	inline Vector3df GetAngularVelocity() const
	{
		return vAngularVelocity;
	}

	inline Vector3df GetImpact() const
	{
		return vImpact;
	}

	inline Vector3df GetAngularImpact() const
	{
		return vAngularImpact;
	}

	inline std::string GetDOName() const
	{
		return sDOName;
	}

	inline void ChangeDOName(const std::string& NewDOName)
	{
		sDOName = NewDOName;
	}

	inline void SetID(const _uint_t id)
	{
		ulID = id;
	}

	inline void SetPosition(const Vector3df& position)
	{
		vPosition = position;
	}

	inline void SetVelocity(const Vector3df& velocity)
	{
		vVelocity = velocity;
	}

	void SetOrientation(const Vector3df& OrientationX, const Vector3df& OrientationZ);

	inline void SetOrientationX(const Vector3df& OrientationX)
	{
		vOrientationX = OrientationX;
	}

	inline void SetOrientationZ(const Vector3df& OrientationZ)
	{
		vOrientationZ = OrientationZ;
	}

	inline void SetAngularVelocity(const Vector3df& angularvelocity)
	{
		vAngularVelocity = angularvelocity;
	}

	inline void SetImpact(const Vector3df& impact)
	{
		vImpact = impact;
	}

	inline void SetAngularImpact(const Vector3df& angularimpact)
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
	inline Vector3df GetFieldImpact() const
	{
		return vFieldImpact;
	};

	inline void SetFieldImpact(const Vector3df vi)
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

	std::string	sDOName;

	_uint_t ulID;   // ID during simulation (not equals to the serial number in DOContainer

	Vector3df vPosition;

	Vector3df vVelocity;

	Vector3df vOrientationX;

	Vector3df vOrientationZ;

	Vector3df vAngularVelocity;

	Vector3df vImpact;

	Vector3df vAngularImpact;

	_float_t  dGranularTemperatureV;    // Granular Temperature (Velocity)

	_float_t  dGranularTemperatureAV;   // Granular Temperature (Angular Velocity)

	bool      beMonitored;              // Should be monotored or not;

	//Vector3df vFieldImpact;           // Field Impact

//	_float_t        dUDV[uNumUDDDOStatus];    // User-defined value
                                            // 0 ~ uNumUDDDOStatus: Accumulative user-defined value
                                            // uNumUDDDOStatus ~ 2*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value
                                            // 2*uNumUDDDOStatus ~ 3*uNumUDDDOStatus-1: Unsynchronized part of Accumulative user-defined value (to be referenced)
                                            // 3*uNumUDDDOStatus ~ 4*uNumUDDDOStatus-1: User-defined value
};

}   // namespace vedo

#endif // _DOSTATUS_H
