#ifndef _NJR_EXPLICIT_FUNCTION_H
#define _NJR_EXPLICIT_FUNCTION_H

#include <vedo/njr/interfaces/ParameterSet.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace njr
{

template <typename TX, typename TY>
class ExplicitFunction
{
public:

	ExplicitFunction()
	{
	};

	ExplicitFunction(const ExplicitFunction& ef)
	{
		*this = ef;
	};

	const ExplicitFunction& operator = (const ExplicitFunction& ef)
	{
		return *this;
	};

	~ExplicitFunction()
	{
	};

	virtual inline const std::string GetName() const
	{
		return "ExplicitFunction";
	};

	virtual inline const bool PathDependent() const = 0;

	virtual void Initialize()
	{
	};

	virtual TY operator () (const TX& tX) const = 0;
};

};   // namespace njr

#endif   // _NJR_EXPLICIT_FUNCTION_H

