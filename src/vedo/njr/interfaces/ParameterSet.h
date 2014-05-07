#ifndef _NJR_PARAMETER_SET_H
#define _NJR_PARAMETER_SET_H

#include <vedo/njr/interfaces/Utility.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>

namespace njr
{

template <typename TNAME, typename TVALUE>
class ParameterSet
{
public:

	ParameterSet()
	{
	};

	ParameterSet(const ParameterSet& ps)
	{
		*this = ps;
	};

	const ParameterSet& operator = (const ParameterSet& ps)
	{
		_mttParameter.clear();
		_mttParameter.insert(ps._mttParameter.begin(), ps._mttParameter.end());
//		_mttParameter = ps._mttParameter;
		return *this;
	};

	~ParameterSet()
	{
		_mttParameter.clear();
	};

	ParameterSet(const TNAME* tpName, const TVALUE* tpValue, const unsigned uNum)
	{
		for(unsigned u=0; u<uNum; u++)
			_mttParameter[*(tpName+u)] = *(tpValue+u);
	};

	bool Erase(const TNAME& tName)
	{
		typename std::map<TNAME, TVALUE>::iterator
			it = _mttParameter.find(tName);

		if(it != _mttParameter.end())
		{
			_mttParameter.erase(it);
			return true;
		}
		else
		{
			return false;
		}
	};

	const TVALUE& operator () (const TNAME& tName) const
	{
		return _mttParameter.at(tName);
/*
		for(typename std::map<TNAME, TVALUE>::const_iterator
			_mIterator  = _mttParameter.begin();
			_mIterator != _mttParameter.end()  ;
			_mIterator++                         )
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
*/
	};

	TVALUE& operator[] (const TNAME& tName)
	{
		return _mttParameter[tName];
	};

	inline unsigned long Size() const
	{
		return _mttParameter.size();
	};

	bool operator > (const ParameterSet<TNAME, TVALUE>& pstTarget) const
	{
		for(typename std::map<TNAME, TVALUE>::const_iterator
			_mIterator  = _mttParameter.begin();
			_mIterator != _mttParameter.end()  ;
			_mIterator++                        )
		{
			if(pstTarget.Defined(_mIterator->first))
			{
				if (abs(_mIterator->second) < abs(pstTarget(_mIterator->first)))
				{
					return false;
				}
			}
		}

		return true;
	};

	bool operator < (const ParameterSet<TNAME, TVALUE>& pstTarget) const
	{
		for(typename std::map<TNAME, TVALUE>::const_iterator
			_mIterator  = _mttParameter.begin();
			_mIterator != _mttParameter.end()  ;
			_mIterator++                        )
		{
			if(pstTarget.Defined(_mIterator->first))
			{
				if (abs(_mIterator->second) > abs(pstTarget(_mIterator->first)))
				{
					return false;
				}
			}
		}

		return true;
	};

	const TNAME GetName(const unsigned& u) const
	{
		if (u < _mttParameter.size())
		{
			typename std::map<TNAME, TVALUE>::const_iterator
				_mIterator = _mttParameter.begin();

			for(unsigned uCounter=0; uCounter<u; uCounter++)
			{
				_mIterator++;
			}

			return _mIterator->first;
		}
		else
		{
			return TNAME();
		}
	};

	const std::pair<TNAME, TVALUE> GetNameAndData(const unsigned& u) const
	{
		if (u < _mttParameter.size())
		{
			typename std::map<TNAME, TVALUE>::const_iterator
				_mIterator = _mttParameter.begin();

			for(unsigned uCounter=0; uCounter<u; uCounter++)
			{
				_mIterator++;
			}

			return std::make_pair(_mIterator->first, _mIterator->second);
		}
		else
		{
			return std::make_pair(TNAME(), TVALUE());
		}
	};

	bool Defined(const TNAME& tName) const
	{
		typename std::map<TNAME, TVALUE>::const_iterator
			it = _mttParameter.find(tName);

		if(it != _mttParameter.end())
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	const std::map<TNAME, TVALUE>* GetData() const
	{
		return &_mttParameter;
	};

	template <typename TVariable>
	void CallFunctionInTVALUE(const TVariable& tv)
	{
		// Aries: We have not make the function "TVALUE::Update" as a template function
		typedef std::mem_fun1_t<void, TVALUE, const TVariable&> Func_Type;
		Func_Type func_obj(&TVALUE::Update);
		njr::binder2nd_refArg<Func_Type> binded_func(func_obj, tv);

		for(typename std::map<TNAME, TVALUE>::iterator
			_mIterator  = _mttParameter.begin();
			_mIterator != _mttParameter.end()  ;
			_mIterator++                         )
		{
			_mIterator->second.Update(tv);
		}
	};

private:

	std::map<TNAME, TVALUE> _mttParameter;
};

};   // namespace njr

template <typename TNAME, typename TVALUE>
std::ostream& operator << (std::ostream& os, const njr::ParameterSet<TNAME, TVALUE>& ps)
{
	const std::map<TNAME, TVALUE>* _Data = ps.GetData();
	for(typename std::map<TNAME, TVALUE>::const_iterator
		_mIterator  = _Data->begin();
		_mIterator != _Data->end()  ;
		_mIterator++                                    )
	{
		os
			<< "["
			<< _mIterator->first
			<< "]: "
			<< _mIterator->second
			<< std::endl;
	}

	return os;
};

#endif // _PARAMETER_SET_H
