#ifndef _SAFE_CONSULTANT_H
#define _SAFE_CONSULTANT_H

#include <vedo/framework/interfaces/Consultant.h>

namespace vedo
{

class SafeConsultant : public Consultant
{

public:

	SafeConsultant(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, vedo_uint_t ulwrite);

	virtual bool ISReset();

	virtual bool Reset();

	virtual void RebuildIactRecordTab(IactContainer& cIact);
};

}   // namespace vedo

#endif  // _SAFE_CONSULTANT_H
