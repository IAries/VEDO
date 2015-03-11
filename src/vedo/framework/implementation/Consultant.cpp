#include <vedo/constants/interfaces/Constants.h>
#include <vedo/njr/interfaces/Utility.h>
#include <vedo/framework/interfaces/Consultant.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

namespace vedo
{

void EnsureLength(vedo_uint_t base, vedo_uint_t target, vedo_uint_t& length, vedo_float_t*& array)
{
	if ((base*target) > length)
	{
		while ((base*target) > length)
		{
        	length *= 2;
        }

		delete[] array;
		array = new vedo_float_t[length];

		#ifdef _VEDO_DEBUG
			// Monitor the status of "Impact Buffer"
			std::cout << "Resizing Impact Buffer = " << length << " (LeapConsulant::EnsureLength)" << std::endl;
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
			array = new vedo_float_t[length];
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
			array = new vedo_float_t[length];
		}
	}
*/
}

X_Comp::X_Comp(const std::vector<DOMap>& vDOMap): map(vDOMap)
{
}

bool X_Comp::operator() (const IactPair& p1, const IactPair& p2)
{
	return (CalcIactCoordinate(p1) <= CalcIactCoordinate(p2));
}

vedo_float_t X_Comp::CalcIactCoordinate(const IactPair& p)
{
	if (DOMap::ISConstrained(map[p.first]) || DOMap::ISFixed(map[p.first]))
	{
		return map[p.second].cpdos()->GetPosition().x();
	}
	else if (DOMap::ISConstrained(map[p.second]) || DOMap::ISFixed(map[p.second]))
	{
		return map[p.first].cpdos()->GetPosition().x();
	}
	else
	{
		return std::min(map[p.second].cpdos()->GetPosition().x(),  map[p.first].cpdos()->GetPosition().x());
	}
}

Y_Comp::Y_Comp(const std::vector<DOMap>& vDOMap): map(vDOMap)
{
}

bool Y_Comp::operator() (const IactPair& p1, const IactPair& p2)
{
	return (CalcIactCoordinate(p1) <= CalcIactCoordinate(p2));
}

vedo_float_t Y_Comp::CalcIactCoordinate(const IactPair& p)
{
	if (DOMap::ISConstrained(map[p.first]) || DOMap::ISFixed(map[p.first]))
	{
		return map[p.second].cpdos()->GetPosition().y();
	}
	else if (DOMap::ISConstrained(map[p.second]) || DOMap::ISFixed(map[p.second]))
	{
		return map[p.first].cpdos()->GetPosition().y();
	}
	else
	{
		return std::min(map[p.second].cpdos()->GetPosition().y(), map[p.first].cpdos()->GetPosition().y());
	}
}

Z_Comp::Z_Comp(const std::vector<DOMap>& vDOMap): map(vDOMap)
{
}

bool Z_Comp::operator() (const IactPair& p1, const IactPair& p2)
{
	return (CalcIactCoordinate(p1) <= CalcIactCoordinate(p2));
}

vedo_float_t Z_Comp::CalcIactCoordinate(const IactPair& p)
{
	if (DOMap::ISConstrained(map[p.first]) || DOMap::ISFixed(map[p.first]))
	{
		return map[p.second].cpdos()->GetPosition().z();
	}
	else if (DOMap::ISConstrained(map[p.second]) || DOMap::ISFixed(map[p.second]))
	{
		return map[p.first].cpdos()->GetPosition().z();
	}
	else
	{
		return std::min(map[p.second].cpdos()->GetPosition().z(), map[p.first].cpdos()->GetPosition().z());
	}
}

Consultant::Consultant
	(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, vedo_uint_t ulwrite):
		vcDO(0), vcIactMaster(0), vcIactSlave(0), culRecord(ulwrite), rank(0), NP(1)
{
	pDOWorld         = DOWorld;
	pIRTbl           = pIactRecordTab;
	ulRecordCount    = 0;
	ulRoundCount     = 0;
	sfilename        = filename.substr(0, filename.length()-4);
	timePartitioning = 0.0;
	vedo::Constants* vedo_cp = vedo::Constants::Instance();
	ImpactBufferSize = vedo_cp->ImpactBufferSize();

	if (uNumUDDImpactStatus != 0)
	{
        dpUDVIS = new vedo_float_t[2*uNumUDDImpactStatus];
        for (vedo_uint_t u=0; u<2*uNumUDDImpactStatus; u++)
        {
            *(dpUDVIS+u) = 0.0;
        }
	}

//	for(vedo_uint_t u=0; u<2*uNumUDDDOStatus; u++)
//		dUDVDS[u] = 0.0;
}

Consultant::~Consultant()
{
//	if(dpUDVIS)
	if (uNumUDDImpactStatus != 0)
	{
		delete dpUDVIS;
	}
}

