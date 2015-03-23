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
	 	_bParameters(psb), _fParameters(psf), _iParameters(psi)
	{
	}

EFPathDependent::EFPathDependent(ParameterSet<std::string, _float_t>& psf): _fParameters(psf)
{
}

EFPathDependent::EFPathDependent(const EFPathDependent& ef)
{
	*this = ef;
}

const EFPathDependent& EFPathDependent::operator = (const EFPathDependent& ef)
{
	_bParameters = ef._bParameters;
	_fParameters = ef._fParameters;
	_iParameters = ef._iParameters;
	return *this;
}

EFPathDependent::~EFPathDependent()
{
	_bParameters.~ParameterSet();
	_fParameters.~ParameterSet();
	_iParameters.~ParameterSet();
}

void EFPathDependent::Initialize()
{
}

}   // namespace aries
