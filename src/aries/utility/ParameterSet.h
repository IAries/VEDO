#ifndef _ARIES_PARAMETER_SET_H
#define _ARIES_PARAMETER_SET_H

#include <aries/utility/binder2nd_refArg.h>
#include <aries/utility/DataType.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>

#ifdef _STD_CPP_11
	#include <unordered_map>
#else
	#include <boost/unordered_map.hpp>
#endif

namespace aries
{

template <typename TNAME, typename TVALUE>
class ParameterSet
{

public:

	ParameterSet()
	{
	}

	ParameterSet(const ParameterSet& ps)
	{
		*this = ps;
	}

	const ParameterSet& operator = (const ParameterSet& ps)
	{
		//_mttParameter.clear();
		//_mttParameter.insert(ps._mttParameter.begin(), ps._mttParameter.end());
		_mttParameter = ps._mttParameter;
		return *this;
	}

	~ParameterSet()
	{
		_mttParameter.clear();
	}

	ParameterSet(const TNAME* tpName, const TVALUE* tpValue, const _uint_t uNum)
	{
		for (_uint_t u=0; u<uNum; u++)
		{
			_mttParameter[*(tpName+u)] = *(tpValue+u);
		}
	}

	bool erase(const TNAME& tName)
	{
		#ifdef _STD_CPP_11
			typename std::unordered_map<TNAME, TVALUE>::iterator it = _mttParameter.find(tName);
		#else
			typename boost::unordered_map<TNAME, TVALUE>::iterator it = _mttParameter.find(tName);
		#endif

		if(it != _mttParameter.end())
		{
			_mttParameter.erase(it);
			return true;
		}
		else
		{
			return false;
		}
	}

	inline void clear()
	{
		_mttParameter.clear();
	}

	inline void Clear()
	{
		_mttParameter.clear();
	}

	const TVALUE& operator () (const TNAME& tName) const
	{
		//return _mttParameter.at(tName);
///*
		#ifdef _STD_CPP_11
			typename std::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
		#else
			typename boost::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
		#endif
		for (_mIterator = _mttParameter.begin(); _mIterator != _mttParameter.end(); _mIterator++)
		{
			if(_mIterator->first == tName)
				return _mIterator->second;
		}

		std::cerr
			<< "The parameter "
			<< tName
			<< " doesn't exist!!"
			<< std::endl;
		std::exit(-1);
//*/
	}

	TVALUE& operator[] (const TNAME& tName)
	{
		return _mttParameter[tName];
	}

	inline _uint_t size() const
	{
		return _mttParameter.size();
	}

	inline _uint_t Size() const
	{
		return this->size();
	}

	bool operator > (const ParameterSet<TNAME, TVALUE>& pstTarget) const
	{
		#ifdef _STD_CPP_11
			typename std::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
		#else
			typename boost::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
		#endif
		for (_mIterator = _mttParameter.begin(); _mIterator != _mttParameter.end(); _mIterator++)
		{
			if (pstTarget.Defined(_mIterator->first))
			{
				if (abs(_mIterator->second) < abs(pstTarget(_mIterator->first)))
				{
					return false;
				}
			}
		}

		return true;
	}

	bool operator < (const ParameterSet<TNAME, TVALUE>& pstTarget) const
	{
		#ifdef _STD_CPP_11
			typename std::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
		#else
			typename boost::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
		#endif
		for (_mIterator = _mttParameter.begin(); _mIterator != _mttParameter.end(); _mIterator++)
		{
			if (pstTarget.Defined(_mIterator->first))
			{
				if (abs(_mIterator->second) > abs(pstTarget(_mIterator->first)))
				{
					return false;
				}
			}
		}

		return true;
	}

	const TNAME GetName(const _uint_t& u) const
	{
		if (u < _mttParameter.size())
		{
			#ifdef _STD_CPP_11
				typename std::unordered_map<TNAME, TVALUE>::const_iterator _mIterator = _mttParameter.begin();
			#else
				typename boost::unordered_map<TNAME, TVALUE>::const_iterator _mIterator = _mttParameter.begin();
			#endif
			for (_uint_t uCounter=0; uCounter<u; uCounter++)
			{
				_mIterator++;
			}

			return _mIterator->first;
		}
		else
		{
			return TNAME();
		}
	}