vedo_uint_t Consultant::GetDONum() const
{
	return (vedo_uint_t) vcDO.size();
}

vedo_uint_t Consultant::GetIactNum() const
{
	return (vedo_uint_t) vcIactMaster.size();
}

vedo_uint_t Consultant::GetDO(vedo_uint_t i) const
{
	return vcDO[i];
}

vedo_uint_t Consultant::GetIactMaster(vedo_uint_t i) const
{
	return vcIactMaster[i];
}

vedo_uint_t Consultant::GetIactSlave(vedo_uint_t i) const
{
	return vcIactSlave[i];
}

const DOWorld* Consultant::GetDOWorld() const
{
	return pDOWorld;
}

const IactRecordTab* Consultant::GetIactRecordTab() const
{
	return pIRTbl;
}

void Consultant::SyncDOContainer(DOContainer& cDO)
{
}

bool Consultant::ISReset()
{
	return true;
}

void Consultant::RecordIDO()
{
	char ltoa[256];

	std::string file = sfilename;
	file.append("_");

	sprintf(ltoa, "%d.ido\0", 10000001 + ulRecordCount);
//	file.append((ulRecordCount<10010001) ? ltoa + 4 : ltoa);
	file.append((ulRecordCount<11000001) ? ltoa + 2 : ltoa);

	pDOWorld->WriteIDO(file.c_str(), pIRTbl);
	ulRecordCount++;
}

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
	cIact.BackupImpactStatus();
	return true;
}

bool Consultant::Reset()
{
	return true;
}

const ImpactStatus* Consultant::RetrieveImpactStatus(vedo_uint_t lcMaster, vedo_uint_t lcSlave) const
{
	return pIRTbl->GetImpactStatus(lcMaster, lcSlave);
}

bool Consultant::InBoundary(vedo_uint_t i) const
{
	njr::Vector3d p = (pDOWorld->GetDOStatus())[i]->GetPosition();
	return pDOWorld->GetSystemParameter()->GetZoneOfInterest().InBoundary(p);
}

bool Consultant::CleanUp(DOContainer &cDO, IactContainer &cIact)
{
	SystemParameter* csp   = pDOWorld->GetSystemParameter();
	if (csp->GetZoneOfInterest().Active())
	{
		std::vector<vedo_uint_t> RedundantElement;
		std::map<vedo_uint_t, vedo::vedo_int_t> ElementMappingJump;

		vedo_uint_t numberDO = csp->GetDONumber();

		const DOStatus* pdos  = 0;
		const DOModel*  pdoml = 0;
		vedo_uint_t ul;
		vedo::vedo_int_t ulDead;

		for (ul=0, ulDead=0; ul<numberDO; ul++)
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
				std::cout << "Number of redundant elements = " << RedundantElement.size() << " / " << numberDO << std::endl;
			#endif   // _VEDO_DEBUG

			cDO.Erase(RedundantElement);
			pIRTbl->ModifyPair(ElementMappingJump);
			pDOWorld->EraseDOStatus(RedundantElement);
			vcIactMaster.clear();
			vcIactSlave.clear();
			IactPairTab.clear();
			cIact.Clear();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Consultant::SyncWorld(DOContainer& vDO)
{
}

void Consultant::RebuildIactRecordTab(IactContainer& cIact)
{
}

void Consultant::BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2)
{
}

void Consultant::BuildIactTab(std::vector<DOMap>& v)
{
}

bool Consultant::EraseSingleElement(const vedo_uint_t& ulID)
{
	if (ulID < pDOWorld->GetSystemParameter()->GetDONumber())
	{
		pDOWorld->EraseDOStatus(ulID);
		pIRTbl->EraseElement(ulID);
		return true;
	}
	else
	{
		std::cout
			<< "Error!! Code: Consultant::EraseSingleElement" << std::endl
			<< "        Note: The ID of element is wrong!"    << std::endl;
		return false;
	}
}

bool Consultant::EraseElements(const std::vector<vedo_uint_t>& ulIDList)
{
	pIRTbl->EraseElements(ulIDList, pDOWorld->GetSystemParameter()->GetDONumber());
	pDOWorld->EraseDOStatus(ulIDList);
	return true;
}

void Consultant::CollectUserDefinedData(IactContainer& cIact)
{
	cIact.CollectUserDefinedData();
    memcpy(dpUDVIS, cIact.GetUserDefinedValue(), 2*uNumUDDImpactStatus*sizeof(vedo_float_t));
}

vedo_float_t Consultant::GetUserDefinedValue(vedo_uint_t u) const
{
	if (u < 2*uNumUDDImpactStatus)
	{
		return *(dpUDVIS+u);
	}
	else
	{
		std::cerr << "Error!! Code: Consultant::GetUserDefinedValue(vedo_uint_t u)" << std::endl;
		exit(-1);
	}
}

}   // namespace vedo
