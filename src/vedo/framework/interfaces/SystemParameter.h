#ifndef _SYSTEM_PARAMETER_H
#define _SYSTEM_PARAMETER_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/framework/interfaces/Boundary.h>
#include <string>
#include <fstream>

namespace vedo
{

// Data Type Class
class SystemParameter
{

public:

	SystemParameter
		(const vedo_float_t& TimeStart,
		 const vedo_float_t& TimeStop,
		 const vedo_float_t& TimeInterval,
		 const vedo_float_t& TimeCurrent,
		 const njr::Vector3d& FieldAcceleration,
		 const Boundary& ZOI,
		 const Boundary& PBC                    );

	SystemParameter(const SystemParameter& sp);

	SystemParameter(std::ifstream& idof);

	const SystemParameter& operator = (const SystemParameter &sp);

	inline vedo_float_t GetTimeStart() const
	{
		return dTimeStart;
	}

	inline vedo_float_t GetTimeStop() const
	{
		return dTimeStop;
	}

	inline vedo_float_t GetTimeInterval() const
	{
		return dTimeInterval;
	}

	inline vedo_float_t GetTimeCurrent() const
	{
		return dTimeCurrent;
	}

	inline vedo::vedo_uint_t GetDONumber() const
	{
		return ulDONumber;
	}

	inline void SetDONumber(vedo::vedo_uint_t& n)
	{
		ulDONumber = n;
	}

	inline vedo::vedo_uint_t GetIactNumber() const
	{
		return ulIactNumber;
	}

	inline void SetIactNumber(vedo::vedo_uint_t& n)
	{
		ulIactNumber = n;
	}

	inline vedo::vedo_uint_t GetMaxIDofDO() const
	{
		return ulMaxIDofDO;
	}

	inline void SetMaxIDofDO(vedo::vedo_uint_t& n)
	{
		ulMaxIDofDO = n;
	}

	inline njr::Vector3d GetFieldAcceleration() const
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

	inline void SetTime(const vedo_float_t& dInterval, const vedo_float_t& dCurrent)
	{
		dTimeInterval = dInterval;
		dTimeCurrent  = dCurrent;
	}

	inline void SetTime
		(const vedo_float_t& dStart, const vedo_float_t& dStop, const vedo_float_t& dInterval, const vedo_float_t& dCurrent)
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

	inline void SetFieldAcceleration(const njr::Vector3d& FieldAcceleration)
	{
		vFieldAcceleration = FieldAcceleration;
	}

	inline void SetBoundary(const Boundary& b)
	{
		ZoneOfInterest = b;
	}

	inline void SetDONumber(const vedo::vedo_uint_t& n)
	{
		ulDONumber = n;
	}

	inline vedo_float_t GetEnergyPotential() const
	{
		return dEnergyPotential;
	}

	inline vedo_float_t GetEnergyTranslation() const
	{
		return dEnergyTranslation;
	}

	inline vedo_float_t GetEnergyRotation() const
	{
		return dEnergyRotation;
	}

	inline njr::Vector3d GetMomentumAvg() const
	{
		return vMomentumAvg;
	}

	inline njr::Vector3d GetAngularMomentumAvg() const
	{
		return vAngularMomentumAvg;
	}

	inline vedo_float_t GetMomentumNorm() const
	{
		return dMomentumNorm;
	}

	inline vedo_float_t GetAngularMomentumNorm() const
	{
		return dAngularMomentumNorm;
	}

	inline vedo_float_t GetVelocityMax() const
	{
		return dVelocityMax;
	}

	inline vedo_float_t GetVelocityMin() const
	{
		return dVelocityMin;
	}

	inline vedo_float_t GetAngularVelocityMax() const
	{
		return dAngularVelocityMax;
	}

	inline vedo_float_t GetAngularVelocityMin() const
	{
		return dAngularVelocityMin;
	}

	inline void SetEnergyPotential(vedo_float_t& e)
	{
		dEnergyPotential = e;
	}

	inline void SetEnergyTranslation(vedo_float_t& e)
	{
		dEnergyTranslation = e;
	}

	inline void SetEnergyRotation(vedo_float_t& e)
	{
		dEnergyRotation = e;
	}

	inline void SetMomentumAvg(const njr::Vector3d& v)
	{
		vMomentumAvg = v;
	}

	inline void SetAngularMomentumAvg(const njr::Vector3d& v)
	{
		vAngularMomentumAvg = v;
	}

	inline void SetMomentumNorm(vedo_float_t& d)
	{
		dMomentumNorm = d;
	}

	inline void SetAngularMomentumNorm(vedo_float_t& d)
	{
		dAngularMomentumNorm = d;
	}

	inline void SetVelocityMax(const vedo_float_t& d)
	{
		dVelocityMax = d;
	}

	inline void SetVelocityMin(const vedo_float_t& d)
	{
		dVelocityMin = d;
	}

	inline void SetAngularVelocityMax(const vedo_float_t& d)
	{
		dAngularVelocityMax = d;
	}

	inline void SetAngularVelocityMin(const vedo_float_t& d)
	{
		dAngularVelocityMin = d;
	}

	//binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	//binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	// Starting time of simulate system
	vedo_float_t dTimeStart;

	// Stopping time of simulate system
	vedo_float_t dTimeStop;

	// Time interval of each simulate iteration
	vedo_float_t dTimeInterval;

	// The current time of current process
	vedo_float_t dTimeCurrent;

	// The number of SimConstants (vedo::uSizeOfConstant + ulMaxIDofDO)
	vedo_uint_t uSizeOfSimConstant;

	// Max serial number of Discrete Objects;
	vedo::vedo_uint_t ulMaxIDofDO;   // Aries: I have not finished this part

	// The number of Discrete Objects;
	vedo::vedo_uint_t ulDONumber;

	// The number of Interactions;
	vedo::vedo_uint_t ulIactNumber;   // Aries: I have not finished this part

	// The global field acceleration
	njr::Vector3d vFieldAcceleration;

	Boundary ZoneOfInterest;               // Zone of Interest (ZOI)
	Boundary PeriodicBoundaryConditions;   // Periodic Boundary Condition (PBC)

	vedo_float_t dEnergyPotential;         // Potential energy
	vedo_float_t dEnergyTranslation;       // Kinetic energy (Total, Translation part)
	vedo_float_t dEnergyRotation;          // Kinetic energy (Total, Rotation part)

	njr::Vector3d vMomentumAvg;            // Average momentum
	njr::Vector3d vAngularMomentumAvg;     // Average angular momentum

	vedo_float_t dMomentumNorm;            // Norm of momentum (Total)
	vedo_float_t dAngularMomentumNorm;     // Norm of angular momentum (Total)

	vedo_float_t dVelocityMin;             // Minimal velocity
	vedo_float_t dVelocityMax;             // Maximal velocity

	vedo_float_t dAngularVelocityMin;      // Minimal angular velocity
	vedo_float_t dAngularVelocityMax;      // Maximal angular velocity
};

}   // namespace vedo

#endif // _SYSTEM_PARAMETER_H
