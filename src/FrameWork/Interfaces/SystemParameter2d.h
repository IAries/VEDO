#ifndef _SYSTEM_PARAMETER_2D_H
#define _SYSTEM_PARAMETER_2D_H

#include <NJR/Interfaces/Vector2d.h>
#include <Framework/Interfaces/Boundary2d.h>
#include <string>
#include <fstream>
#define  PUBLISH "2010-08-27"

namespace vedo
{

// Data Type Class
class SystemParameter2d
{

public:

	SystemParameter2d
		(const std::string& Title,
		const std::string& Note,
		const double& TimeStart,
		const double& TimeStop,
		const double& TimeInterval,
		const double& TimeCurrent,
		const unsigned long int& DONumber,
		const njr::Vector2d& FieldAcceleration,
		const Boundary2d& ZOI,
		const Boundary2d& PBC             );

	SystemParameter2d(const SystemParameter2d& sp);

	SystemParameter2d(std::ifstream& idof);

	const SystemParameter2d& operator = (const SystemParameter2d &sp);

	SystemParameter2d(std::ifstream& idof, unsigned int version);

	// Functions for getting private variables
	inline std::string GetPublish() const
	{
		return sPublish;
	};

	inline std::string GetTitle() const
	{
		return sTitle;
	};

	inline std::string GetNote() const
	{
		return sNote;
	};

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

	inline njr::Vector2d GetFieldAcceleration() const
	{
		return vFieldAcceleration;
	};

	inline const Boundary2d& GetZoneOfInterest() const
	{
		return ZoneOfInterest;
	};

	inline const Boundary2d& GetPeriodicBoundaryConditions() const
	{
		return PeriodicBoundaryConditions;
	};

	//Simulate Clock Control function
	inline void SetTime(const double& dCurrent, const double& dInterval)
	{
		dTimeCurrent  = dCurrent;
		dTimeInterval = dInterval;
	};

	inline void SetTime
		(const double& dStart,
		const double& dStop,
		const double& dInterval,
		const double& dCurrent)
	{
		dTimeStart    = dStart;
		dTimeStop     = dStop;
		dTimeCurrent  = dCurrent;
		dTimeInterval = dInterval;
	};

	inline void SetNote (const std::string& Note)
	{
		sNote = Note;
	};

	inline void NextStep()
	{
		dTimeCurrent += dTimeInterval;
	};

	inline void SetFieldAcceleration(const njr::Vector2d& FieldAcceleration)
	{
		vFieldAcceleration = FieldAcceleration;
	};

	inline void SetBoundary(const Boundary2d& b)
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

	inline njr::Vector2d GetMomentumAvg() const
	{
		return vMomentumAvg;
	};

	inline njr::Vector2d GetAngularMomentumAvg() const
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

	inline void SetMomentumAvg(const njr::Vector2d& v)
	{
		vMomentumAvg = v;
	};

	inline void SetAngularMomentumAvg(const njr::Vector2d& v)
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

	// The last update of IDOWorld format
	std::string sPublish;

	// Title of simulate Problem
	std::string sTitle;

	// Comment of simulate Problem
	std::string sNote;

	// Starting time of simulate system
	double dTimeStart;

	// Stopping time of simulate system
	double dTimeStop;

	// The current time of current process
	double dTimeCurrent;

	// Time interval of each simulate iteration
	double dTimeInterval;

	// The number of Discrete Objects;
	unsigned long ulDONumber;

	// The global field acceleration
	njr::Vector2d vFieldAcceleration;

	Boundary2d  ZoneOfInterest;               // Zone of Interest (ZOI)
	Boundary2d  PeriodicBoundaryConditions;   // Periodic Boundary Condition (PBC)

	double      dEnergyPotential;             // Potential energy
	double      dEnergyTranslation;           // Kinetic energy (Total, Translation part)
	double      dEnergyRotation;              // Kinetic energy (Total, Rotation part)

	njr::Vector2d vMomentumAvg;                 // Average momentum
	njr::Vector2d vAngularMomentumAvg;          // Average angular momentum

	double      dMomentumNorm;                // Norm of momentum (Total)
	double      dAngularMomentumNorm;         // Norm of angular momentum (Total)

	double      dVelocityMin;                 // Minimal velocity
	double      dVelocityMax;                 // Maximal velocity

	double      dAngularVelocityMin;          // Minimal angular velocity
	double      dAngularVelocityMax;          // Maximal angular velocity
};

};   // namespace vedo

#endif // _SYSTEM_PARAMETER_2D_H
