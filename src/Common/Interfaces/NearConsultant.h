#ifndef _NEAR_CONSULTANT_H
#define _NEAR_CONSULTANT_H

#include <FrameWork/Interfaces/Consultant.h>

namespace VEDO
{

class NearConsultant : public Consultant
{

public:

	NearConsultant
		(DOWorld* DOWorld,
		IactRecordTab* pIactRecordTab,
		char filename[],
		unsigned long ulwrite);

	virtual bool ISReset();

	virtual bool Reset();

	virtual void RebuildIactRecordTab(IactContainer& cIact);
};

};   // namespace VEDO


#endif //_NEAR_CONSULTANT_H
