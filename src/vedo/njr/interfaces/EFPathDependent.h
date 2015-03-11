#ifndef _NJR_EXPLICIT_FUNCTION_PATH_DEPENDENT_H
#define _NJR_EXPLICIT_FUNCTION_PATH_DEPENDENT_H

#include <vedo/njr/interfaces/Constants.h>
#include <vedo/njr/interfaces/ParameterSet.h>
#include <vedo/njr/interfaces/ExplicitFunction.h>
#include <vedo/constants/interfaces/Constants.h>
#include <cstdlib>
#include <iostream>
#include <string>

namespace njr
{

class EFPathDependent: public ExplicitFunction<vedo::vedo_float_t, vedo::vedo_float_t>
{
public:

	EFPathDependent();

	EFPathDependent
		(ParameterSet<std::string, bool>&,
		 ParameterSet<std::string, vedo::vedo_float_t>&,
		 ParameterSet<std::string, vedo::vedo_int_t>&   );

	EFPathDependent(ParameterSet<std::string, vedo::vedo_float_t>&);

	EFPathDependent(const EFPathDependent&);

	const EFPathDependent& operator = (const EFPathDependent&);

	~EFPathDependent();

	void SetBoolParameterSet(const ParameterSet<std::string, bool>&);

	bool GetBoolParameter(const std::string&);

	vedo::vedo_uint_t GetBoolParameterSize();

	void SetDoubleParameterSet(const ParameterSet<std::string, vedo::vedo_float_t>&);

	vedo::vedo_float_t GetDoubleParameter(const std::string&);

	vedo::vedo_uint_t GetDoubleParameterSize();

	void SetIntParameterSet(const ParameterSet<std::string, vedo::vedo_int_t>&);

	vedo::vedo_int_t GetIntParameter(const std::string&);

	vedo::vedo_uint_t GetIntParameterSize();

	virtual inline const std::string GetName() const
	{
		return "EFPathDependent";
	}

	virtual inline const bool PathDependent() const
	{
		return true;
	}

	virtual void Initialize();

	virtual vedo::vedo_float_t operator () (const vedo::vedo_float_t&) = 0;

protected:

	ParameterSet<std::string, bool> _BoolParameters;

	ParameterSet<std::string, vedo::vedo_float_t> _DoubleParameters;

	ParameterSet<std::string, vedo::vedo_int_t> _IntParameters;
};

}   // namespace njr

#endif   // _NJR_EXPLICIT_FUNCTION_PATH_DEPENDENT_H
