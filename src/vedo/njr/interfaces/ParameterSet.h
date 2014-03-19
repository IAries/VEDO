#ifndef _NJR_PARAMETER_SET_H
#define _NJR_PARAMETER_SET_H

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <iostream>

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
		bool bHasDeleted = false;

		if(!_mttParameter.empty())
		{
			typename std::map<TNAME, TVALUE>::iterator _mIterator = _mttParameter.begin();
			bool bContinued = true;
			while(bContinued)
			{
				if(_mIterator->first == tName)
				{
					_mttParameter.erase(_mIterator);
					bContinued  = false;
					bHasDeleted = true;
				}
				else
				{
					_mIterator++;
					if (_mIterator == _mttParameter.end())
						bContinued = false;
				}
			}
		}

		return bHasDeleted;
	};

	const TVALUE& operator () (const TNAME& tName) const
	{
		for(typename std::map<TNAME, TVALUE>::const_iterator
			_mIterator  = _mttParameter.begin();
			_mIterator != _mttParameter.end()  ;
			_mIterator++                         )
		{
			if(_mIterator->first == tName)
				return _mIterator->second;
		}

		std::cerr
			<< "The Keyword "
			<< tName
			<< " doesn't exist!!"
			<< std::endl;
		std::exit(-1);
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
			_mIterator++                         )
		{
			if(pstTarget.Defined(_mIterator->first))
				if (abs(_mIterator->second) < abs(pstTarget(_mIterator->first)))
					return false;
		}

		return true;
	};

	bool operator < (const ParameterSet<TNAME, TVALUE>& pstTarget) const
	{
		for(typename std::map<TNAME, TVALUE>::const_iterator
			_mIterator  = _mttParameter.begin();
			_mIterator != _mttParameter.end()  ;
			_mIterator++                         )
		{
			if(pstTarget.Defined(_mIterator->first))
				if (abs(_mIterator->second) > abs(pstTarget(_mIterator->first)))
					return false;
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
				_mIterator++;

			return _mIterator->first;
		}
		else
		{
			return TNAME();
		}
	};

	bool Defined(const TNAME& tName) const
	{
		for(typename std::map<TNAME, TVALUE>::const_iterator
			_mIterator  = _mttParameter.begin();
			_mIterator != _mttParameter.end()  ;
			_mIterator++                         )
		{
			if(_mIterator->first == tName)
				return true;
		}
		return false;
	};

	void Print(std::ostream& os) const
	{
		for(typename std::map<TNAME, TVALUE>::const_iterator
			_mIterator  = _mttParameter.begin();
			_mIterator != _mttParameter.end()  ;
			_mIterator++                         )
		{
			os
				<< '['
				<< _mIterator->first
				<< "]: "
				<< _mIterator->second
				<< std::endl;
		}
	};

private:

	std::map<TNAME, TVALUE> _mttParameter;
};

};   // namespace njr

template <typename TNAME, typename TVALUE>
std::ostream& operator << (std::ostream& os, const njr::ParameterSet<TNAME, TVALUE>& ps)
{
	ps.Print(os);
	return os;
};

#endif // _PARAMETER_SET_H
