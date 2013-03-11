#include <FrameWork/Interfaces/Consultant.h>
#include <NJR/Interfaces/Utility.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

using namespace std;

Consultant::Consultant
	(DOWorld* DOWorld,
	IactRecordTab* pIactRecordTab,
	char filename[],
	unsigned long ulwrite         ):
	vcDO(0),
	vcIactMaster(0),
	vcIactSlave(0),
	culRecord(ulwrite),
	rank(0),
	NP(1),
	ImpactBufferSize(65536)
//	ImpactBufferSize = 18,000 for 60,000 elements / 4 processors
//	ImpactBufferSize = 60,000 for 60,000 elements / 8 processors
{
	pDOWorld                      = DOWorld;
	pIRTbl                        = pIactRecordTab;
	ulRecordCount                 = 1;
	ulRoundCount                  = 1;
	sfilename                     = filename;
	sfilename[strlen(filename)-4] = 0;
	timePartitioning              = 0.0;

	for(unsigned u=0; u<2*uNumUserDefinedData; u++)
		dUDV[u] = 0.0;
};

Consultant::~Consultant()
{
};

unsigned long Consultant::GetDONum() const
{
	return (unsigned long) vcDO.size();
};

unsigned long Consultant::GetIactNum() const
{
	return (unsigned long) vcIactMaster.size();
};

unsigned long Consultant::GetDO(unsigned long i) const
{
	return vcDO[i];
};

unsigned long Consultant::GetIactMaster(unsigned long i) const
{
	return vcIactMaster[i];
};

unsigned long Consultant::GetIactSlave(unsigned long i) const
{
	return vcIactSlave[i];
};

const DOWorld* Consultant::GetDOWorld() const
{
	return pDOWorld;
};

const IactRecordTab* Consultant::GetIactRecordTab() const
{
	return pIRTbl;
};

void Consultant::SyncDOContainer(DOContainer& cDO)
{
};

void Consultant::RecordIDO()
{
	char ltoa[256];

	string file = sfilename.c_str();
	file.append("_");

	sprintf(ltoa, "%d.ido\0", 10000000 + ulRecordCount);
//	file.append((ulRecordCount<10010000) ? ltoa + 4 : ltoa);
	file.append((ulRecordCount<11000000) ? ltoa + 2 : ltoa);

	pDOWorld->WriteIDO(file.c_str());
	this->pIRTbl->WriteIRT((file.substr(0,file.size()-4)+".irt").c_str());

	ulRecordCount++;
};

bool Consultant::NextStep(DOContainer &cDO, IactContainer& cIact)
{
	ulRoundCount++;
	pDOWorld->NextStep();
	if (ISRecord())
	{
		pDOWorld->UpdateDOStatus(cDO.GetDOStatus());
		RebuildIactRecordTab(cIact);
		RecordIDO();
	}
	return true;
};

const ImpactStatus* Consultant::RetrieveImpactStatus
	(unsigned long lcMaster, unsigned long lcSlave) const
{
	return pIRTbl->GetImpactStatus(lcMaster, lcSlave);
}

bool Consultant::InBoundary(unsigned long i) const
{
	NJRvector3d p = (pDOWorld->GetDOStatus())[i]->GetPosition();
	return
		pDOWorld
			->GetSystemParameter()
			->GetZoneOfInterest().InBoundary(p);
}

void Consultant::CleanUp(DOContainer &cDO, IactContainer &cIact)
{
	vector<unsigned long> RedundantElement;
	map<unsigned long, long> ElementMappingJump;

	SystemParameter* csp   = pDOWorld->GetSystemParameter();
	unsigned long numberDO = csp->GetDONumber();

	const DOStatus* pdos  = 0;
	const DOModel*  pdoml = 0;
	unsigned long ul;
	long ulDead;

	for(ul=0, ulDead=0; ul<numberDO; ul++)
	{
		pdos     = pDOWorld->GetDOStatus(ul);
		pdoml    = pDOWorld->GetDOModel(pdos->GetDOName());
		DOMap pm = (DOMap(ul, pdos, pdoml, 0.0));
		if (DOMap::ISMobile(pm))
		{
			if (! (InBoundary(ul)) )
			{
				RedundantElement.push_back(ul);
				ulDead--;
			}
		}
		ElementMappingJump[ul] = ulDead;
	}

	if (!RedundantElement.empty())
	{
		cerr
			<< "Number of redundant elements = "
			<< RedundantElement.size()
			<< " / "
			<< numberDO
			<< endl;

		cDO.Erase(RedundantElement);
		pIRTbl->ModifyPair(ElementMappingJump);
		pDOWorld->EraseDOStatus(RedundantElement);
		vcIactMaster.clear();
		vcIactSlave.clear();
		IactPairTab.clear();
		cIact.Clear();
	}
};

void Consultant::SyncWorld(DOContainer& vDO)
{
};

void Consultant::BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2)
{
};

void Consultant::BuildIactTab(std::vector<DOMap>& v)
{
};

void Consultant::CollectUserDefinedData(IactContainer& cIact)
{
	cIact.CollectUserDefinedData();

	for(unsigned u=0; u<2*uNumUserDefinedData; u++)
		dUDV[u] = cIact.GetUserDefinedValue(u);
};

double Consultant::GetUserDefinedValue(unsigned u) const
{
	if(u < 2*uNumUserDefinedData)
	{
		return dUDV[u];
	}
	else
	{
		cerr << "Consultant::GetUserDefinedValue(unsigned) const ERROR!!" << endl;
		exit(-1);
	}
};
