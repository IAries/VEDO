#include <Common/Interfaces/SafeConsultant.h>
#include <string>

using namespace std;

SafeConsultant::SafeConsultant
	(DOWorld* DOWorld,
	IactRecordTab* pIactRecordTab,
	char filename[],
	unsigned long ulwrite)
: Consultant(DOWorld, pIactRecordTab, filename, ulwrite)
{
	SafeConsultant::Reset();
};

bool SafeConsultant::ISReset()
{
	return false;
};

bool SafeConsultant::Reset()
{
	unsigned long ulDONum = pDOWorld->GetSystemParameter()->GetDONumber();
    unsigned long ul, uj;
	const IactModel* cpiactml;
	string doname1, doname2;

	vcDO.clear();
	vcIactMaster.clear();
	vcIactSlave.clear();

    for (ul=0; ul<ulDONum; ++ul)
    {
		vcDO.push_back(ul);
	}

	for (ul = 0; ul < ulDONum ; ++ul)
	{
		for (uj=ul+1; uj<ulDONum ;++uj)
		{
			doname1 = pDOWorld->GetDOStatus(ul)->GetDOName();
			doname2 = pDOWorld->GetDOStatus(uj)->GetDOName();

			cpiactml = pDOWorld->GetIactModel
				(pDOWorld->GetDOModel(doname1)->GetDOGroup(),
				pDOWorld->GetDOModel(doname2)->GetDOGroup());

        	if (cpiactml == 0)
        	{
				continue;
			}

			if ( doname1 == (cpiactml->GetMasterDOGroup()) )
			{
				vcIactMaster.push_back(ul);
				vcIactSlave.push_back(uj);
			}
			else
			{
				vcIactMaster.push_back(uj);
				vcIactSlave.push_back(ul);
			}
		}
	}

	cerr
		<< "Interaction size = "
		<< (unsigned int) vcIactMaster.size()
		<< '\n';

	return true;
};

void SafeConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	CollectUserDefinedData(cIact);
};
