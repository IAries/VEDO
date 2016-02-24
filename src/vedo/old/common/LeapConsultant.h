#ifndef _LEAP_CONSULTANT_H
#define _LEAP_CONSULTANT_H

#include <vedo/framework/Consultant.h>
#include <vedo/common/NBSConsultant.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iterator>
#include <limits>
#include <map>
#include <utility>
#include <vector>

namespace vedo
{

class LeapConsultant: public Consultant
{

public:

	LeapConsultant(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, _uint_t ulwrite, _uint_t ulupdate);

	virtual _uint_t GetIactNum() const;

	virtual _uint_t GetIactMaster (_uint_t i) const;

	virtual _uint_t GetIactSlave (_uint_t i) const;

	virtual bool ISReset();

	virtual bool Reset();

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	virtual void RebuildIactRecordTab(IactContainer& cIact);

	virtual void SyncWorld(DOContainer& vDO);

private:

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);

	std::vector<_uint_t> overlapTab;

	std::vector<_float_t> VecHitRatio;

//	template<class TrirComp> void subReset(TrirComp Comp);
	template<class TrirComp>
	void subReset
		(TrirComp,
		 _float_t, _float_t, _float_t, _float_t, _float_t, _float_t, _float_t, _float_t);
};

template<class TrirComp>
//void LeapConsultant::subReset(TrirComp Comp)
void LeapConsultant::subReset
	(TrirComp Comp,
	 _float_t SphereXMin, _float_t SphereXMax,
	 _float_t SphereYMin, _float_t SphereYMax,
	 _float_t SphereZMin, _float_t SphereZMax,
	 _float_t SphereVMax, _float_t SphereRMax )
{
	vcDO.clear();
	IactPairTab.clear();

	const SystemParameter* csp = Consultant::pDOWorld->GetSystemParameter();

	_uint_t   numberDO           = csp->GetDONumber();
	Vector3df vFieldAcceleration = csp->GetFieldAcceleration();
	_float_t  dt                 = culUpIact * csp->GetTimeInterval();

	std::vector<DOMap> vDOMap;
/*
	std::vector<DOMap> FTab;
	std::vector<DOMap> MTab;
	std::vector<DOMap> WorkTab;
	_float_t xmin = std::numeric_limits<_float_t>::max();
	_float_t xmax = std::numeric_limits<_float_t>::min();

	_float_t ymin = std::numeric_limits<_float_t>::max();
	_float_t ymax = std::numeric_limits<_float_t>::min();

	_float_t zmin = std::numeric_limits<_float_t>::max();
	_float_t zmax = std::numeric_limits<_float_t>::min();
*/
/******************************************************************************
 * Aries' Comment (2006/03/31)
 *
 *    If there are no spheres, is it right ?
 ******************************************************************************/
/*
	_float_t xmin = SphereXMin;
	_float_t xmax = SphereXMax;
	_float_t ymin = SphereYMin;
	_float_t ymax = SphereYMax;
	_float_t zmin = SphereZMin;
	_float_t zmax = SphereZMax;
	_float_t vmax = SphereVMax;
	_float_t rmax = SphereRMax;

	std::vector<_float_t> vecxloc;
	std::vector<_float_t> vecyloc;
	std::vector<_float_t> veczloc;
*/

	_float_t safeD;   // The safe distance of each element
	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	aries::Constants* aries_cp = aries::Constants::Instance();
	_float_t SafetyFactor = aries_cp->SafetyFactor();
	for (_uint_t i=0; i<numberDO; ++i)
	{
		Consultant::vcDO.push_back(i);
		cpdos  = Consultant::pDOWorld->GetDOStatus(i);
		cpdoml = Consultant::pDOWorld->GetDOModel(cpdos->GetDOName());
		safeD
			= SafetyFactor
            * (2.0 * cpdoml->GetRange() + dt  * (cpdos->GetVelocity()).length() + 0.5 * dt * dt * vFieldAcceleration.length());
/*
        xmin = std::min(xmin, cpdos->GetPosition().x());
        xmax = std::max(xmax, cpdos->GetPosition().x());
		ymin = std::min(ymin, cpdos->GetPosition().y());
		ymax = std::max(ymax, cpdos->GetPosition().y());
		zmin = std::min(zmin, cpdos->GetPosition().z());
		zmax = std::max(zmax, cpdos->GetPosition().z());
		vmax = std::max(vmax, cpdos->GetVelocity().length());
		rmax = cpdoml->GetShapeAttributes().sphere.radius;
*/
		vDOMap.push_back(DOMap(i, cpdos, cpdoml, safeD));
	}

	_float_t ZoneRange = SafetyFactor * (2.0 * SphereRMax + dt * SphereVMax + 0.5 * dt * dt * vFieldAcceleration.length());

	// Determine how many "safety region" per direction
	_uint_t ncelx = std::ceil((SphereXMax - SphereXMin) / ZoneRange);
	ncelx = std::max(ncelx, (_uint_t)1);
	_uint_t ncely = std::ceil((SphereYMax - SphereYMin) / ZoneRange);
	ncely = std::max(ncely, (_uint_t)1);
	_uint_t ncelz = std::ceil((SphereZMax - SphereZMin) / ZoneRange);
	ncelz = std::max(ncelz, (_uint_t)1);

	const Boundary* pbc = &(csp->GetPeriodicBoundaryConditions());
	bool pbc_x   = (pbc->GetSwitch(0) && ((SphereXMax - SphereXMin) >= 0.5 * pbc->GetLength().x()));
	bool pbc_y   = (pbc->GetSwitch(1) && ((SphereYMax - SphereYMin) >= 0.5 * pbc->GetLength().y()));
	bool pbc_z   = (pbc->GetSwitch(2) && ((SphereZMax - SphereZMin) >= 0.5 * pbc->GetLength().z()));
	bool has_pbc = (pbc_x || pbc_y || pbc_z);
	_uint_t maxZoneID_x = ncelx - 1;
	_uint_t maxZoneID_y = ncely - 1;
	_uint_t maxZoneID_z = ncelz - 1;

	std::map<Trir, std::vector<DOMap>*, TrirComp> locMap;
	//std::map<Trir, std::vector<DOMap>* > locMap;
	std::vector<DOMap> GlobalElement;

	Vector3df pos;
	for (_uint_t i=0; i<numberDO; ++i)
	{
		if (vDOMap[i].cpdoml()->GetScope() == local)
		{
			pos = vDOMap[i].cpdos()->GetPosition();
			Trir zone
				(static_cast<_uint_t>((pos.x() - SphereXMin) / ZoneRange),
				 static_cast<_uint_t>((pos.y() - SphereYMin) / ZoneRange),
				 static_cast<_uint_t>((pos.z() - SphereZMin) / ZoneRange) );

			if (locMap.find(zone) == locMap.end())
			{
				locMap.insert(std::make_pair(zone, new std::vector<DOMap>()));
			}
			locMap[zone]->push_back(vDOMap[i]);
		}
		else
		{
			GlobalElement.push_back(vDOMap[i]);
		}
	}

	typename std::map<Trir, std::vector<DOMap>*, TrirComp>::iterator iter1, iter2;
	_uint_t ix, iy, iz;

	std::vector<DOMap>* pvec1 = 0;
	//for (_uint_t ulCounter=0, iter1=locMap.begin(); iter1!=locMap.end(); ++iter1, ++ulCounter)
	for (iter1=locMap.begin(); iter1!=locMap.end(); ++iter1)
	{
		ix = iter1->first.xIdx;
		iy = iter1->first.yIdx;
		iz = iter1->first.zIdx;
		//std::cout << "ulCounter = " << ulCounter << " ; zone = (" << ix << ',' << iy << ',' << iz << ")\n";
		pvec1 = iter1->second;
		// Self and neightbor zone check
		if ((iter2 = locMap.find(Trir(ix-1, iy-1, iz-1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (ix==0 || iy==0 || iz==0))
		{
			_uint_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_xID == ix+1) || (pbc_yID == iy+1) || (pbc_zID == iz+1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix-1, iy-1, iz))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (ix==0 || iy==0))
		{
			_uint_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = iz;
			bool duplicated = (pbc_xID == ix+1) || (pbc_yID == iy+1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix-1, iy-1, iz+1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (ix==0 || iy==0 || iz==maxZoneID_z))
		{
			_uint_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = pbc_z ? 0 : iz+1;
			bool duplicated = (pbc_xID == ix+1) || (pbc_yID == iy+1) || (pbc_zID == iz-1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix  , iy-1, iz-1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (iy==0 || iz==0))
		{
			_uint_t pbc_xID = ix;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_yID == iy+1) || (pbc_zID == iz+1);
			if (!duplicated)
			{
				if((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix, iy-1, iz))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (iy==0))
		{
			_uint_t pbc_xID = ix;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = iz;
			bool duplicated = (pbc_yID == iy+1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix  , iy-1, iz+1))) != locMap.end())
		{
			BuildIactTab( *pvec1 , *(iter2->second));
		}
		else if (has_pbc && (iy==0 || iz==maxZoneID_z))
		{
			_uint_t pbc_xID = ix;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = pbc_z ? 0 : iz+1;
			bool duplicated = (pbc_yID == iy+1) || (pbc_zID == iz-1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix+1, iy-1, iz-1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==maxZoneID_x || iy==0 || iz==0) )
		{
			_uint_t pbc_xID = pbc_x ? 0 : ix+1;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_xID == ix-1) || (pbc_yID == iy+1) || (pbc_zID == iz+1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix+1, iy-1, iz  ))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (ix==maxZoneID_x || iy==0))
		{
			_uint_t pbc_xID = pbc_x ? 0 : ix+1;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = iz;
			bool duplicated = (pbc_xID == ix-1) || (pbc_yID == iy+1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix+1, iy-1, iz+1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (ix==maxZoneID_x || iy==0 || iz==maxZoneID_z))
		{
			_uint_t pbc_xID = pbc_x ? 0 : ix+1;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = pbc_z ? 0 : iz+1;
			bool duplicated = (pbc_xID == ix-1) || (pbc_yID == iy+1) || (pbc_zID == iz-1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix-1, iy  , iz-1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (ix==0 || iz==0))
		{
			_uint_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			_uint_t pbc_yID = iy;
			_uint_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_xID == ix+1) || (pbc_zID == iz+1);
			if (!duplicated)
			{
				if((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix-1, iy, iz))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (ix==0))
		{
			_uint_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			_uint_t pbc_yID = iy;
			_uint_t pbc_zID = iz;
			bool duplicated = (pbc_xID == ix+1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix, iy, iz-1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if ( has_pbc && (iz==0))
		{
			_uint_t pbc_xID = ix;
			_uint_t pbc_yID = iy;
			_uint_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_zID == iz+1);
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix+1, iy  , iz-1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (ix==maxZoneID_x || iz==0))
		{
			_uint_t pbc_xID = pbc_x ? 0 : ix+1;
			_uint_t pbc_yID = iy;
			_uint_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_xID == ix-1) || (pbc_zID == iz+1);
			if (!duplicated)
			{
				if((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		// Self and self detection
		BuildIactTab(*pvec1);

		// Self and fix element detection
		BuildIactTab(*pvec1, GlobalElement);
	}

	// For processor to exchange their detected IactPairs
/*
	_uint_t NumLocalIactPair = IactPairTab.size();
	_uint_t NumIactPair      = NumLocalIactPair;

	_uint_t NumAllIactPair = 0;
	_uint_t recvcounts;
	NumAllIactPair += NumIactPair;
	recvcounts      = NumIactPair * 2;

	_uint_t displs = 0;

	_uint_t* pLocalIactPair = new _uint_t[NumLocalIactPair * 2];
	for (_uint_t i=0; i<NumLocalIactPair; ++i)
	{
		pLocalIactPair[2*i]   = IactPairTab[i].first;
		pLocalIactPair[2*i+1] = IactPairTab[i].second ;
	}

	_uint_t* pAllIactPair = new _uint_t[NumAllIactPair * 2];

	MPI_Allgatherv( pLocalIactPair, 2*NumLocalIactPair, MPI_UNSIGNED_LONG,
					pAllIactPair  , recvcounts, displs, MPI_UNSIGNED_LONG,
					MPI_COMM_WORLD);

	this->IactPairTab.clear();
	for (_uint_t i=0; i<NumAllIactPair; ++i)
	{
		IactPairTab.push_back
			(std::make_pair(pAllIactPair[2*i], pAllIactPair[2*i+1]));
	}
*/

	// Clear the LocMap data
	for (iter1 = locMap.begin(); iter1!=locMap.end() ; ++iter1)
	{
		delete iter1->second;
	}

/*
	#ifdef _DEBUG
	//	std::cout << "Size of total IactTab = " << IactPairTab.size() << std::endl;
	#endif   // _DEBUG

	std::time(&starttime);

	DistributeIactPair();

	std::time(&endtime);
	timePartitioning += (endtime - starttime);

	ConstructDOandOverLapTab();

	#ifdef _DEBUG
		std::cout
			<< "Size of Cutting Zone (ncelx, necly, ncelz) = ("
			<< ncelx << ", " << ncely << ", " << ncelz << ")" << std::endl
			<< "Size of Interaction = "
			<< LeapConsultant::IactPairTab.size() << std::endl
			<< "Size of Overlap Table = (" << overlapTab.size() << ")" << std::endl;
	#endif   // _DEBUG

	delete[] pAllIactPair;
	delete[] pLocalIactPair;
*/
};

}   // namespace vedo


#endif // _LEAP_CONSULTANT_H
