#ifndef _NBS_CONSULTANT_H
#define _NBS_CONSULTANT_H

#include <FrameWork/Interfaces/Consultant.h>
#include <utility>

class NBSConsultant : public Consultant
{

public:

	NBSConsultant
		(DOWorld* DOWorld,
		IactRecordTab* pIactRecordTab,
		char filename[],
		unsigned long ulwrite ,
		unsigned long ulupdate);

	virtual unsigned long GetIactNum() const;

	virtual unsigned long GetIactMaster (unsigned long i) const;

	virtual unsigned long GetIactSlave (unsigned long i) const;

	virtual bool ISReset();

	virtual bool Reset();

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	// The time spent on reset
	double timeReset;

	virtual void RebuildIactRecordTab(IactContainer& cIact);

private:

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);
};



class Trir
{

public:

	Trir(int _xidx,int _yidx,int _zidx) : xIdx(_xidx), yIdx(_yidx), zIdx(_zidx)
	{
	}

	int xIdx;
	int yIdx;
	int zIdx;

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

#endif // _NBS_CONSULTANT_H
