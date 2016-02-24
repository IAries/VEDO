#include <vedo/framework/DOStatus.h>
#include <aries/utility/BinaryIO.h>
#include <cstring>

namespace vedo
{

DOStatus::DOStatus():
    ulID(0), vPosition(), vVelocity(), vOrientationX(), vOrientationZ(), vAngularVelocity(),
    vImpact(), vAngularImpact(), dGranularTemperatureV(0.0), dGranularTemperatureAV(0.0), beMonitored(true)
{
//	for(_uint_t u=0; u<4*uNumUDDDOStatus; u++)
//		dUDV[u] = 0.0;
}

DOStatus::DOStatus
	(const std::string& doname, const Vector3df& position, const Vector3df& velocity,
	 const Vector3df& orientationX, const Vector3df& orientationZ,
	 const Vector3df& angularvelocity, const Vector3df& impact, const Vector3df& angularimpact): ulID(0)
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
//	memcpy(dUDV, dos.dUDV, 4*uNumUDDDOStatus*sizeof(_float_t));
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

void DOStatus::SetOrientation(const Vector3df& OrientationX, const Vector3df& OrientationZ)
{
	vOrientationX = OrientationX;
	vOrientationZ = OrientationZ;
}

/*
void DOStatus::SetUserDefinedValue(_uint_t u, _float_t d)
{
	if (u < uNumUDDDOStatus)
	{
		dUDV[u+3*uNumUDDDOStatus] = d;
	}
	else
	{
		std::cerr << "Error!! Code: DOStatus::SetUserDefinedValue(_uint_t, _float_t)" << std::endl;
		exit(-1);
	}
}

_float_t DOStatus::GetUserDefinedValue(_uint_t u) const
{
	if (u < uNumUDDDOStatus)
	{
		return dUDV[u+3*uNumUDDDOStatus];
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOStatus::GetUserDefinedValue(_uint_t)"
			<< std::endl;
		exit(-1);
	}
}

void DOStatus::AddAccumulativeUserDefinedValue(_uint_t u, _float_t d)
{
	if (u < uNumUDDDOStatus)
	{
		dUDV[u                ] += d;
		dUDV[u+uNumUDDDOStatus] += d;
	}
	else
	{
		std::cerr << "Error!! Code: DOStatus::AddAccumulativeUserDefinedValue(_uint_t, _float_t)" << std::endl;
		exit(-1);
	}
}

const _float_t* DOStatus::RetrieveUserDefinedValue()
{
	memcpy
		(&dUDV[2*uNumUDDDOStatus]      ,
		 &dUDV[uNumUDDDOStatus]        ,
		 uNumUDDDOStatus*sizeof(_float_t) );

	for(_uint_t u=0; u<uNumUDDDOStatus; u++)
		dUDV[u+uNumUDDDOStatus] = 0.0;

	return &dUDV[2*uNumUDDDOStatus];
}

const _float_t* DOStatus::RetrieveAllUserDefinedValue() const
{
	return &dUDV[0];
}

void DOStatus::SetAllUserDefinedValue(const _float_t* dp)
{
	for(_uint_t u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = *(dp+u);
}

void DOStatus::CleanAllUserDefinedValue()
{
	for(_uint_t u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
}

void DOStatus::CleanUserDefinedValue()
{
	for(_uint_t u=0; u<uNumUDDDOStatus; u++)
		dUDV[u+3*uNumUDDDOStatus] = 0.0;
}

void DOStatus::CleanAccumulativeUserDefinedValue()
{
	for(_uint_t u=0; u<3*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
}

void DOStatus::Clean()
{
	for(_uint_t u=0; u<4*uNumUDDDOStatus; u++)
		dUDV[u] = 0.0;
}
*/

}   // namespace vedo



std::ofstream& vedo::DOStatus::operator >> (std::ofstream& idof) const
{
	aries::WriteString(sDOName, idof);
	idof.write((char*) &ulID            , sizeof(_uint_t));
	idof.write((char*) &vPosition       , sizeof(Vector3df));
	idof.write((char*) &vVelocity       , sizeof(Vector3df));
	idof.write((char*) &vOrientationX   , sizeof(Vector3df));
	idof.write((char*) &vOrientationZ   , sizeof(Vector3df));
	idof.write((char*) &vAngularVelocity, sizeof(Vector3df));
	idof.write((char*) &vImpact         , sizeof(Vector3df));
	idof.write((char*) &vAngularImpact  , sizeof(Vector3df));
//	idof.write((char*) &dUDV[0]         , 4*uNumUDDDOStatus*sizeof(_float_t));
	return idof;
}

std::ifstream& vedo::DOStatus::operator << (std::ifstream& idof)
{
	aries::ReadString(sDOName, idof);
	idof.read((char*) &ulID            , sizeof(_uint_t));
	idof.read((char*) &vPosition       , sizeof(Vector3df));
	idof.read((char*) &vVelocity       , sizeof(Vector3df));
	idof.read((char*) &vOrientationX   , sizeof(Vector3df));
	idof.read((char*) &vOrientationZ   , sizeof(Vector3df));
	idof.read((char*) &vAngularVelocity, sizeof(Vector3df));
	idof.read((char*) &vImpact         , sizeof(Vector3df));
	idof.read((char*) &vAngularImpact  , sizeof(Vector3df));
//	idof.read((char*) &dUDV[0]         , 4*uNumUDDDOStatus*sizeof(_float_t));
	return idof;
}
