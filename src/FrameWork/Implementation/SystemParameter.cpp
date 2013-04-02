#include <NJR/Interfaces/Utility.h>
#include <Framework/Interfaces/Constants.h>
#include <Framework/Interfaces/SystemParameter.h>

namespace vedo
{

SystemParameter::SystemParameter
	(const std::string& title,
	 const std::string& note,
	 const double& timestart,
	 const double& timestop,
	 const double& timeinterval,
	 const double& timecurrent,
     const unsigned long& DONumber,
	 const njr::Vector3d& fieldacceleration,
	 const Boundary& ZOI,
	 const Boundary& PBC           ):
	 ZoneOfInterest(ZOI),
	 PeriodicBoundaryConditions(PBC),
	 dEnergyPotential(0.0),
	 dEnergyTranslation(0.0)   , dEnergyRotation(0.0)      ,
	 dVelocityMax(0.0)         , dVelocityMin(0.0)         ,
	 dAngularVelocityMax(0.0)  , dAngularVelocityMin(0.0)
{
	sPublish	       = PUBLISH;
	sTitle             = title;
	sNote              = note;
	dTimeStart         = timestart;
	dTimeStop          = timestop;
	dTimeCurrent       = timecurrent;
	dTimeInterval      = timeinterval;
	vFieldAcceleration = fieldacceleration;
	ulDONumber         = DONumber;
};

SystemParameter::SystemParameter(const SystemParameter& sp)
{
	*this = sp;
};

SystemParameter::SystemParameter(std::ifstream& idof)
{
	*this << idof;
};

SystemParameter::SystemParameter(std::ifstream& idof, unsigned int version)
{
	njr::ReadString(sPublish, idof);
	njr::ReadString(sTitle, idof);
	njr::ReadString(sNote, idof);
	idof.read((char*) &dTimeStart, sizeof(double));
	idof.read((char*) &dTimeStop, sizeof(double));
	idof.read((char*) &dTimeCurrent, sizeof(double));
	idof.read((char*) &dTimeInterval, sizeof(double));
	idof.read((char*) &vFieldAcceleration, sizeof(njr::Vector3d));

    if(version >= 2011)
        idof.read((char*) &(ZoneOfInterest.GetSwitch()), sizeof(bool)*3);

	idof.read((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.read((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(njr::Vector3d));
	ZoneOfInterest.Correct();

    if(version >= 2011)
        idof.read((char*) &(PeriodicBoundaryConditions.GetSwitch()), sizeof(bool)*3);

	idof.read((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.read((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(njr::Vector3d));
	idof.read((char*) &ulDONumber, sizeof(unsigned long));
};

const SystemParameter& SystemParameter::operator = (const SystemParameter& sp)
{
	sPublish                   = sp.sPublish;
	sTitle                     = sp.sTitle;
	sNote                      = sp.sNote;
	dTimeStart                 = sp.dTimeStart;
	dTimeStop                  = sp.dTimeStop;
	dTimeCurrent               = sp.dTimeCurrent;
	dTimeInterval              = sp.dTimeInterval;
	ulDONumber                 = sp.ulDONumber;
	vFieldAcceleration         = sp.vFieldAcceleration;
	ZoneOfInterest             = sp.ZoneOfInterest;
	PeriodicBoundaryConditions = sp.PeriodicBoundaryConditions;
	dEnergyPotential           = sp.dEnergyPotential;
	dEnergyTranslation         = sp.dEnergyTranslation;
	dEnergyRotation            = sp.dEnergyRotation;
	vMomentumAvg               = sp.vMomentumAvg;
	vAngularMomentumAvg        = sp.vAngularMomentumAvg;
	dMomentumNorm              = sp.dMomentumNorm;
	dAngularMomentumNorm       = sp.dAngularMomentumNorm;
	dVelocityMin               = sp.dVelocityMin;
	dVelocityMax               = sp.dVelocityMax;
	dAngularVelocityMin        = sp.dAngularVelocityMin;
	dAngularVelocityMax        = sp.dAngularVelocityMax;
	return *this;
};

std::ofstream& SystemParameter::operator >> (std::ofstream& idof) const
{
	njr::WriteString(sPublish, idof);
	njr::WriteString(sTitle, idof);
	njr::WriteString(sNote, idof);
	idof.write((char*) &dTimeStart, sizeof(double));
	idof.write((char*) &dTimeStop, sizeof(double));
	idof.write((char*) &dTimeCurrent, sizeof(double));
	idof.write((char*) &dTimeInterval, sizeof(double));
	idof.write((char*) &vedo::dSafetyFactor, sizeof(double));
	idof.write((char*) &vedo::uNumUDDDOStatus, sizeof(unsigned));
	idof.write((char*) &vedo::uNumUDDImpactStatus, sizeof(unsigned));
	idof.write((char*) &vFieldAcceleration, sizeof(njr::Vector3d));
	idof.write((char*) &(ZoneOfInterest.GetSwitch()), sizeof(bool)*3);
	idof.write((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.write((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(njr::Vector3d));
	idof.write((char*) &(PeriodicBoundaryConditions.GetSwitch()), sizeof(bool)*3);
	idof.write((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.write((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(njr::Vector3d));
	idof.write((char*) &ulDONumber, sizeof(unsigned long));
	return idof;
};

std::ifstream& SystemParameter::operator << (std::ifstream& idof)
{
    double dTemp;

	njr::ReadString(sPublish, idof);
	njr::ReadString(sTitle, idof);
	njr::ReadString(sNote, idof);
	idof.read((char*) &dTimeStart, sizeof(double));
	idof.read((char*) &dTimeStop, sizeof(double));
	idof.read((char*) &dTimeCurrent, sizeof(double));
	idof.read((char*) &dTimeInterval, sizeof(double));
	idof.read((char*) &vedo::dSafetyFactor, sizeof(double));
//	idof.read((char*) &vedo::uNumUDDDOStatus, sizeof(unsigned));
	idof.read((char*) &dTemp, sizeof(unsigned));
//	idof.read((char*) &vedo::uNumUDDImpactStatus, sizeof(unsigned));
	idof.read((char*) &dTemp, sizeof(unsigned));
	idof.read((char*) &vFieldAcceleration, sizeof(njr::Vector3d));
	idof.read((char*) &(ZoneOfInterest.GetSwitch()), sizeof(bool)*3);
	idof.read((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.read((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(njr::Vector3d));
	ZoneOfInterest.Correct();
	idof.read((char*) &(PeriodicBoundaryConditions.GetSwitch()), sizeof(bool)*3);
	idof.read((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.read((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(njr::Vector3d));
	PeriodicBoundaryConditions.Correct();
	idof.read((char*) &ulDONumber, sizeof(unsigned long));

	return idof;
};

};   // namespace vedo
