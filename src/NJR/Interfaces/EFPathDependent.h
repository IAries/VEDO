#ifndef _NJR_EXPLICIT_FUNCTION_PATH_DEPENDENT_H
#define _NJR_EXPLICIT_FUNCTION_PATH_DEPENDENT_H

#include <NJR/Interfaces/Constants.h>
#include <NJR/Interfaces/ParameterSet.h>
#include <NJR/Interfaces/ExplicitFunction.h>
#include <cstdlib>
#include <iostream>
#include <string>

namespace njr
{

class EFPathDependent: public ExplicitFunction<double, double>
{
public:

	EFPathDependent();

	EFPathDependent
		(ParameterSet<std::string, bool>&  ,
		 ParameterSet<std::string, double>&,
		 ParameterSet<std::string, int>&    );

	EFPathDependent(ParameterSet<std::string, double>&);

	EFPathDependent(const EFPathDependent&);

	const EFPathDependent& operator = (const EFPathDependent&);

	~EFPathDependent();

	void SetBoolParameterSet(const ParameterSet<std::string, bool>&);

	bool GetBoolParameter(const std::string&);

	unsigned long GetBoolParameterSize();

	void SetDoubleParameterSet(const ParameterSet<std::string, double>&);

	double GetDoubleParameter(const std::string&);

	unsigned long GetDoubleParameterSize();

	void SetIntParameterSet(const ParameterSet<std::string, int>&);

	int GetIntParameter(const std::string&);

	unsigned long GetIntParameterSize();

	virtual inline const std::string GetName() const
	{
		return "EFPathDependent";
	};

	virtual inline const bool PathDependent() const
	{
		return true;
	};

	virtual void Initialize();

	virtual double operator () (const double&) = 0;

protected:

	ParameterSet<std::string, bool>   _BoolParameters;

	ParameterSet<std::string, double> _DoubleParameters;

	ParameterSet<std::string, int>    _IntParameters;
};

};   // namespace njr

#endif   // _NJR_EXPLICIT_FUNCTION_PATH_DEPENDENT_H

