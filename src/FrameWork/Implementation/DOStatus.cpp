// DOStatus.cpp: implementation of the DOStatus class.

#include <FrameWork/Interfaces/DOStatus.h>
#include <NJR/Interfaces/Utility.h>
#include <cstring>

namespace VEDO
{

DOStatus::DOStatus():
    vPosition(), vVelocity(), vOrientationX(), vOrientationZ(), vAngularVelocity(),
    dGranularTemperatureV(0.0), dGranularTemperatureAV(0.0),
    beMonitored(true)
{
//	for(unsigned u=0; u<4*uNumUDDDOStatus; u++)
//		dUDV[u] = 0.0;
};

DOStatus::DOStatus
	(const std::string& doname,
	 const NJR::Vector3d& position,
	 const NJR::Vector3d& velocity,
	 const NJR::Vector3d& orientationX,
	 const NJR::Vector3d& orientationZ,
	 const NJR::Vector3d& angularVelocity)
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
//	memcpy(dUDV, dos.dUDV, 4*uNumUDDDOStatus*sizeof(double));
	return *this;
};

DOStatus::DOStatus(const std::string& doname)
{
	sDOName	= doname;
};

DOStatus::~DOStatus()
{
};

DOStatus::DOStatus(std::ifstream& idof)
{
	*this << idof;
};

bool DOStatus::operator () (const DOStatus* p) const
{
	return (sDOName == (p->sDOName));
};

void DOStatus::SetOrientation
	(const NJR::Vector3d& orientationX, const NJR::Vector3d& orientationZ)
{
	vOrientationX = orientationX;
	vOrientationZ = orientationZ;
};

/*
void DOStatus::SetUserDefinedValue(unsigned u, double d)
{
	if (u < uNumUDDDOStatus)
	{
		dUDV[u+3*uNumUDDDOStatus] = d;
	}
	else
	{
		std::cerr << "Error!! Code: DOStatus::SetUserDefinedValue(unsigned, double)" << std::endl;
		exit(-1);
	}
};

double DOStatus::GetUserDefinedValue(unsigned u) const
{
	if (u < uNumUDDDOStatus)
	{
		return dUDV[u+3*uNumUDDDOStatus];
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOStatus::GetUserDefinedValue(unsigned)"
			<< std::endl;
		exit(-1);
	}
};

void DOStatus::AddAccumulativeUserDefinedValue(unsigned u, double d)
{
	if (u < uNumUDDDOStatus)
	{
		dUDV[u                ] += d;
		dUDV[u+uNumUDDDOStatus] += d;
	}
	else
	{
		std::cerr << "Error!! Code: DOStatus::AddAccumulativeUserDefinedValue(unsigned, double)" << std::endl;
		exit(-1);
	}
};

const double* DOStatus::RetrieveUserDefinedValue()
{
	memcpy
		(&dUDV[2*uNumUDDDOStatus]      ,
		 &dUDV[uNumUDDDOStatus]        ,
		 uNumUDDDOStatus*sizeof(double) );

	for(unsigned u=0; u<uNumUDDDOStatus; u++)
		dUDV[u+uNumUDDDOStatus] = 0.0;

	return &dUDV[2*uNumUDDDOStatus];
};

const double* DOStatus::RetrieveAllUserDefinedValue() const
{
	return &dUDV[0];
};

void DOStatus::SetAllUserDefinedValue(const double* dp)
{
	for(unsigned u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = *(dp+u);
};

void DOStatus::CleanAllUserDefinedValue()
{
	for(unsigned u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
};

void DOStatus::CleanUserDefinedValue()
{
	for(unsigned u=0; u<uNumUDDDOStatus; u++)
		dUDV[u+3*uNumUDDDOStatus] = 0.0;
};

void DOStatus::CleanAccumulativeUserDefinedValue()
{
	for(unsigned u=0; u<3*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
};

void DOStatus::Clean()
{
	for(unsigned u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
};
*/

};   // namespace VEDO



std::ofstream& VEDO::DOStatus::operator >> (std::ofstream& idof) const
{
     NJR::WriteString(sDOName, idof);
	 idof.write((char*) &vPosition       , sizeof(NJR::Vector3d));
	 idof.write((char*) &vVelocity       , sizeof(NJR::Vector3d));
     idof.write((char*) &vOrientationX   , sizeof(NJR::Vector3d));
     idof.write((char*) &vOrientationZ   , sizeof(NJR::Vector3d));
     idof.write((char*) &vAngularVelocity, sizeof(NJR::Vector3d));
//     idof.write((char*) &dUDV[0]         , 4*uNumUDDDOStatus*sizeof(double));
	 return idof;
};

std::ifstream& VEDO::DOStatus::operator << (std::ifstream& idof)
{
     NJR::ReadString(sDOName, idof);
	 idof.read((char*) &vPosition       , sizeof(NJR::Vector3d));
	 idof.read((char*) &vVelocity       , sizeof(NJR::Vector3d));
     idof.read((char*) &vOrientationX   , sizeof(NJR::Vector3d));
     idof.read((char*) &vOrientationZ   , sizeof(NJR::Vector3d));
     idof.read((char*) &vAngularVelocity, sizeof(NJR::Vector3d));
//     idof.read((char*) &dUDV[0]         , 4*uNumUDDDOStatus*sizeof(double));
	 return idof;
};
