#ifndef _NEAR_CONSULTANT_H
#define _NEAR_CONSULTANT_H

#include <vedo/framework/interfaces/Consultant.h>

namespace vedo
{

class NearConsultant : public Consultant
{

public:

	NearConsultant(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, vedo_uint_t ulwrite);

	virtual bool ISReset();

	virtual bool Reset();

	virtual void RebuildIactRecordTab(IactContainer& cIact);
};

}   // namespace vedo


#endif //_NEAR_CONSULTANT_H
