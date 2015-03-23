#ifndef _ARIES_EXPLICIT_FUNCTION_PATH_DEPENDENT_H
#define _ARIES_EXPLICIT_FUNCTION_PATH_DEPENDENT_H

#include <aries/utility/ExplicitFunction.h>
#include <aries/utility/ParameterSet.h>
#include <cstdlib>
#include <iostream>
#include <string>

namespace aries
{

class EFPathDependent: public ExplicitFunction<_float_t, _float_t>
{
public:

	EFPathDependent();

	EFPathDependent
		(ParameterSet<std::string, bool    >&,
		 ParameterSet<std::string, _float_t>&,
		 ParameterSet<std::string, _int_t  >& );

	EFPathDependent(ParameterSet<std::string, _float_t>&);

	EFPathDependent(const EFPathDependent&);

	const EFPathDependent& operator = (const EFPathDependent&);

	~EFPathDependent();

	inline void SetBoolParameterSet(const ParameterSet<std::string, bool>& psb)
	{
		_bParameters = psb;
	}

	inline bool GetBoolParameter(const std::string& pn)
	{
		return _bParameters(pn);
	}

	inline _uint_t GetBoolParameterSize()
	{
		return _bParameters.size();
	}

	inline void SetFloatParameterSet(const ParameterSet<std::string, _float_t>& psf)
	{
		_fParameters = psf;
	}

	inline _float_t GetFloatParameter(const std::string& pn)
	{
		return _fParameters(pn);
	}

	inline _uint_t GetFloarParameterSize()
	{
		return _fParameters.size();
	}

	inline void SetIntParameterSet(const ParameterSet<std::string, _int_t>& psi)
	{
		_iParameters = psi;
	}

	inline _int_t GetIntParameter(const std::string& pn)
	{
		return _iParameters(pn);
	}

	inline _uint_t GetIntParameterSize()
	{
		return _iParameters.size();
	}

	virtual inline const std::string GetName() const
	{
		return "EFPathDependent";
	}

	virtual inline const bool PathDependent() const
	{
		return true;
	}

	virtual void Initialize();

	virtual _float_t operator () (const _float_t&) = 0;

protected:

	ParameterSet<std::string, bool    > _bParameters;

	ParameterSet<std::string, _float_t> _fParameters;

	ParameterSet<std::string, _int_t  > _iParameters;
};

}   // namespace aries

#endif   // _ARIES_EXPLICIT_FUNCTION_PATH_DEPENDENT_H
