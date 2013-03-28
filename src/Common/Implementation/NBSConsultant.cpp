#include <NJR/Interfaces/Utility.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <map>
#include <limits>

namespace VEDO
{

NBSConsultant::NBSConsultant
	(DOWorld* DOWorld,
	IactRecordTab* pIactRecordTab,
	char filename[],
	unsigned long ulwrite,
	unsigned long ulupdate)
: Consultant(DOWorld, pIactRecordTab, filename, ulwrite)
{
	culUpIact = ulupdate;
	timeReset=0.0;
	NBSConsultant::Reset();
};

unsigned long NBSConsultant::GetIactNum() const
{
	return (unsigned long)(IactPairTab.size());
};

unsigned long NBSConsultant::GetIactMaster(unsigned long i) const
{
	return IactPairTab[i].first;
};

unsigned long NBSConsultant::GetIactSlave(unsigned long i) const
{
	return IactPairTab[i].second;
};

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
};

bool NBSConsultant::ISReset()
{
	return ((ulRoundCount % culUpIact) == 0);
};

bool NBSConsultant::Reset()
{
	time_t starttime;
	time(&starttime);

	//NJR::RunTime("Enter NBS Reset");

	unsigned long ul;
	vcDO.clear();
	IactPairTab.clear();

	const SystemParameter* csp = pDOWorld->GetSystemParameter();
	unsigned long numberDO = csp->GetDONumber();
	NJR::Vector3d vFieldAcceleration = csp->GetFieldAcceleration();
	double dt = culUpIact * csp->GetTimeInterval();

	std::vector<DOMap> vDOMap;
	std::vector<DOMap> FTab;
	std::vector<DOMap> MTab;
	std::vector<DOMap> WorkTab;

	double xmin = std::numeric_limits<double>::max();
	double xmax = std::numeric_limits<double>::min();
	double ymin = std::numeric_limits<double>::max();
	double ymax = std::numeric_limits<double>::min();
	double zmin = std::numeric_limits<double>::max();
	double zmax = std::numeric_limits<double>::min();
	double vmax = 0.0;
	double rmax = 0.0;

	double safeD;   // The safe distance of each element
	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	for (ul=0; ul<numberDO; ++ul)
	{
		vcDO.push_back(ul);

		cpdos  = pDOWorld->GetDOStatus(ul);
		cpdoml = pDOWorld->GetDOModel(cpdos->GetDOName());

		safeD
			= VEDO::dSafetyFactor
            * (   2.0 * cpdoml->GetRange()
                + dt  * (cpdos->GetVelocity()).length()
                + 0.5 * dt * dt * vFieldAcceleration.length() );

		if (cpdoml->GetScope() == "local")
		{
			xmin = std::min( xmin, cpdos->GetPosition().x() );
			xmax = std::max( xmax, cpdos->GetPosition().x() );
			ymin = std::min( ymin, cpdos->GetPosition().y() );
			ymax = std::max( ymax, cpdos->GetPosition().y() );
			zmin = std::min( zmin, cpdos->GetPosition().z() );
			zmax = std::max( zmax, cpdos->GetPosition().z() );
			vmax = std::max( vmax, cpdos->GetVelocity().length() );
			rmax = cpdoml->GetRange();
		}

		vDOMap.push_back(DOMap(ul, cpdos, cpdoml, safeD));
	}
	// Max safe distance
	double ZoneRange
        = VEDO::dSafetyFactor
        * (2.0 * rmax + dt * vmax + 0.5 * dt * dt * vFieldAcceleration.length());
	// Determine how many "safety region" per direction
	int ncelx = std::ceil((xmax - xmin) / ZoneRange);
	ncelx = std::max(ncelx, 1);
	int ncely = std::ceil((ymax - ymin) / ZoneRange);
	ncely = std::max(ncely, 1);
	int ncelz = std::ceil((zmax - zmin) / ZoneRange);
	ncelz = std::max(ncelz, 1);

	#ifdef _VEDO_DEBUG
        std::cout
            << "(ncelx, necly, ncelz) = ("
            << ncelx
            << ','
            << ncely
            << ','
            << ncelz << ")\n";
	#endif   // _VEDO_DEBUG

	std::map<Trir, std::vector<DOMap>*, TrirLeY> locMap;

	std::vector<DOMap> GlobalElement;

	for (ul=0; ul<numberDO; ++ul)
	{
		if (vDOMap[ul].cpdoml()->GetScope() == "local")
		{
			NJR::Vector3d p = vDOMap[ul].cpdos()->GetPosition();
			Trir zone
				(static_cast<int>((p.x()-xmin)/ZoneRange),
				static_cast<int>((p.y()-ymin)/ZoneRange),
				static_cast<int>((p.z()-zmin)/ZoneRange)  );

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
	for (int ix=0; ix<ncelx; ++ix)
	{
		for (int iy=0; iy<ncely; ++iy)
		{
			for (int iz=0; iz<ncelz; ++iz)
			{

				//std::cout << "zone (" << ix << ',' << iy << ',' << iz << " )\n";
				iter1 = locMap.find(Trir(ix, iy, iz));
				if (iter1 == locMap.end())
				{
					continue;
				}
				std::vector<DOMap>* pvec1 = iter1->second;

				// Self and neightbor zone check
				if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz-1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz+1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix, iy-1, iz-1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix, iy-1, iz)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix, iy-1, iz+1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz-1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz+1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix-1, iy, iz-1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix-1, iy, iz)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				if ( (iter2 = locMap.find(Trir(ix, iy, iz-1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}

				// Self and self detection
				BuildIactTab(*pvec1);

				// Self and fix element detection
				BuildIactTab(*pvec1, GlobalElement);

				if ( (iter2 = locMap.find(Trir(ix+1, iy, iz-1)))
					!= locMap.end()                               )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}
	}
/*
	std::cout
		<< "(Iact size = "
		<< (unsigned int)(NBSConsultant::IactPairTab.size())
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
		<< (unsigned int)(IactPairTab.size())
		<< ")\n";
*/
	#ifdef _VEDO_DEBUG
        std::cout
            << "(Cutting Range = "
            << ZoneRange
            << ") (Uniqued Iact size = "
            << (unsigned int)(IactPairTab.size())
            << ")\n";
	#endif   // _VEDO_DEBUG

	// Clear the LocMap data
	for (iter1=locMap.begin(); iter1!=locMap.end(); ++iter1)
	{
		delete iter1->second;
	}

	//NJR::RunTime("Leap Reset Accomplished");

	time_t endtime;
	time(&endtime);
	timeReset += endtime - starttime;

	return true;
};

void NBSConsultant::BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2)
{
	for (unsigned long i=0; i<v1.size(); ++i)
	{
		for (unsigned long j=0; j<v2.size(); ++j)
		{
			const IactModel* cpiactml
				= pDOWorld->GetIactModel
					(v1[i].cpdoml()->GetDOGroup(),
					v2[j].cpdoml()->GetDOGroup()  );
			if (cpiactml == 0)
			{
				continue;
			}

			if (  DOMap::CalDistance(v1[i], v2[j])
				> (v1[i].SafeLength() + v2[j].SafeLength()) )
			{
				continue;
			}

			if (v1[i].id() < v2[j].id())
			{
				NBSConsultant::IactPairTab.push_back
					(std::make_pair(v1[i].id(), v2[j].id()));
			}
			else
			{
				NBSConsultant::IactPairTab.push_back
					(std::make_pair(v2[j].id(), v1[i].id()));
			}
		}
	}
}

void NBSConsultant::BuildIactTab(std::vector<DOMap>& v)
{
	for (unsigned long i=0; i<v.size(); ++i)
	{
		for (unsigned long j=i+1; j<v.size(); ++j)
		{
			const IactModel* cpiactml
				= pDOWorld->GetIactModel
					(v[i].cpdoml()->GetDOGroup(),
					v[j].cpdoml()->GetDOGroup()  );
			if (cpiactml == 0)
			{
				continue;
			}

			if (  DOMap::CalDistance(v[i], v[j])
				> (v[i].SafeLength() + v[j].SafeLength()) )
			{
				continue;
			}

			if (v[i].id() < v[j].id())
			{
				NBSConsultant::IactPairTab.push_back
					(std::make_pair(v[i].id(), v[j].id()));
			}
			else
			{
				NBSConsultant::IactPairTab.push_back
					(std::make_pair(v[j].id(), v[i].id()));
			}
		}
	}
}

void NBSConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	CollectUserDefinedData(cIact);

	this->pIRTbl->Clear();
	for (unsigned int i=0; i<cIact.size(); ++i)
	{
		const Interaction* pInt = cIact.GetInteraction(i);
		if ( pInt->IsActive() )
		{
			this->pIRTbl->PushRecord
				(IactPairTab[i].first,
				IactPairTab[i].second,
				*pInt->GetImpactStatus() );
		}
	}

	std::cout
		<< "At RebuildIactRecordTab: size  = "
		<< pIRTbl->GetTabSize()
		<< '\n';
	std::cout
		<< "At the instant, pDOWorld->time = "
		<< pDOWorld->GetSystemParameter()->GetTimeCurrent()
		<< '\n';
};

};   // namespace VEDO
