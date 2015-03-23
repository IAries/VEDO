#ifndef _ARIES_BINDER2ND_REFARG_H
#define _ARIES_BINDER2ND_REFARG_H

#include <algorithm>
#include <fstream>
#include <list>
#include <string>
#include <vector>

namespace aries
{

// Function object for deleting object by its point
struct Delete_ptr
{
	template<class T> T* operator() (T* p)
	{
		delete p;
		return 0;
	}
};



struct Copy_obj
{
	template<class T> T* operator() (const T* p)
	{
		return new T(*p);
	}
};

/**************************************************************************************************************************
 * std::binder2nd class has its notorious problem when the argument type is reference type. STL source codes treat the
 * argument type as _Fn2::second_argument_type&, which causes reference-to-reference problem.
 *
 * REF: Thomas Becker, "STL & Generic Programming: STL Function Objects and Their Adaptors," Dr. Dobbs, June 01, 2002.
 *
 * A solution is to use type_traits, which has been implemented in Boost or Modern C++ Design. Here we implement a
 * temporary class binder2nd_refArg to resolve this problem. (06/12/2009, Liao)
 **************************************************************************************************************************/
template<class _Fn2>
class binder2nd_refArg: public std::unary_function<typename _Fn2::first_argument_type, typename _Fn2::result_type>
{

public:

	typedef std::unary_function<typename _Fn2::first_argument_type, typename _Fn2::result_type> _Base;

	typedef typename _Base::argument_type argument_type;

	typedef typename _Base::result_type result_type;

	binder2nd_refArg(const _Fn2& _Func, typename _Fn2::second_argument_type _Right): op(_Func), value(_Right)
	{
	}

	result_type operator() (const argument_type& _Left) const
	{
		// apply functor to operands
		return (op(_Left, value));
	}

	result_type operator()(argument_type& _Left) const
	{
		// apply functor to operands
		return (op(_Left, value));
	}

protected:

	_Fn2 op;   // the functor to apply

	typename _Fn2::second_argument_type value;	// the right operand
};

}   // namespace aries

#endif   // _ARIES_BINDER2ND_REFARG_H
