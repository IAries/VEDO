#include <aries/utility/BinaryIO.h>
#include <aries/utility/Constants.h>
#include <vedo/framework/SystemParameter.h>

#include <stdint.h>

namespace vedo
{

SystemParameter::SystemParameter
	(const _float_t& timestart, const _float_t& timestop,
	 const _float_t& timeinterval, const _float_t& timecurrent,
	 const Vector3df& fieldacceleration, const Boundary& ZOI, const Boundary& PBC):
	 ZoneOfInterest(ZOI), PeriodicBoundaryConditions(PBC),
	 ulDONumber(0), ulIactNumber(0), dEnergyPotential(0.0), dEnergyTranslation(0.0), dEnergyRotation(0.0),
	 dVelocityMax(0.0), dVelocityMin(0.0), dAngularVelocityMax(0.0), dAngularVelocityMin(0.0)
{
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
	aries::WriteString(aries::information::_ReleaseDate, idof);

	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUnsigned = 3;   // aries_cp->NumUDDDOStatus(), aries_cp->NumUDDImpactStatus(), ulMaxIDofDO
	idof.write((char*) &uNumUnsigned, sizeof(_uint_t));
	_uint_t uTemp = aries_cp->NumUDDDOStatus();
	idof.write((char*) &uTemp, sizeof(_uint_t));
	uTemp = aries_cp->NumUDDImpactStatus();
	idof.write((char*) &uTemp, sizeof(_uint_t));
	idof.write((char*) &ulMaxIDofDO, sizeof(_uint_t));

	_uint_t uNumFloat = 1;   // aries_cp->SafetyFactor()
	idof.write((char*) &uNumFloat, sizeof(_uint_t));
	_float_t dTemp = aries_cp->SafetyFactor();
	idof.write((char*) &dTemp, sizeof(_float_t));

	idof.write((char*) &dTimeStart, sizeof(_float_t));
	idof.write((char*) &dTimeStop, sizeof(_float_t));
	idof.write((char*) &dTimeCurrent, sizeof(_float_t));
	idof.write((char*) &dTimeInterval, sizeof(_float_t));
	idof.write((char*) &vFieldAcceleration, sizeof(Vector3df));
	idof.write((char*) &(ZoneOfInterest.GetSwitch()), 3*sizeof(bool));
	idof.write((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(Vector3df));
	idof.write((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(Vector3df));
	idof.write((char*) &(PeriodicBoundaryConditions.GetSwitch()), 3*sizeof(bool));
	idof.write((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(Vector3df));
	idof.write((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(Vector3df));
	return idof;
}

std::ifstream& SystemParameter::operator << (std::ifstream& idof)
{
	std::string ReleaseDate;
	aries::ReadString(ReleaseDate, idof);

	_uint_t  uTemp;
	_float_t dTemp;
	aries::Constants* aries_cp = aries::Constants::Instance();

	_uint_t uNumUnsigned = 3;   // aries_cp->NumUDDDOStatus(), aries_cp->NumUDDImpactStatus(), ulMaxIDofDO
	idof.read((char*) &uTemp, sizeof(_uint_t));
	if (uTemp != uNumUnsigned)
	{
		std::cout
			<< "Caution!! Code: std::ifstream& SystemParameter::operator << (std::ifstream&)" << std::endl
			<< "          Condition: Size of integer data is wrong!!"                         << std::endl;
		std::exit(-1);
	}
	idof.read((char*) &uTemp, sizeof(_uint_t));
	idof.read((char*) &uTemp, sizeof(_uint_t));
	idof.read((char*) &ulMaxIDofDO, sizeof(_uint_t));

	_uint_t uNumFloat = 1;   // aries_cp->SafetyFactor()
	idof.read((char*) &uTemp, sizeof(_uint_t));
	if (uTemp != uNumFloat)
	{
		std::cout
			<< "Caution!! Code: std::ifstream& SystemParameter::operator << (std::ifstream&)" << std::endl
			<< "          Condition: Size of float data is wrong!!"                           << std::endl;
		std::exit(-1);
	}
	idof.read((char*) &dTemp, sizeof(_float_t));
	aries_cp->SetSafetyFactor(dTemp);

	idof.read((char*) &dTimeStart, sizeof(_float_t));
	idof.read((char*) &dTimeStop, sizeof(_float_t));
	idof.read((char*) &dTimeCurrent, sizeof(_float_t));
	idof.read((char*) &dTimeInterval, sizeof(_float_t));
	idof.read((char*) &vFieldAcceleration, sizeof(Vector3df));
	idof.read((char*) &(ZoneOfInterest.GetSwitch()), sizeof(bool)*3);
	idof.read((char*) &(ZoneOfInterest.GetLowerPoint()), sizeof(Vector3df));
	idof.read((char*) &(ZoneOfInterest.GetUpperPoint()), sizeof(Vector3df));
	ZoneOfInterest.Correct();
	idof.read((char*) &(PeriodicBoundaryConditions.GetSwitch()), sizeof(bool)*3);
	idof.read((char*) &(PeriodicBoundaryConditions.GetLowerPoint()), sizeof(Vector3df));
	idof.read((char*) &(PeriodicBoundaryConditions.GetUpperPoint()), sizeof(Vector3df));
	PeriodicBoundaryConditions.Correct();

	return idof;
}

}   // namespace vedo
