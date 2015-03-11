#include <vedo/njr/interfaces/Utility.h>
#include <vedo/common/interfaces/NBSConsultant.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <map>
#include <limits>

namespace vedo
{

NBSConsultant::NBSConsultant
	(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, vedo_uint_t ulwrite, vedo_uint_t ulupdate):
		Consultant(DOWorld, pIactRecordTab, filename, ulwrite)
{
	culUpIact = ulupdate;
	timeReset = 0.0;
	NBSConsultant::Reset();
}

vedo_uint_t NBSConsultant::GetIactNum() const
{
	return (vedo_uint_t)(IactPairTab.size());
}

vedo_uint_t NBSConsultant::GetIactMaster(vedo_uint_t i) const
{
	return IactPairTab[i].first;
}

vedo_uint_t NBSConsultant::GetIactSlave(vedo_uint_t i) const
{
	return IactPairTab[i].second;
}

bool NBSConsultant::NextStep(DOContainer &cDO, IactContainer& cIact)
{
	ulRoundCount++;
	pDOWorld->NextStep();
	bool rebuild = false;
	if (ISReset())
	{
		pDOWorld->UpdateDOStatus(cDO.GetDOStatus());
		RebuildIactRecordTab(cIact);
		rebuild =true;
		cIact.CollectUserDefinedData();
		NBSConsultant::Reset();
	}

	if (ISRecord())
	{
		if (!rebuild)
		{
			pDOWorld->UpdateDOStatus(cDO.GetDOStatus());
			RebuildIactRecordTab(cIact);
			rebuild =true;
			cIact.CollectUserDefinedData();
		}
		Consultant::RecordIDO();
	}

	cIact.BackupImpactStatus();

	const SystemParameter* csp = pDOWorld->GetSystemParameter();

	if ((csp->GetTimeCurrent()) >= (csp->GetTimeStop()))
	{
		if (!rebuild)
		{
			pDOWorld->UpdateDOStatus (cDO.GetDOStatus());
			RebuildIactRecordTab(cIact);
			rebuild = true;
			cIact.CollectUserDefinedData();
		}
//		pDOWorld->WriteXML("terminate.xml");
		pDOWorld->WriteIDO("terminate.ido", pIRTbl);
	}
	return true;
}

bool NBSConsultant::ISReset()
{
	return ((ulRoundCount % culUpIact) == 0);
}

bool NBSConsultant::Reset()
{
	time_t starttime;
	time(&starttime);

	//njr::RunTime("Enter NBS Reset");

	vedo_uint_t ul;
	vcDO.clear();
	IactPairTab.clear();

	const SystemParameter* csp = pDOWorld->GetSystemParameter();
	vedo_uint_t numberDO = csp->GetDONumber();
	njr::Vector3d vFieldAcceleration = csp->GetFieldAcceleration();
	vedo_float_t dt = culUpIact * csp->GetTimeInterval();

	std::vector<DOMap> vDOMap;
	std::vector<DOMap> FTab;
	std::vector<DOMap> MTab;
	std::vector<DOMap> WorkTab;

	vedo_float_t xmin = std::numeric_limits<vedo_float_t>::max();
	vedo_float_t xmax = std::numeric_limits<vedo_float_t>::min();
	vedo_float_t ymin = std::numeric_limits<vedo_float_t>::max();
	vedo_float_t ymax = std::numeric_limits<vedo_float_t>::min();
	vedo_float_t zmin = std::numeric_limits<vedo_float_t>::max();
	vedo_float_t zmax = std::numeric_limits<vedo_float_t>::min();
	vedo_float_t vmax = 0.0;
	vedo_float_t rmax = 0.0;

	vedo_float_t safeD;   // The safe distance of each element
	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	Constants* vedo_cp = Constants::Instance();
	vedo_float_t SafetyFactor = vedo_cp->SafetyFactor();
	for (ul=0; ul<numberDO; ++ul)
	{
		vcDO.push_back(ul);

		cpdos  = pDOWorld->GetDOStatus(ul);
		cpdoml = pDOWorld->GetDOModel(cpdos->GetDOName());

		safeD
			= SafetyFactor
			* (2.0 * cpdoml->GetRange() + dt  * (cpdos->GetVelocity()).length() + 0.5 * dt * dt * vFieldAcceleration.length());

		if (cpdoml->GetScope() == local)
		{
			xmin = std::min(xmin, cpdos->GetPosition().x()     );
			xmax = std::max(xmax, cpdos->GetPosition().x()     );
			ymin = std::min(ymin, cpdos->GetPosition().y()     );
			ymax = std::max(ymax, cpdos->GetPosition().y()     );
			zmin = std::min(zmin, cpdos->GetPosition().z()     );
			zmax = std::max(zmax, cpdos->GetPosition().z()     );
			vmax = std::max(vmax, cpdos->GetVelocity().length());
			rmax = cpdoml->GetRange();
		}

		vDOMap.push_back(DOMap(ul, cpdos, cpdoml, safeD));
	}
	// Max safe distance
	vedo_float_t ZoneRange = SafetyFactor * (2.0 * rmax + dt * vmax + 0.5 * dt * dt * vFieldAcceleration.length());
	// Determine how many "safety region" per direction
	vedo_int_t ncelx = std::ceil((xmax - xmin) / ZoneRange);
	ncelx = std::max(ncelx, (vedo_int_t)1);
	vedo_int_t ncely = std::ceil((ymax - ymin) / ZoneRange);
	ncely = std::max(ncely, (vedo_int_t)1);
	vedo_int_t ncelz = std::ceil((zmax - zmin) / ZoneRange);
	ncelz = std::max(ncelz, (vedo_int_t)1);

	#ifdef _VEDO_DEBUG
        std::cout << "(ncelx, necly, ncelz) = (" << ncelx << ',' << ncely << ',' << ncelz << ")" << std::endl;
	#endif   // _VEDO_DEBUG

	std::map<Trir, std::vector<DOMap>*, TrirLeY> locMap;

	std::vector<DOMap> GlobalElement;

	for (ul=0; ul<numberDO; ++ul)
	{
		if (vDOMap[ul].cpdoml()->GetScope() == local)
		{
			njr::Vector3d p = vDOMap[ul].cpdos()->GetPosition();
			Trir zone
				(static_cast<vedo_int_t>((p.x()-xmin)/ZoneRange),
				 static_cast<vedo_int_t>((p.y()-ymin)/ZoneRange),
				 static_cast<vedo_int_t>((p.z()-zmin)/ZoneRange) );

			if (locMap.find(zone) == locMap.end())
			{
				locMap.insert(std::make_pair(zone, new std::vector<DOMap>()));
			}
			locMap[zone]->push_back(vDOMap[ul]);
		}
		else
		{
			GlobalElement.push_back(vDOMap[ul]);
		}
	}

	std::map<Trir, std::vector<DOMap>*, TrirLeY>::iterator iter1, iter2;
	for (vedo_int_t ix=0; ix<ncelx; ++ix)
	{
		for (vedo_int_t iy=0; iy<ncely; ++iy)
		{
			for (vedo_int_t iz=0; iz<ncelz; ++iz)
			{

				//std::cout << "zone (" << ix << ',' << iy << ',' << iz << " )\n";
				iter1 = locMap.find(Trir(ix, iy, iz));
				if (iter1 == locMap.end())
				{
					continue;
				}
				std::vector<DOMap>* pvec1 = iter1->second;

				// Self and neightbor zone check
				if ((iter2 = locMap.find(Trir(ix-1, iy-1, iz-1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix-1, iy-1, iz  ))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix-1, iy-1, iz+1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix  , iy-1, iz-1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix  , iy-1, iz  ))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix  , iy-1, iz+1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix+1, iy-1, iz-1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix+1, iy-1, iz  ))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix+1, iy-1, iz+1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix-1, iy  , iz-1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix-1, iy  , iz  ))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ((iter2 = locMap.find(Trir(ix  , iy  , iz-1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				// Self and self detection
				BuildIactTab(*pvec1);

				// Self and fix element detection
				BuildIactTab(*pvec1, GlobalElement);

				if ((iter2 = locMap.find(Trir(ix+1, iy  , iz-1))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}
	}
/*
	std::cout
		<< "(Iact size = "
		<< (vedo_uint_t)(NBSConsultant::IactPairTab.size())
		<< ")\n";

	sort
		(NBSConsultant::IactPairTab.begin(), NBSConsultant::IactPairTab.end());

	NBSConsultant::IactPairTab.erase
		(unique
			(NBSConsultant::IactPairTab.begin(),
			NBSConsultant::IactPairTab.end()),
			NBSConsultant::IactPairTab.end()    );

	std::cout
		<< "(SafeDistance = "
		<< SD
		<< ") (Cutting Range = "
		<< Range
		<< ") (Uniqued Iact size = "
		<< (vedo_uint_t)(IactPairTab.size())
		<< ")\n";
*/
	#ifdef _VEDO_DEBUG
        std::cout << "(Cutting Range = " << ZoneRange << ") (Uniqued Iact size = " << (vedo_uint_t)(IactPairTab.size()) << ")" << std::endl;
	#endif   // _VEDO_DEBUG

	// Clear the LocMap data
	for (iter1=locMap.begin(); iter1!=locMap.end(); ++iter1)
	{
		delete iter1->second;
	}

	//njr::RunTime("Leap Reset Accomplished");

	time_t endtime;
	time(&endtime);
	timeReset += endtime - starttime;

	return true;
}

void NBSConsultant::BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2)
{
	for (vedo_uint_t i=0; i<v1.size(); ++i)
	{
		for (vedo_uint_t j=0; j<v2.size(); ++j)
		{
			const IactModel* cpiactml = pDOWorld->GetIactModel(v1[i].cpdoml()->GetDOGroup(), v2[j].cpdoml()->GetDOGroup());
			if (cpiactml == 0)
			{
				continue;
			}

			if (DOMap::CalDistance(v1[i], v2[j]) > (v1[i].SafeLength() + v2[j].SafeLength()))
			{
				continue;
			}

			if (v1[i].id() < v2[j].id())
			{
				NBSConsultant::IactPairTab.push_back(std::make_pair(v1[i].id(), v2[j].id()));
			}
			else
			{
				NBSConsultant::IactPairTab.push_back(std::make_pair(v2[j].id(), v1[i].id()));
			}
		}
	}
}

void NBSConsultant::BuildIactTab(std::vector<DOMap>& v)
{
	for (vedo_uint_t i=0; i<v.size(); ++i)
	{
		for (vedo_uint_t j=i+1; j<v.size(); ++j)
		{
			const IactModel* cpiactml = pDOWorld->GetIactModel(v[i].cpdoml()->GetDOGroup(), v[j].cpdoml()->GetDOGroup());
			if (cpiactml == 0)
			{
				continue;
			}

			if (DOMap::CalDistance(v[i], v[j]) > (v[i].SafeLength() + v[j].SafeLength()))
			{
				continue;
			}

			if (v[i].id() < v[j].id())
			{
				NBSConsultant::IactPairTab.push_back(std::make_pair(v[i].id(), v[j].id()));
			}
			else
			{
				NBSConsultant::IactPairTab.push_back(std::make_pair(v[j].id(), v[i].id()));
			}
		}
	}
}

void NBSConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	CollectUserDefinedData(cIact);

	this->pIRTbl->Clear();
	for (vedo_uint_t i=0; i<cIact.size(); ++i)
	{
		const Interaction* pInt = cIact.GetInteraction(i);
		if ( pInt->IsActive() )
		{
			this->pIRTbl->PushRecord(IactPairTab[i].first, IactPairTab[i].second, *pInt->GetImpactStatus());
		}
	}

	#ifdef _VEDO_DEBUG
		std::cout
			<< "At RebuildIactRecordTab: size  = " << pIRTbl->GetTabSize() << std::endl
			<< "At the instant, pDOWorld->time = " << pDOWorld->GetSystemParameter()->GetTimeCurrent() << std::endl;
	#endif   // _VEDO_DEBUG
}

}   // namespace vedo
