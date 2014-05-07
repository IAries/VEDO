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
		(const double& TimeStart,
		 const double& TimeStop,
		 const double& TimeInterval,
		 const double& TimeCurrent,
		 const njr::Vector3d& FieldAcceleration,
		 const Boundary& ZOI,
		 const Boundary& PBC                    );

	SystemParameter(const SystemParameter& sp);

	SystemParameter(std::ifstream& idof);

	const SystemParameter& operator = (const SystemParameter &sp);

	inline double GetTimeStart() const
	{
		return dTimeStart;
	};

	inline double GetTimeStop() const
	{
		return dTimeStop;
	};

	inline double GetTimeInterval() const
	{
		return dTimeInterval;
	};

	inline double GetTimeCurrent() const
	{
		return dTimeCurrent;
	};

	inline unsigned long GetDONumber() const
	{
		return ulDONumber;
	};

	inline void SetDONumber(unsigned long& n)
	{
		ulDONumber = n;
	};

	inline unsigned long GetIactNumber() const
	{
		return ulIactNumber;
	};

	inline void SetIactNumber(unsigned long& n)
	{
		ulIactNumber = n;
	};

	inline unsigned long GetMaxIDofDO() const
	{
		return ulMaxIDofDO;
	};

	inline void SetMaxIDofDO(unsigned long& n)
	{
		ulMaxIDofDO = n;
	};

	inline njr::Vector3d GetFieldAcceleration() const
	{
		return vFieldAcceleration;
	};

	inline const Boundary& GetZoneOfInterest() const
	{
		return ZoneOfInterest;
	};

	inline const Boundary& GetPeriodicBoundaryConditions() const
	{
		return PeriodicBoundaryConditions;
	};

	inline void SetTime(const double& dInterval, const double& dCurrent)
	{
		dTimeInterval = dInterval;
		dTimeCurrent  = dCurrent;
	};

	inline void SetTime
		(const double& dStart,
		 const double& dStop,
		 const double& dInterval,
		 const double& dCurrent  )
	{
		dTimeStart    = dStart;
		dTimeStop     = dStop;
		dTimeCurrent  = dCurrent;
		dTimeInterval = dInterval;
	};

	inline void NextStep()
	{
		dTimeCurrent += dTimeInterval;
	};

	inline void SetFieldAcceleration(const njr::Vector3d& FieldAcceleration)
	{
		vFieldAcceleration = FieldAcceleration;
	};

	inline void SetBoundary(const Boundary& b)
	{
		ZoneOfInterest = b;
	};

	inline void SetDONumber(const unsigned long& n)
	{
		ulDONumber = n;
	}

	inline double GetEnergyPotential() const
	{
		return dEnergyPotential;
	};

	inline double GetEnergyTranslation() const
	{
		return dEnergyTranslation;
	};

	inline double GetEnergyRotation() const
	{
		return dEnergyRotation;
	};

	inline njr::Vector3d GetMomentumAvg() const
	{
		return vMomentumAvg;
	};

	inline njr::Vector3d GetAngularMomentumAvg() const
	{
		return vAngularMomentumAvg;
	};

	inline double GetMomentumNorm() const
	{
		return dMomentumNorm;
	};

	inline double GetAngularMomentumNorm() const
	{
		return dAngularMomentumNorm;
	};

	inline double GetVelocityMax() const
	{
		return dVelocityMax;
	};

	inline double GetVelocityMin() const
	{
		return dVelocityMin;
	};

	inline double GetAngularVelocityMax() const
	{
		return dAngularVelocityMax;
	};

	inline double GetAngularVelocityMin() const
	{
		return dAngularVelocityMin;
	};

	inline void SetEnergyPotential(double& e)
	{
		dEnergyPotential = e;
	};

	inline void SetEnergyTranslation(double& e)
	{
		dEnergyTranslation = e;
	};

	inline void SetEnergyRotation(double& e)
	{
		dEnergyRotation = e;
	};

	inline void SetMomentumAvg(const njr::Vector3d& v)
	{
		vMomentumAvg = v;
	};

	inline void SetAngularMomentumAvg(const njr::Vector3d& v)
	{
		vAngularMomentumAvg = v;
	};

	inline void SetMomentumNorm(double& d)
	{
		dMomentumNorm = d;
	};

	inline void SetAngularMomentumNorm(double& d)
	{
		dAngularMomentumNorm = d;
	};

	inline void SetVelocityMax(const double& d)
	{
		dVelocityMax = d;
	};

	inline void SetVelocityMin(const double& d)
	{
		dVelocityMin = d;
	};

	inline void SetAngularVelocityMax(const double& d)
	{
		dAngularVelocityMax = d;
	};

	inline void SetAngularVelocityMin(const double& d)
	{
		dAngularVelocityMin = d;
	};

	//binary output
	std::ofstream& operator >> (std::ofstream& idof) const;

	//binary input
	std::ifstream& operator << (std::ifstream& idof);

private:

	// Starting time of simulate system
	double dTimeStart;

	// Stopping time of simulate system
	double dTimeStop;

	// Time interval of each simulate iteration
	double dTimeInterval;

	// The current time of current process
	double dTimeCurrent;

	// The number of SimConstants (vedo::uSizeOfConstant + ulMaxIDofDO)
	unsigned uSizeOfSimConstant;

	// Max serial number of Discrete Objects;
	unsigned long ulMaxIDofDO;   // Aries: I have not finished this part

	// The number of Discrete Objects;
	unsigned long ulDONumber;

	// The number of Interactions;
	unsigned long ulIactNumber;   // Aries: I have not finished this part

	// The global field acceleration
	njr::Vector3d vFieldAcceleration;

	Boundary      ZoneOfInterest;               // Zone of Interest (ZOI)
	Boundary      PeriodicBoundaryConditions;   // Periodic Boundary Condition (PBC)

	double        dEnergyPotential;             // Potential energy
	double        dEnergyTranslation;           // Kinetic energy (Total, Translation part)
	double        dEnergyRotation;              // Kinetic energy (Total, Rotation part)

	njr::Vector3d vMomentumAvg;                 // Average momentum
	njr::Vector3d vAngularMomentumAvg;          // Average angular momentum

	double        dMomentumNorm;                // Norm of momentum (Total)
	double        dAngularMomentumNorm;         // Norm of angular momentum (Total)

	double        dVelocityMin;                 // Minimal velocity
	double        dVelocityMax;                 // Maximal velocity

	double        dAngularVelocityMin;          // Minimal angular velocity
	double        dAngularVelocityMax;          // Maximal angular velocity
};

};   // namespace vedo

#endif // _SYSTEM_PARAMETER_H