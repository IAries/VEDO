#include <vedo/constants/interfaces/Constants.h>
#include <vedo/njr/interfaces/Utility.h>
#include <vedo/framework/interfaces/SystemParameter.h>

#include <stdint.h>

namespace vedo
{

SystemParameter::SystemParameter
	(const vedo_float_t& timestart, const vedo_float_t& timestop,
	 const vedo_float_t& timeinterval, const vedo_float_t& timecurrent,
	 const njr::Vector3d& fieldacceleration, const Boundary& ZOI, const Boundary& PBC):
	 ZoneOfInterest(ZOI), PeriodicBoundaryConditions(PBC),
	 ulDONumber(0), ulIactNumber(0), dEnergyPotential(0.0), dEnergyTranslation(0.0), dEnergyRotation(0.0),
	 dVelocityMax(0.0), dVelocityMin(0.0), dAngularVelocityMax(0.0), dAngularVelocityMin(0.0)
{
	//uSizeOfSimConstant = VEDO_Constants->SizeOfConstant() + 1;
	uSizeOfSimConstant = vedo::uSizeOfConstant + 1;
	dTimeStart         = timestart;
	dTimeStop          = timestop;
	dTimeCurrent       = timecurrent;
	dTimeInterval      = timeinterval;
	vFieldAcceleration = fieldacceleration;
}

SystemParameter::SystemParameter(const SystemParameter& sp)
{
	*this = sp;
}

SystemParameter::SystemParameter(std::ifstream& idof)
{
	*this << idof;
}

const SystemParameter& SystemParameter::operator = (const SystemParameter& sp)
{
	uSizeOfSimConstant         = sp.uSizeOfSimConstant;
	dTimeStart                 = sp.dTimeStart;
	dTimeStop                  = sp.dTimeStop;
	dTimeCurrent               = sp.dTimeCurrent;
	dTimeInterval              = sp.dTimeInterval;
	ulDONumber                 = sp.ulDONumber;
	ulIactNumber               = sp.ulIactNumber;
	ulMaxIDofDO                = sp.ulMaxIDofDO;
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
}

std::ofstream& SystemParameter::operator >> (std::ofstream& idof) const
{
	njr::WriteString(vedo::sReleaseDate, idof);
	idof.write((char*) &uSizeOfSimConstant, sizeof(vedo_uint_t));
	Constants* vedo_cp = Constants::Instance();
	vedo_float_t dTemp = vedo_cp->SafetyFactor();
	idof.write((char*) &dTemp, sizeof(vedo_float_t));
	idof.write((char*) &vedo::uNumUDDDOStatus, sizeof(vedo_uint_t));
	idof.write((char*) &vedo::uNumUDDImpactStatus, sizeof(vedo_uint_t));
	idof.write((char*) &ulMaxIDofDO, sizeof(vedo_uint_t));
	idof.write((char*) &dTimeStart, sizeof(vedo_float_t));
	idof.write((char*) &dTimeStop, sizeof(vedo_float_t));
	idof.write((char*) &dTimeCurrent, sizeof(vedo_float_t));
	idof.write((char*) &dTimeInterval, sizeof(vedo_float_t));
	idof.write((char*) &vFieldAcceleration, sizeof(njr::Vector3d));
	idof.write((char*) &(ZoneOfInterest.GetSwitch()), 3*sizeof(bool));
	idof.write((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.write((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(njr::Vector3d));
	idof.write((char*) &(PeriodicBoundaryConditions.GetSwitch()), 3*sizeof(bool));
	idof.write((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.write((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(njr::Vector3d));
	return idof;
}

std::ifstream& SystemParameter::operator << (std::ifstream& idof)
{
	std::string ReleaseDate;
	njr::ReadString(ReleaseDate, idof);
	vedo_float_t dTemp;
	vedo_uint_t  uTemp;
	idof.read((char*) &uSizeOfSimConstant, sizeof(vedo_uint_t));
	Constants* vedo_cp = Constants::Instance();
	idof.read((char*) &dTemp, sizeof(vedo_float_t));
	vedo_cp->SetSafetyFactor(dTemp);
	idof.read((char*) &uTemp, sizeof(vedo_uint_t));
	idof.read((char*) &uTemp, sizeof(vedo_uint_t));
	idof.read((char*) &ulMaxIDofDO, sizeof(vedo_uint_t));
	idof.read((char*) &dTimeStart, sizeof(vedo_float_t));
	idof.read((char*) &dTimeStop, sizeof(vedo_float_t));
	idof.read((char*) &dTimeCurrent, sizeof(vedo_float_t));
	idof.read((char*) &dTimeInterval, sizeof(vedo_float_t));
	idof.read((char*) &vFieldAcceleration, sizeof(njr::Vector3d));
	idof.read((char*) &(ZoneOfInterest.GetSwitch()), sizeof(bool)*3);
	idof.read((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.read((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(njr::Vector3d));
	ZoneOfInterest.Correct();
	idof.read((char*) &(PeriodicBoundaryConditions.GetSwitch()), sizeof(bool)*3);
	idof.read((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(njr::Vector3d));
	idof.read((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(njr::Vector3d));
	PeriodicBoundaryConditions.Correct();

	return idof;
}

}   // namespace vedo
