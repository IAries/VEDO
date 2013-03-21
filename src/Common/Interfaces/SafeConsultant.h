#ifndef _SAFE_CONSULTANT_H
#define _SAFE_CONSULTANT_H

#include <FrameWork/Interfaces/Consultant.h>

namespace VEDO
{

class SafeConsultant : public Consultant
{

public:

	SafeConsultant
		(DOWorld* DOWorld,
		IactRecordTab* pIactRecordTab,
		char filename[],
		unsigned long ulwrite);

	virtual bool ISReset();

	virtual bool Reset();

	virtual void RebuildIactRecordTab(IactContainer& cIact);
};

};   // namespace VEDO

#endif  // _SAFE_CONSULTANT_H
