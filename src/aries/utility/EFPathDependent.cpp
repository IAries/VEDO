#include <aries/utility/EFPathDependent.h>
#include <cstdlib>
#include <iostream>
#include <string>

namespace aries
{

EFPathDependent::EFPathDependent()
{
}

EFPathDependent::EFPathDependent
	(ParameterSet<std::string, bool    >& psb,
	 ParameterSet<std::string, _float_t>& psf,
	 ParameterSet<std::string, _int_t  >& psi ):
	 	_BoolParameters(psb), _FloatParameters(psf), _IntParameters(psi)
	{
	}

EFPathDependent::EFPathDependent(ParameterSet<std::string, _float_t>& psf): _FloatParameters(psf)
{
}

EFPathDependent::EFPathDependent(const EFPathDependent& ef)
{
	*this = ef;
}

const EFPathDependent& EFPathDependent::operator = (const EFPathDependent& ef)
{
	_BoolParameters  = ef._BoolParameters;
	_FloatParameters = ef._FloatParameters;
	_IntParameters   = ef._IntParameters;
	return *this;
}

EFPathDependent::~EFPathDependent()
{
	_BoolParameters.~ParameterSet();
	_FloatParameters.~ParameterSet();
	_IntParameters.~ParameterSet();
}

void EFPathDependent::Initialize()
{
}

}   // namespace aries
