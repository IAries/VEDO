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

template <typename TType, typename TRecord>
class VariationRecord
{
public:

	static VariationRecord<TType, TRecord>* Instance()
	{
		if (_VariationRecord == 0)
		{
			_VariationRecord = new VariationRecord<TType, TRecord>;
		}
		return _VariationRecord;
	}

	VariationRecord<TType, TRecord>(const VariationRecord<TType, TRecord>& vr)
	{
		*this = vr;
	}

	const VariationRecord<TType, TRecord>& operator = (const VariationRecord<TType, TRecord>& vr)
	{
		this->Instance();
		return *this;
	}

	~VariationRecord()
	{
	}

	void AddRecord(const TType& type, const _float_t& time, const std::vector<TRecord>& records)
	{
		_Records[type][time] = records;
	}

	void AddRecord(const TType& type, const std::vector<TRecord>& records)
	{
		_Records[type].insert(std::make_pair(_CurrentTime, records));
	}

	std::multimap<_float_t, std::vector<TRecord> > GetRecord(const TType& type)
	{
		return _Records[type];
	}

	std::map<TType, std::multimap<_float_t, std::vector<TRecord> > > * GetRecord() const
	{
		return &_Records;
	}

	_uint_t size(const TType& type)
	{
		return _Records[type].size();
	}

	inline void SetCurrentTime(const _float_t& time)
	{
		_CurrentTime = time;
	}

	inline _float_t GetCurrentTime() const
	{
		return _CurrentTime;
	}

private:

	VariationRecord<TType, TRecord>()
	{
	}

	static VariationRecord<TType, TRecord>* _VariationRecord;

	_float_t _CurrentTime;

	std::map<TType, std::multimap<_float_t, std::vector<TRecord> > > _Records;
};

}   // namespace aries

#endif   // _ARIES_VARIATION_RECORD_H
