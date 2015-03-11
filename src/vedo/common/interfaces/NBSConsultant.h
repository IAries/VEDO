#ifndef _NBS_CONSULTANT_H
#define _NBS_CONSULTANT_H

#include <vedo/framework/interfaces/Consultant.h>
#include <utility>

namespace vedo
{

class NBSConsultant : public Consultant
{

public:

	NBSConsultant(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, vedo_uint_t ulwrite, vedo_uint_t ulupdate);

	virtual vedo_uint_t GetIactNum() const;

	virtual vedo_uint_t GetIactMaster (vedo_uint_t i) const;

	virtual vedo_uint_t GetIactSlave (vedo_uint_t i) const;

	virtual bool ISReset();

	virtual bool Reset();

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	// The time spent on reset
	vedo_float_t timeReset;

	virtual void RebuildIactRecordTab(IactContainer& cIact);

private:

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);
};



class Trir
{

public:

	Trir(vedo_int_t _xidx,vedo_int_t _yidx,vedo_int_t _zidx) : xIdx(_xidx), yIdx(_yidx), zIdx(_zidx)
	{
	}

	vedo_int_t xIdx;

	vedo_int_t yIdx;

	vedo_int_t zIdx;

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
