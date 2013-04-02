#ifndef _NEAR_CONSULTANT_H
#define _NEAR_CONSULTANT_H

#include <Framework/Interfaces/Consultant.h>

namespace vedo
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

};   // namespace vedo


#endif //_NEAR_CONSULTANT_H