	const TVALUE* GetData(const _uint_t& u) const
	{
		if (u < _mttParameter.size())
		{
			#ifdef _STD_CPP_11
				typename std::unordered_map<TNAME, TVALUE>::const_iterator _mIterator = _mttParameter.begin();
			#else
				typename boost::unordered_map<TNAME, TVALUE>::const_iterator _mIterator = _mttParameter.begin();
			#endif
			for (_uint_t uCounter=0; uCounter<u; uCounter++)
			{
				_mIterator++;
			}

			return &(_mIterator->second);
		}
		else
		{
			return new TVALUE();
		}
	}

	#ifdef _STD_CPP_11
		const std::unordered_map<TNAME, TVALUE>* GetData() const
		{
			return &_mttParameter;
		}
	#else
		const boost::unordered_map<TNAME, TVALUE>* GetData() const
		{
			return &_mttParameter;
		}
	#endif


	const std::pair<TNAME, TVALUE> GetNameAndData(const _uint_t& u) const
	{
		if (u < _mttParameter.size())
		{
			#ifdef _STD_CPP_11
				typename std::unordered_map<TNAME, TVALUE>::const_iterator _mIterator = _mttParameter.begin();
			#else
				typename boost::unordered_map<TNAME, TVALUE>::const_iterator _mIterator = _mttParameter.begin();
			#endif
			for (_uint_t uCounter=0; uCounter<u; uCounter++)
			{
				_mIterator++;
			}

			return std::make_pair(_mIterator->first, _mIterator->second);
		}
		else
		{
			return std::make_pair(TNAME(), TVALUE());
		}
	}

	ParameterSet<TNAME, TVALUE> Average(const ParameterSet<TNAME, TVALUE>& pstTarget) const
	{
		ParameterSet<TNAME, TVALUE> pstNew;

		#ifdef _STD_CPP_11
			typename std::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
		#else
			typename boost::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
		#endif
		for (_mIterator  = _mttParameter.begin(); _mIterator != _mttParameter.end(); _mIterator++)
		{
			if (pstTarget.Defined(_mIterator->first))
			{
				pstNew._mttParameter[_mIterator->first] = 0.5 * (_mIterator->second + pstTarget(_mIterator->first));
			}
		}
		return pstNew;
	}


	bool Defined(const TNAME& tName) const
	{
		#ifdef _STD_CPP_11
			typename std::unordered_map<TNAME, TVALUE>::const_iterator it = _mttParameter.find(tName);
		#else
			typename boost::unordered_map<TNAME, TVALUE>::const_iterator it = _mttParameter.find(tName);
		#endif

		if(it != _mttParameter.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	///*
	template <typename TVariable>
	void CallFunctionInTVALUE(const TVariable& tv)
	{
		// Aries: We have not make the function "TVALUE::Update" as a template function
		typedef std::mem_fun1_t<void, TVALUE, const TVariable&> Func_Type;
		Func_Type func_obj(&TVALUE::Update);
		aries::binder2nd_refArg<Func_Type> binded_func(func_obj, tv);
		#ifdef _STD_CPP_11
			for (typename std::unordered_map<TNAME, TVALUE>::iterator
				_mIterator  = _mttParameter.begin(); _mIterator != _mttParameter.end(); _mIterator++)
			{
				_mIterator->second.Update(tv);
			}
		#else
			for (typename boost::unordered_map<TNAME, TVALUE>::iterator
				_mIterator  = _mttParameter.begin(); _mIterator != _mttParameter.end(); _mIterator++)
			{
				_mIterator->second.Update(tv);
			}
		#endif
	}
	//*/

private:

	#ifdef _STD_CPP_11
		std::unordered_map<TNAME, TVALUE> _mttParameter;
	#else
		boost::unordered_map<TNAME, TVALUE> _mttParameter;
	#endif
};

}   // namespace aries



template <typename TNAME, typename TVALUE>
std::ostream& operator << (std::ostream& os, const aries::ParameterSet<TNAME, TVALUE>& ps)
{
	#ifdef _STD_CPP_11
		const std::unordered_map<TNAME, TVALUE>* _Data = ps.GetData();
		typename std::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
	#else
		const boost::unordered_map<TNAME, TVALUE>* _Data = ps.GetData();
		typename boost::unordered_map<TNAME, TVALUE>::const_iterator _mIterator;
	#endif

	for (_mIterator=_Data->begin(); _mIterator!=_Data->end(); _mIterator++)
	{
		os << "[" << _mIterator->first << "]: " << _mIterator->second << std::endl;
	}
	return os;
}

#endif // _ARIES_PARAMETER_SET_H
