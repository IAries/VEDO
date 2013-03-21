// DOStatus.cpp: implementation of the DOStatus class.

#include <FrameWork/Interfaces/DOStatus.h>
#include <NJR/Interfaces/Utility.h>

namespace VEDO
{

void DOStatus::SetOrientation
	(const NJR::NJRvector3d& orientationX, const NJR::NJRvector3d& orientationZ)
{
	vOrientationX = orientationX;
	vOrientationZ = orientationZ;
};

DOStatus::DOStatus(const std::string& doname)
{
	sDOName	= doname;
};

DOStatus::DOStatus
	(const std::string& doname,
	 const NJR::NJRvector3d& position,
	 const NJR::NJRvector3d& velocity,
	 const NJR::NJRvector3d& orientationX,
	 const NJR::NJRvector3d& orientationZ,
	 const NJR::NJRvector3d& angularVelocity)
{
	sDOName	         = doname;
	vPosition        = position;
	vVelocity        = velocity;
	vOrientationX    = orientationX;
	vOrientationZ    = orientationZ;
	vAngularVelocity = angularVelocity;
	beMonitored      = false;
};

DOStatus::DOStatus(const DOStatus& dos)
{
	*this = dos;
};

DOStatus::DOStatus(std::ifstream& idof)
{
	*this << idof;
};

const DOStatus& DOStatus::operator = (const DOStatus& dos)
{
	sDOName                = dos.sDOName;
	vPosition              = dos.vPosition;
	vVelocity              = dos.vVelocity;
	vOrientationX          = dos.vOrientationX;
	vOrientationZ          = dos.vOrientationZ;
	vAngularVelocity       = dos.vAngularVelocity;
	beMonitored            = dos.beMonitored;
	dGranularTemperatureV  = dos.dGranularTemperatureV;
	dGranularTemperatureAV = dos.dGranularTemperatureAV;
	return *this;
};

bool DOStatus::operator () (const DOStatus* p) const
{
	return (sDOName == (p->sDOName));
};

};   // namespace VEDO



std::ofstream& VEDO::DOStatus::operator >> (std::ofstream& idof) const
{
     NJR::WriteString(sDOName, idof);
	 idof.write((char*) &vPosition       , sizeof(NJR::NJRvector3d));
	 idof.write((char*) &vVelocity       , sizeof(NJR::NJRvector3d));
     idof.write((char*) &vOrientationX   , sizeof(NJR::NJRvector3d));
     idof.write((char*) &vOrientationZ   , sizeof(NJR::NJRvector3d));
     idof.write((char*) &vAngularVelocity, sizeof(NJR::NJRvector3d));
	 return idof;
};

std::ifstream& VEDO::DOStatus::operator << (std::ifstream& idof)
{
     NJR::ReadString(sDOName, idof);
	 idof.read((char *) &vPosition       , sizeof(NJR::NJRvector3d));
	 idof.read((char *) &vVelocity       , sizeof(NJR::NJRvector3d));
     idof.read((char *) &vOrientationX   , sizeof(NJR::NJRvector3d));
     idof.read((char *) &vOrientationZ   , sizeof(NJR::NJRvector3d));
     idof.read((char *) &vAngularVelocity, sizeof(NJR::NJRvector3d));
	 return idof;
};
