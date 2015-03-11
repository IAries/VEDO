// DOStatus.cpp: implementation of the DOStatus class.

#include <vedo/njr/interfaces/Utility.h>
#include <vedo/framework/interfaces/DOStatus.h>
#include <cstring>

namespace vedo
{

DOStatus::DOStatus():
    ulID(0), vPosition(), vVelocity(), vOrientationX(), vOrientationZ(), vAngularVelocity(),
    vImpact(), vAngularImpact(), dGranularTemperatureV(0.0), dGranularTemperatureAV(0.0), beMonitored(true)
{
//	for(vedo_uint_t u=0; u<4*uNumUDDDOStatus; u++)
//		dUDV[u] = 0.0;
}

DOStatus::DOStatus
	(const std::string& doname, const njr::Vector3d& position, const njr::Vector3d& velocity,
	 const njr::Vector3d& orientationX, const njr::Vector3d& orientationZ,
	 const njr::Vector3d& angularvelocity, const njr::Vector3d& impact, const njr::Vector3d& angularimpact): ulID(0)
{
	sDOName	         = doname;
	vPosition        = position;
	vVelocity        = velocity;
	vOrientationX    = orientationX;
	vOrientationZ    = orientationZ;
	vAngularVelocity = angularvelocity;
	vImpact          = impact;
	vAngularImpact   = angularimpact;
	beMonitored      = false;
}

DOStatus::DOStatus(const DOStatus& dos)
{
	*this = dos;
}

const DOStatus& DOStatus::operator = (const DOStatus& dos)
{
	sDOName                = dos.sDOName;
	ulID                   = dos.ulID;
	vPosition              = dos.vPosition;
	vVelocity              = dos.vVelocity;
	vOrientationX          = dos.vOrientationX;
	vOrientationZ          = dos.vOrientationZ;
	vAngularVelocity       = dos.vAngularVelocity;
	vImpact                = dos.vImpact;
	vAngularImpact         = dos.vAngularImpact;
	beMonitored            = dos.beMonitored;
	dGranularTemperatureV  = dos.dGranularTemperatureV;
	dGranularTemperatureAV = dos.dGranularTemperatureAV;
//	memcpy(dUDV, dos.dUDV, 4*uNumUDDDOStatus*sizeof(vedo_float_t));
	return *this;
}

DOStatus::DOStatus(const std::string& doname)
{
	sDOName	= doname;
}

DOStatus::~DOStatus()
{
}

DOStatus::DOStatus(std::ifstream& idof)
{
	*this << idof;
}

bool DOStatus::operator () (const DOStatus* p) const
{
	return (sDOName == (p->sDOName));
}

void DOStatus::SetOrientation(const njr::Vector3d& OrientationX, const njr::Vector3d& OrientationZ)
{
	vOrientationX = OrientationX;
	vOrientationZ = OrientationZ;
}

/*
void DOStatus::SetUserDefinedValue(vedo_uint_t u, vedo_float_t d)
{
	if (u < uNumUDDDOStatus)
	{
		dUDV[u+3*uNumUDDDOStatus] = d;
	}
	else
	{
		std::cerr << "Error!! Code: DOStatus::SetUserDefinedValue(vedo_uint_t, vedo_float_t)" << std::endl;
		exit(-1);
	}
}

vedo_float_t DOStatus::GetUserDefinedValue(vedo_uint_t u) const
{
	if (u < uNumUDDDOStatus)
	{
		return dUDV[u+3*uNumUDDDOStatus];
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOStatus::GetUserDefinedValue(vedo_uint_t)"
			<< std::endl;
		exit(-1);
	}
}

void DOStatus::AddAccumulativeUserDefinedValue(vedo_uint_t u, vedo_float_t d)
{
	if (u < uNumUDDDOStatus)
	{
		dUDV[u                ] += d;
		dUDV[u+uNumUDDDOStatus] += d;
	}
	else
	{
		std::cerr << "Error!! Code: DOStatus::AddAccumulativeUserDefinedValue(vedo_uint_t, vedo_float_t)" << std::endl;
		exit(-1);
	}
}

const vedo_float_t* DOStatus::RetrieveUserDefinedValue()
{
	memcpy
		(&dUDV[2*uNumUDDDOStatus]      ,
		 &dUDV[uNumUDDDOStatus]        ,
		 uNumUDDDOStatus*sizeof(vedo_float_t) );

	for(vedo_uint_t u=0; u<uNumUDDDOStatus; u++)
		dUDV[u+uNumUDDDOStatus] = 0.0;

	return &dUDV[2*uNumUDDDOStatus];
}

const vedo_float_t* DOStatus::RetrieveAllUserDefinedValue() const
{
	return &dUDV[0];
}

void DOStatus::SetAllUserDefinedValue(const vedo_float_t* dp)
{
	for(vedo_uint_t u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = *(dp+u);
}

void DOStatus::CleanAllUserDefinedValue()
{
	for(vedo_uint_t u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
}

void DOStatus::CleanUserDefinedValue()
{
	for(vedo_uint_t u=0; u<uNumUDDDOStatus; u++)
		dUDV[u+3*uNumUDDDOStatus] = 0.0;
}

void DOStatus::CleanAccumulativeUserDefinedValue()
{
	for(vedo_uint_t u=0; u<3*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
}

void DOStatus::Clean()
{
	for(vedo_uint_t u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
}
*/

}   // namespace vedo



std::ofstream& vedo::DOStatus::operator >> (std::ofstream& idof) const
{
	njr::WriteString(sDOName, idof);
	idof.write((char*) &ulID            , sizeof(vedo::vedo_uint_t));
	idof.write((char*) &vPosition       , sizeof(njr::Vector3d));
	idof.write((char*) &vVelocity       , sizeof(njr::Vector3d));
	idof.write((char*) &vOrientationX   , sizeof(njr::Vector3d));
	idof.write((char*) &vOrientationZ   , sizeof(njr::Vector3d));
	idof.write((char*) &vAngularVelocity, sizeof(njr::Vector3d));
	idof.write((char*) &vImpact         , sizeof(njr::Vector3d));
	idof.write((char*) &vAngularImpact  , sizeof(njr::Vector3d));
//	idof.write((char*) &dUDV[0]         , 4*uNumUDDDOStatus*sizeof(vedo_float_t));
	return idof;
}

std::ifstream& vedo::DOStatus::operator << (std::ifstream& idof)
{
	njr::ReadString(sDOName, idof);
	idof.read((char*) &ulID            , sizeof(vedo::vedo_uint_t));
	idof.read((char*) &vPosition       , sizeof(njr::Vector3d));
	idof.read((char*) &vVelocity       , sizeof(njr::Vector3d));
	idof.read((char*) &vOrientationX   , sizeof(njr::Vector3d));
	idof.read((char*) &vOrientationZ   , sizeof(njr::Vector3d));
	idof.read((char*) &vAngularVelocity, sizeof(njr::Vector3d));
	idof.read((char*) &vImpact         , sizeof(njr::Vector3d));
	idof.read((char*) &vAngularImpact  , sizeof(njr::Vector3d));
//	idof.read((char*) &dUDV[0]         , 4*uNumUDDDOStatus*sizeof(vedo_float_t));
	return idof;
}
