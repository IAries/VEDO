#include <vedo/njr/interfaces/EFPathDependent.h>
#include <cstdlib>
#include <iostream>
#include <string>

namespace njr
{

EFPathDependent::EFPathDependent()
{
}

EFPathDependent::EFPathDependent
	(ParameterSet<std::string, bool>&               psb,
	 ParameterSet<std::string, vedo::vedo_float_t>& psd,
	 ParameterSet<std::string, vedo::vedo_int_t>&   psi )
{
	_BoolParameters   = psb;
	_DoubleParameters = psd;
	_IntParameters    = psi;
}

EFPathDependent::EFPathDependent(ParameterSet<std::string, vedo::vedo_float_t>& psd)
{
	_DoubleParameters = psd;
}

EFPathDependent::EFPathDependent(const EFPathDependent& ef)
{
	*this = ef;
}

const EFPathDependent& EFPathDependent::operator = (const EFPathDependent& ef)
{
	_BoolParameters   = ef._BoolParameters;
	_DoubleParameters = ef._DoubleParameters;
	_IntParameters    = ef._IntParameters;
	return *this;
}

EFPathDependent::~EFPathDependent()
{
	_BoolParameters.~ParameterSet();
	_DoubleParameters.~ParameterSet();
	_IntParameters.~ParameterSet();
}

void EFPathDependent::SetBoolParameterSet(const ParameterSet<std::string, bool>& psb)
{
	_BoolParameters = psb;
}

bool EFPathDependent::GetBoolParameter(const std::string& pn)
{
	return _BoolParameters(pn);
}

vedo::vedo_uint_t EFPathDependent::GetBoolParameterSize()
{
	return _BoolParameters.Size();
}

void EFPathDependent::SetDoubleParameterSet(const ParameterSet<std::string, vedo::vedo_float_t>& psd)
{
	_DoubleParameters = psd;
}

vedo::vedo_float_t EFPathDependent::GetDoubleParameter(const std::string& s)
{
	return _DoubleParameters(s);
}

vedo::vedo_uint_t EFPathDependent::GetDoubleParameterSize()
{
	return _DoubleParameters.Size();
}

void EFPathDependent::SetIntParameterSet(const ParameterSet<std::string, vedo::vedo_int_t>& psi)
{
	_IntParameters = psi;
}

vedo::vedo_int_t EFPathDependent::GetIntParameter(const std::string& s)
{
	return _IntParameters(s);
}

vedo::vedo_uint_t EFPathDependent::GetIntParameterSize()
{
	return _IntParameters.Size();
}

void EFPathDependent::Initialize()
{
}

}   // namespace njr
