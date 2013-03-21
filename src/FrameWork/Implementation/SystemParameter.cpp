#include <NJR/Interfaces/Utility.h>
#include <FrameWork/Interfaces/SystemParameter.h>

namespace VEDO
{

SystemParameter::SystemParameter
	(const std::string& title,
	 const std::string& note,
	 const double& timestart,
	 const double& timestop,
	 const double& timeinterval,
	 const double& timecurrent,
     const unsigned long& DONumber,
	 const NJR::NJRvector3d& fieldforce,
	 const Boundary& ZOI,
	 const Boundary& PBC           ):
	 ZoneOfInterest(ZOI),
	 PeriodicBoundaryConditions(PBC),
	 dEnergyPotential(0.0),
	 dEnergyTranslation(0.0)   , dEnergyRotation(0.0)      ,
	 dVelocityMax(0.0)         , dVelocityMin(0.0)         ,
	 dAngularVelocityMax(0.0)  , dAngularVelocityMin(0.0)
{
	sPublish	   = PUBLISH;
	sTitle         = title;
	sNote          = note;
	dTimeStart     = timestart;
	dTimeStop      = timestop;
	dTimeCurrent   = timecurrent;
	dTimeInterval  = timeinterval;
	vFieldForce    = fieldforce;
	ulDONumber     = DONumber;
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
	NJR::ReadString(sPublish, idof);
	NJR::ReadString(sTitle, idof);
	NJR::ReadString(sNote, idof);
	idof.read((char*) &dTimeStart, sizeof(double));
	idof.read((char*) &dTimeStop, sizeof(double));
	idof.read((char*) &dTimeCurrent, sizeof(double));
	idof.read((char*) &dTimeInterval, sizeof(double));
	idof.read((char*) &vFieldForce, sizeof(NJR::NJRvector3d));
	idof.read((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(NJR::NJRvector3d));
	idof.read((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(NJR::NJRvector3d));
	ZoneOfInterest.Correct();
	NJR::NJRvector3d vPBC_Point(NJRDXF::ZERO);
	PeriodicBoundaryConditions.SetLowerPoint(&vPBC_Point);
	PeriodicBoundaryConditions.SetUpperPoint(&vPBC_Point);
	PeriodicBoundaryConditions.Correct();
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
	vFieldForce                = sp.vFieldForce;
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
	NJR::WriteString(sPublish, idof);
	NJR::WriteString(sTitle, idof);
	NJR::WriteString(sNote, idof);
	idof.write((char*) &dTimeStart, sizeof(double));
	idof.write((char*) &dTimeStop, sizeof(double));
	idof.write((char*) &dTimeCurrent, sizeof(double));
	idof.write((char*) &dTimeInterval, sizeof(double));
	idof.write((char*) &vFieldForce, sizeof(NJR::NJRvector3d));
	idof.write((char*) &(ZoneOfInterest.GetSwitch()), sizeof(bool)*3);
	idof.write((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(NJR::NJRvector3d));
	idof.write((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(NJR::NJRvector3d));
	idof.write((char*) &(PeriodicBoundaryConditions.GetSwitch()), sizeof(bool)*3);
	idof.write((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(NJR::NJRvector3d));
	idof.write((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(NJR::NJRvector3d));
	idof.write((char*) &ulDONumber, sizeof(unsigned long));
	return idof;
};

std::ifstream& SystemParameter::operator << (std::ifstream& idof)
{
	NJR::ReadString(sPublish, idof);
	NJR::ReadString(sTitle, idof);
	NJR::ReadString(sNote, idof);
	idof.read((char*) &dTimeStart, sizeof(double));
	idof.read((char*) &dTimeStop, sizeof(double));
	idof.read((char*) &dTimeCurrent, sizeof(double));
	idof.read((char*) &dTimeInterval, sizeof(double));
	idof.read((char*) &vFieldForce, sizeof(NJR::NJRvector3d));
	idof.read((char*) &(ZoneOfInterest.GetSwitch()), sizeof(bool)*3);
	idof.read((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(NJR::NJRvector3d));
	idof.read((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(NJR::NJRvector3d));
	ZoneOfInterest.Correct();
	idof.read((char*) &(PeriodicBoundaryConditions.GetSwitch()), sizeof(bool)*3);
	idof.read((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(NJR::NJRvector3d));
	idof.read((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(NJR::NJRvector3d));
	PeriodicBoundaryConditions.Correct();
	idof.read((char*) &ulDONumber, sizeof(unsigned long));
	return idof;
};

};   // namespace VEDO
