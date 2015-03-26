#ifndef _ARIES_VARIATION_RECORD_H
#define _ARIES_VARIATION_RECORD_H

#include <aries/utility/DataType.h>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace aries
{

class VariationRecord
{
public:

	static VariationRecord* Instance()
	{
		if (_VariationRecord == 0)
		{
			_VariationRecord = new VariationRecord;
		}
		return _VariationRecord;
	}

	VariationRecord(const VariationRecord& vr)
	{
		*this = vr;
	}

	const VariationRecord& operator = (const VariationRecord& vr)
	{
		this->Instance();
		return *this;
	}

	~VariationRecord()
	{
	}

	void AddRecord(const std::string& type, const _float_t& time, const std::vector<std::string>& records)
	{
		//_Records[type][time] = records;
		_Records[type].insert(std::make_pair(time, records));
	}

	void AddRecord(const std::string& type, const std::vector<std::string>& records)
	{
		_Records[type].insert(std::make_pair(_CurrentTime, records));
	}

	std::multimap<_float_t, std::vector<std::string> > GetRecord(const std::string& type)
	{
		return _Records[type];
	}

	const std::map<std::string, std::multimap<_float_t, std::vector<std::string> > > * GetRecord() const
	{
		return &_Records;
	}

	_uint_t size(const std::string& type)
	{
		return _Records[type].size();
	}

	inline void SetCurrentTime(const _float_t& time)
	{
		_CurrentTime = time;
	}

	inline _float_t CurrentTime() const
	{
		return _CurrentTime;
	}

protected:

	VariationRecord()
	{
	}

	static VariationRecord* _VariationRecord;

	_float_t _CurrentTime;

	std::map<std::string, std::multimap<_float_t, std::vector<std::string> > > _Records;
};

}   // namespace aries

#endif   // _ARIES_VARIATION_RECORD_H
