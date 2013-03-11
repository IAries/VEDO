// DOStatus.cpp: implementation of the DOStatus class.

#include <FrameWork/Interfaces/DOStatus.h>
#include <NJR/Interfaces/Utility.h>

using namespace std;

void DOStatus::SetOrientation
	(const NJRvector3d& orientationX, const NJRvector3d& orientationZ)
{
	vOrientationX = orientationX;
	vOrientationZ = orientationZ;
};

DOStatus::DOStatus(const string& doname)
{
	sDOName	= doname;
};

DOStatus::DOStatus
	(const string& doname,
	 const NJRvector3d& position,
	 const NJRvector3d& velocity,
	 const NJRvector3d& orientationX,
	 const NJRvector3d& orientationZ,
	 const NJRvector3d& angularVelocity)
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

DOStatus::DOStatus(ifstream& idof)
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
	dRange                 = dos.dRange;
	beMonitored            = dos.beMonitored;
	dGranularTemperatureV  = dos.dGranularTemperatureV;
	dGranularTemperatureAV = dos.dGranularTemperatureAV;
	return *this;
};

bool DOStatus::operator () (const DOStatus* p) const
{
	return (sDOName == (p->sDOName));
};

ofstream& DOStatus::operator >> (ofstream& idof) const
{
     NJR::WriteString(sDOName, idof);
	 idof.write((char*) &vPosition       , sizeof(NJRvector3d));
	 idof.write((char*) &vVelocity       , sizeof(NJRvector3d));
     idof.write((char*) &vOrientationX   , sizeof(NJRvector3d));
     idof.write((char*) &vOrientationZ   , sizeof(NJRvector3d));
     idof.write((char*) &vAngularVelocity, sizeof(NJRvector3d));
	 return idof;
};

ifstream& DOStatus::operator << (ifstream& idof)
{
     NJR::ReadString(sDOName, idof);
	 idof.read((char *) &vPosition       , sizeof(NJRvector3d));
	 idof.read((char *) &vVelocity       , sizeof(NJRvector3d));
     idof.read((char *) &vOrientationX   , sizeof(NJRvector3d));
     idof.read((char *) &vOrientationZ   , sizeof(NJRvector3d));
     idof.read((char *) &vAngularVelocity, sizeof(NJRvector3d));
	 return idof;
};
