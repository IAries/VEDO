// DOStatus.cpp: implementation of the DOStatus class.

#include <Framework/Interfaces/DOStatus.h>
#include <NJR/Interfaces/Utility.h>
#include <cstring>

namespace vedo
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
	 const njr::Vector3d& position,
	 const njr::Vector3d& velocity,
	 const njr::Vector3d& orientationX,
	 const njr::Vector3d& orientationZ,
	 const njr::Vector3d& angularVelocity)
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
	(const njr::Vector3d& OrientationX, const njr::Vector3d& OrientationZ)
{
	vOrientationX = OrientationX;
	vOrientationZ = OrientationZ;
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

};   // namespace vedo



std::ofstream& vedo::DOStatus::operator >> (std::ofstream& idof) const
{
     njr::WriteString(sDOName, idof);
	 idof.write((char*) &vPosition       , sizeof(njr::Vector3d));
	 idof.write((char*) &vVelocity       , sizeof(njr::Vector3d));
     idof.write((char*) &vOrientationX   , sizeof(njr::Vector3d));
     idof.write((char*) &vOrientationZ   , sizeof(njr::Vector3d));
     idof.write((char*) &vAngularVelocity, sizeof(njr::Vector3d));
//     idof.write((char*) &dUDV[0]         , 4*uNumUDDDOStatus*sizeof(double));
	 return idof;
};

std::ifstream& vedo::DOStatus::operator << (std::ifstream& idof)
{
     njr::ReadString(sDOName, idof);
	 idof.read((char*) &vPosition       , sizeof(njr::Vector3d));
	 idof.read((char*) &vVelocity       , sizeof(njr::Vector3d));
     idof.read((char*) &vOrientationX   , sizeof(njr::Vector3d));
     idof.read((char*) &vOrientationZ   , sizeof(njr::Vector3d));
     idof.read((char*) &vAngularVelocity, sizeof(njr::Vector3d));
//     idof.read((char*) &dUDV[0]         , 4*uNumUDDDOStatus*sizeof(double));
	 return idof;
};
