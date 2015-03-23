#ifndef _SYSTEM_PARAMETER_H
#define _SYSTEM_PARAMETER_H

#include <vedo/framework/Boundary.h>
#include <vedo/framework/DataType.h>
#include <string>
#include <fstream>

namespace vedo
{

// Data Type Class
class SystemParameter
{

public:

	SystemParameter
		(const _float_t& TimeStart,
		 const _float_t& TimeStop,
		 const _float_t& TimeInterval,
		 const _float_t& TimeCurrent,
		 const aries::Vector3df& FieldAcceleration,
		 const Boundary& ZOI,
		 const Boundary& PBC                    );

	SystemParameter(const SystemParameter& sp);

	SystemParameter(std::ifstream& idof);

	const SystemParameter& operator = (const SystemParameter &sp);

	inline _float_t GetTimeStart() const
	{
		return dTimeStart;
	}

	inline _float_t GetTimeStop() const
	{
		return dTimeStop;
	}

	inline _float_t GetTimeInterval() const
	{
		return dTimeInterval;
	}

	inline _float_t GetTimeCurrent() const
	{
		return dTimeCurrent;
	}

	inline _uint_t GetDONumber() const
	{
		return ulDONumber;
	}

	inline void SetDONumber(_uint_t& n)
	{
		ulDONumber = n;
	}

	inline _uint_t GetIactNumber() const
	{
		return ulIactNumber;
	}

	inline void SetIactNumber(_uint_t& n)
	{
		ulIactNumber = n;
	}

	inline _uint_t GetMaxIDofDO() const
	{
		return ulMaxIDofDO;
	}

	inline void SetMaxIDofDO(_uint_t& n)
	{
		ulMaxIDofDO = n;
	}

	inline aries::Vector3df GetFieldAcceleration() const
	{
		return vFieldAcceleration;
	}

	inline const Boundary& GetZoneOfInterest() const
	{
		return ZoneOfInterest;
	}

	inline const Boundary& GetPeriodicBoundaryConditions() const
	{
		return PeriodicBoundaryConditions;
	}

	inline void SetTime(const _float_t& dInterval, const _float_t& dCurrent)
	{
		dTimeInterval = dInterval;
		dTimeCurrent  = dCurrent;
	}

	inline void SetTime
		(const _float_t& dStart, const _float_t& dStop, const _float_t& dInterval, const _float_t& dCurrent)
	{
		dTimeStart    = dStart;
		dTimeStop     = dStop;
		dTimeCurrent  = dCurrent;
		dTimeInterval = dInterval;
	}

	inline void NextStep()
	{
		dTimeCurrent += dTimeInterval;
	}

	inline void SetFieldAcceleration(const aries::Vector3df& FieldAcceleration)
	{
		vFieldAcceleration = FieldAcceleration;
	}

	inline void SetBoundary(const Boundary& b)
	{
		ZoneOfInterest = b;
	}

	inline void SetDONumber(const _uint_t& n)
	{
		ulDONumber = n;
	}

	inline _float_t GetEnergyPotential() const
	{
		return dEnergyPotential;
	}

	inline _float_t GetEnergyTranslation() const
	{
		return dEnergyTranslation;
	}

	inline _float_t GetEnergyRotation() const
	{
		return dEnergyRotation;
	}

	inline aries::Vector3df GetMomentumAvg() const
	{
		return vMomentumAvg;
	}

	inline aries::Vector3df GetAngularMomentumAvg() const
	{
		return vAngularMomentumAvg;
	}

	inline _float_t GetMomentumNorm() const
	{
		return dMomentumNorm;
	}

	inline _float_t GetAngularMomentumNorm() const
	{
		return dAngularMomentumNorm;
	}

	inline _float_t GetVelocityMax() const
	{
		return dVelocityMax;
	}

	inline _float_t GetVelocityMin() const
	{
		return dVelocityMin;
	}

	inline _float_t GetAngularVelocityMax() const
	{
		return dAngularVelocityMax;
	}

	inline _float_t GetAngularVelocityMin() const
	{
		return dAngularVelocityMin;
	}

	inline void SetEnergyPotential(_float_t& e)
	{
		dEnergyPotential = e;
	}

	inline void SetEnergyTranslation(_float_t& e)
	{
		dEnergyTranslation = e;
	}

	inline void SetEnergyRotation(_float_t& e)
	{
		dEnergyRotation = e;
	}

	inline void SetMomentumAvg(const aries::Vector3df& v)
	{
		vMomentumAvg = v;
	}

	inline void SetAngularMomentumAvg(const aries::Vector3df& v)
	{
		vAngularMomentumAvg = v;
	}

	inline void SetMomentumNorm(_float_t& d)
	{
		dMomentumNorm = d;
	}

	inline void SetAngularMomentumNorm(_float_t& d)
	{
		dAngularMomentumNorm = d;
	}

	inline void SetVelocityMax(const _float_t& d)
	{
		dVelocityMax = d;
	}

	inline void SetVelocityMin(const _float_t& d)
	{
		dVelocityMin = d;
	}

	inline void SetAngularVelocityMax(const _float_t& d)
	{
		dAngularVelocityMax = d;
	}

	inline void SetAngularVelocityMin(const _float_t& d)
	{
		dAngularVelocityMin = d;
	}

	//binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	//binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	// Starting time of simulate system
	_float_t dTimeStart;

	// Stopping time of simulate system
	_float_t dTimeStop;

	// Time interval of each simulate iteration
	_float_t dTimeInterval;

	// The current time of current process
	_float_t dTimeCurrent;

	// Max serial number of Discrete Objects;
	_uint_t ulMaxIDofDO;   // Aries: I have not finished this part

	// The number of Discrete Objects;
	_uint_t ulDONumber;

	// The number of Interactions;
	_uint_t ulIactNumber;   // Aries: I have not finished this part

	// The global field acceleration
	aries::Vector3df vFieldAcceleration;

	Boundary ZoneOfInterest;               // Zone of Interest (ZOI)
	Boundary PeriodicBoundaryConditions;   // Periodic Boundary Condition (PBC)

	_float_t dEnergyPotential;         // Potential energy
	_float_t dEnergyTranslation;       // Kinetic energy (Total, Translation part)
	_float_t dEnergyRotation;          // Kinetic energy (Total, Rotation part)

	aries::Vector3df vMomentumAvg;            // Average momentum
	aries::Vector3df vAngularMomentumAvg;     // Average angular momentum

	_float_t dMomentumNorm;            // Norm of momentum (Total)
	_float_t dAngularMomentumNorm;     // Norm of angular momentum (Total)

	_float_t dVelocityMin;             // Minimal velocity
	_float_t dVelocityMax;             // Maximal velocity

	_float_t dAngularVelocityMin;      // Minimal angular velocity
	_float_t dAngularVelocityMax;      // Maximal angular velocity
};

}   // namespace vedo

#endif // _SYSTEM_PARAMETER_H
