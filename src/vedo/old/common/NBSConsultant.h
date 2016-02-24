#ifndef _NBS_CONSULTANT_H
#define _NBS_CONSULTANT_H

#include <vedo/framework/Consultant.h>
#include <utility>

namespace vedo
{

class NBSConsultant : public Consultant
{

public:

	NBSConsultant(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, _uint_t ulwrite, _uint_t ulupdate);

	virtual _uint_t GetIactNum() const;

	virtual _uint_t GetIactMaster (_uint_t i) const;

	virtual _uint_t GetIactSlave (_uint_t i) const;

	virtual bool ISReset();

	virtual bool Reset();

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	// The time spent on reset
	_float_t timeReset;

	virtual void RebuildIactRecordTab(IactContainer& cIact);

private:

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);
};



class Trir
{

public:

	Trir(_uint_t _xidx,_uint_t _yidx,_uint_t _zidx) : xIdx(_xidx), yIdx(_yidx), zIdx(_zidx)
	{
	}

	_uint_t xIdx;

	_uint_t yIdx;

	_uint_t zIdx;

};



class TrirLeY
{

public:

	bool operator() (const Trir& t1,const Trir& t2) const
	{
		if (t1.yIdx != t2.yIdx)
		{
			return (t1.yIdx < t2.yIdx);
		}

		if (t1.xIdx != t2.xIdx)
		{
			return (t1.xIdx < t2.xIdx);
		}
		return (t1.zIdx < t2.zIdx);
	}
};



class TrirLeX
{

public:

	bool operator() (const Trir& t1, const Trir& t2) const
	{
		if (t1.xIdx != t2.xIdx)
		{
			return (t1.xIdx < t2.xIdx);
		}

		if (t1.yIdx != t2.yIdx)
		{
			return (t1.yIdx < t2.yIdx);
		}
		return (t1.zIdx < t2.zIdx);
	}
};



class TrirLeZ
{

public:

	bool operator() (const Trir& t1, const Trir& t2) const
	{
		if (t1.zIdx != t2.zIdx)
		{
			return (t1.zIdx < t2.zIdx);
		}

		if (t1.xIdx != t2.xIdx)
		{
			return (t1.xIdx < t2.xIdx);
		}

		return (t1.yIdx < t2.yIdx);
	}
};

}   // namespace vedo

#endif // _NBS_CONSULTANT_H
