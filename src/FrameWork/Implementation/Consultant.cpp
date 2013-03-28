#include <FrameWork/Interfaces/Constants.h>
#include <FrameWork/Interfaces/Consultant.h>
#include <NJR/Interfaces/Utility.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

namespace VEDO
{

void EnsureLength
	(unsigned int base,
	 unsigned long target,
	 unsigned long& length,
	 double*& array        )
{
	if ((base*target) > length)
	{
		while ((base*target) > length)
        	length *= 2;

		delete[] array;
		array = new double[length];

		#ifdef _VEDO_DEBUG
			// Monitor the status of "Impact Buffer"
			std::cout
				<< "Resizing Impact Buffer = "
				<< length
				<< " (LeapConsulant::EnsureLength)"
				<< std::endl;
		#endif   // _VEDO_DEBUG
	}

/*
	if ((base*target) != 0)
	{
		if ((base*target) > length)
		{
			length = base*target;
			#ifdef _VEDO_DEBUG
				// Monitor the status of "Impact Buffer"
				std::cout
					<< "Resizing Impact Buffer = "
					<< length
					<< " (NBSGPConsulant::EnsureLength)\n";
			#endif   // _VEDO_DEBUG
			delete[] array;
			array = new double[length];
		}
		else if ((length%base) != 0)
		{
			length += (base - (length%base));
			#ifdef _VEDO_DEBUG
				// Monitor the status of "Impact Buffer"
				std::cout
					<< "Resizing Impact Buffer = "
					<< length
					<< " (NBSGPConsulant::EnsureLength)\n";
			#endif   // _VEDO_DEBUG
			delete[] array;
			array = new double[length];
		}
	}
*/
};

X_Comp::X_Comp(const std::vector<DOMap>& vDOMap): map(vDOMap)
{
};

bool X_Comp::operator() (const IactPair& p1, const IactPair& p2)
{
	return (CalcIactCoordinate(p1) <= CalcIactCoordinate(p2));
};

double X_Comp::CalcIactCoordinate(const IactPair& p)
{
	if (DOMap::ISConstrained(map[p.first]) || DOMap::ISFixed(map[p.first]))
		return map[p.second].cpdos()->GetPosition().x();
	else if (DOMap::ISConstrained(map[p.second]) || DOMap::ISFixed(map[p.second]))
		return map[p.first].cpdos()->GetPosition().x();
	else
		return
			std::min
				(map[p.second].cpdos()->GetPosition().x(),
				 map[p.first].cpdos()->GetPosition().x()   );
};

Y_Comp::Y_Comp(const std::vector<DOMap>& vDOMap): map(vDOMap)
{
};

bool Y_Comp::operator() (const IactPair& p1, const IactPair& p2)
{
	return (CalcIactCoordinate(p1) <= CalcIactCoordinate(p2));
};

double Y_Comp::CalcIactCoordinate(const IactPair& p)
{
	if (DOMap::ISConstrained(map[p.first]) || DOMap::ISFixed(map[p.first]))
		return map[p.second].cpdos()->GetPosition().y();
	else if (DOMap::ISConstrained(map[p.second]) || DOMap::ISFixed(map[p.second]))
		return map[p.first].cpdos()->GetPosition().y();
	else
		return
			std::min
				(map[p.second].cpdos()->GetPosition().y(),
				 map[p.first].cpdos()->GetPosition().y()   );
};

Z_Comp::Z_Comp(const std::vector<DOMap>& vDOMap): map(vDOMap)
{
};

bool Z_Comp::operator() (const IactPair& p1, const IactPair& p2)
{
	return (CalcIactCoordinate(p1) <= CalcIactCoordinate(p2));
};

double Z_Comp::CalcIactCoordinate(const IactPair& p)
{
	if (DOMap::ISConstrained(map[p.first]) || DOMap::ISFixed(map[p.first]))
		return map[p.second].cpdos()->GetPosition().z();
	else if (DOMap::ISConstrained(map[p.second]) || DOMap::ISFixed(map[p.second]))
		return map[p.first].cpdos()->GetPosition().z();
	else
		return
			std::min
				(map[p.second].cpdos()->GetPosition().z(),
				 map[p.first].cpdos()->GetPosition().z()   );
};

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
	ImpactBufferSize(VEDO::uImpactBufferSize)
{
	pDOWorld                      = DOWorld;
	pIRTbl                        = pIactRecordTab;
	ulRecordCount                 = 1;
	ulRoundCount                  = 1;
	sfilename                     = filename;
	sfilename[strlen(filename)-4] = 0;
	timePartitioning              = 0.0;

	for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
		dUDVIS[u] = 0.0;

//	for(unsigned u=0; u<2*uNumUDDDOStatus; u++)
//		dUDVDS[u] = 0.0;
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

	std::string file = sfilename.c_str();
	file.append("_");

	sprintf(ltoa, "%d.ido\0", 10000000 + ulRecordCount);
//	file.append((ulRecordCount<10010000) ? ltoa + 4 : ltoa);
	file.append((ulRecordCount<11000000) ? ltoa + 2 : ltoa);

	pDOWorld->WriteIDO(file.c_str(), pIRTbl);
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
	NJR::Vector3d p = (pDOWorld->GetDOStatus())[i]->GetPosition();
	return
		pDOWorld
			->GetSystemParameter()
			->GetZoneOfInterest().InBoundary(p);
}

void Consultant::CleanUp(DOContainer &cDO, IactContainer &cIact)
{
	std::vector<unsigned long> RedundantElement;
	std::map<unsigned long, long> ElementMappingJump;

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
        #ifdef _VEDO_DEBUG
            std::cout
                << "Number of redundant elements = "
                << RedundantElement.size()
                << " / "
                << numberDO
                << std::endl;
        #endif   // _VEDO_DEBUG

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

	for(unsigned u=0; u<2*uNumUDDImpactStatus; u++)
		dUDVIS[u] = cIact.GetUserDefinedValue(u);
};

double Consultant::GetUserDefinedValue(unsigned u) const
{
	if(u < 2*uNumUDDImpactStatus)
	{
		return dUDVIS[u];
	}
	else
	{
		std::cerr
			<< "Error!! Code: Consultant::GetUserDefinedValue(unsigned u)"
			<< std::endl;
		exit(-1);
	}
};

};   // namespace VEDO
