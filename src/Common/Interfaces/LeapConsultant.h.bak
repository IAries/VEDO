#ifndef _LEAP_CONSULTANT_H
#define _LEAP_CONSULTANT_H

#include <FrameWork/Interfaces/Consultant.h>
#include <utility>

class LeapConsultant : public Consultant
{

public:

	LeapConsultant
		(DOWorld* DOWorld,
		IactRecordTab* pIactRecordTab,
		char filename[],
		unsigned long ulwrite,
		unsigned long ulupdate);

	virtual unsigned long GetIactNum() const;

	virtual unsigned long GetIactMaster (unsigned long i) const;

	virtual unsigned long GetIactSlave (unsigned long i) const;

	virtual bool ISReset();

	virtual bool Reset();

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	virtual void RebuildIactRecordTab(IactContainer& cIact);
};

#endif // _LEAP_CONSULTANT_H
