#ifndef _NBS_PARALLEL_CONSULTANT_H
#define _NBS_PARALLEL_CONSULTANT_H

#include <mpi.h>
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

// The two undef prevents confliction min and max in std namespace conflicts with the macros in <mpi.h>
#undef max
#undef min

namespace vedo
{

class NBSParallelConsultant: public Consultant
{

public:

	NBSParallelConsultant
		(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, _uint_t ulwrite, _uint_t ulupdate);

	virtual ~NBSParallelConsultant();

	virtual _uint_t GetDONum() const;

	virtual _uint_t GetIactNum() const;

	virtual _uint_t GetDO(_uint_t) const;

	virtual _uint_t GetIactMaster(_uint_t) const;

	virtual _uint_t GetIactSlave(_uint_t) const;

	virtual void SyncDOContainer(DOContainer& cDO);

	void WriteDOSubWorld(DOContainer&) const;

	inline bool ISReset()
	{
		return ((ulRoundCount % culUpIact) == 0);
	}

	virtual bool Reset();

	virtual bool CleanUp(DOContainer& cDO, IactContainer& cIact);

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	inline _uint_t getRank()
	{
		return rank;
	}

	const ImpactStatus* RetrieveImpactStatus(_uint_t lcMaster, _uint_t lcSlave) const;

	static const _uint_t MASTER = 0;

	virtual void SyncWorld(DOContainer& vDO);

//	void MasterSyncWorld(DOContainer& vDO, IactContainer& cIact);

//	void SlaveSyncWorld(DOContainer& vDO, IactContainer& cIact);

	virtual void RebuildIactRecordTab(IactContainer& cIact);

protected:

	virtual void CollectUserDefinedData(IactContainer&);

private:

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);

	void ConstructDOandOverLapTab();

	void DistributeIactPair();

	// The global ID of "Discrete Element (Object)"
	std::vector<_uint_t> DOTab;

	std::vector< std::vector<_uint_t> > overlapTab;

	std::vector<_uint_t> G2LTab;

	//RDManager* pRDManager;

	// The space for transfer Impact
	mutable std::vector< std::pair<_uint_t, _float_t*> > sendBufVec;

	mutable std::vector< std::pair<_uint_t, _float_t*> > recvBufVec;

	// The space for synchorize the world
	mutable std::vector< std::pair< _uint_t, std::pair<_uint_t*, _float_t*> > > syncSendBufVec;

	mutable std::vector< std::pair< _uint_t, std::pair<_uint_t*, _float_t*> > > syncRecvBufVec;

	std::vector<_float_t> VecHitRatio;

//	template<class TrirComp> void subReset(TrirComp Comp);
	template<class TrirComp>
	void subReset
		(TrirComp,
		 _float_t, _float_t, _float_t, _float_t, _float_t, _float_t, _float_t, _float_t);
};

template<class TrirComp>
//void NBSParallelConsultant::subReset(TrirComp Comp)
void NBSParallelConsultant::subReset
	(TrirComp Comp,
	 _float_t SphereXMin, _float_t SphereXMax, _float_t SphereYMin, _float_t SphereYMax,
	 _float_t SphereZMin, _float_t SphereZMax, _float_t SphereVMax, _float_t SphereRMax )
{
	vcDO.clear();
	IactPairTab.clear();

	const SystemParameter* csp = Consultant::pDOWorld->GetSystemParameter();

	_uint_t   numberDO           = csp->GetDONumber();
	vedo::Vector3df vFieldAcceleration = csp->GetFieldAcceleration();
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
	// Aries: The following loop can be merged with the next loop
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
	_uint_t ncelx = std::max((_uint_t)(std::ceil((SphereXMax - SphereXMin) / ZoneRange)), (_uint_t)1);
	_uint_t ncely = std::max((_uint_t)(std::ceil((SphereYMax - SphereYMin) / ZoneRange)), (_uint_t)1);
	_uint_t ncelz = std::max((_uint_t)(std::ceil((SphereZMax - SphereZMin) / ZoneRange)), (_uint_t)1);

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

	vedo::Vector3df pos;
	// Aries: The following loop can be merged with the least loop
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

	_uint_t ct = 0;   // Counter

	// The number of "safety region" which distributed to each processor
	_uint_t avgZone = locMap.size() / NP;

	typename std::map<Trir, std::vector<DOMap>*, TrirComp>::iterator iter1, iter2;
	_uint_t ix, iy, iz;

	std::vector<DOMap>* pvec1 = 0;
	for (iter1=locMap.begin(); iter1!=locMap.end(); ++iter1, ++ct)
	{
		if (rank == (NP-1))
		{
			if (ct < ((NP-1)*avgZone))
			{
				continue;
			}
		}
		else
		{
			if ( !( (ct >= rank*avgZone) && (ct < ((rank+1)*avgZone)) ) )
			{
				continue;
			}
		}

/*
		if ( (rank!=(NP-1)) && !((ct>=rank*avgZone) && (ct<(rank+1)*avgZone)) )
		{
			continue;
		}

		if ( (rank==(NP-1)) && !(ct>=(NP-1)*avgZone) )
		{
			continue;
		}
*/

		ix = iter1->first.xIdx;
		iy = iter1->first.yIdx;
		iz = iter1->first.zIdx;

//		std::cout << "[" << rank << "] ct = " << ct << " ; zone = (" << ix << ',' << iy << ',' << iz << ')' << std::endl;

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
			bool duplicated = ((pbc_xID == ix+1) || (pbc_yID == iy+1) || (pbc_zID == iz+1));
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix-1, iy-1, iz  ))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if(has_pbc && (ix==0 || iy==0))
		{
			_uint_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = iz;
			bool duplicated = ((pbc_xID == ix+1) || (pbc_yID == iy+1));
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
			bool duplicated = ((pbc_xID == ix+1) || (pbc_yID == iy+1) || (pbc_zID == iz-1));
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
			bool duplicated = ((pbc_yID == iy+1) || (pbc_zID == iz+1));
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix  , iy-1, iz  ))) != locMap.end())
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
			BuildIactTab(*pvec1 , *(iter2->second));
		}
		else if (has_pbc && (iy==0 || iz==maxZoneID_z))
		{
			_uint_t pbc_xID = ix;
			_uint_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			_uint_t pbc_zID = pbc_z ? 0 : iz+1;
			bool duplicated = ((pbc_yID == iy+1) || (pbc_zID == iz-1));
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
		else if (has_pbc && (ix==maxZoneID_x || iy==0 || iz==0))
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
			bool duplicated = ((pbc_xID == ix-1) || (pbc_yID == iy+1));
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
			bool duplicated = ((pbc_xID == ix-1) || (pbc_yID == iy+1) || (pbc_zID == iz-1));
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
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix-1, iy  , iz  ))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==0) )
		{
			_uint_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			_uint_t pbc_yID = iy;
			_uint_t pbc_zID = iz;
			bool duplicated = (pbc_xID == ix+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ((iter2 = locMap.find(Trir(ix  , iy  , iz-1))) != locMap.end())
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if (has_pbc && (iz==0))
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
			bool duplicated = ((pbc_xID == ix-1) || (pbc_zID == iz+1));
			if (!duplicated)
			{
				if ((iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID))) != locMap.end())
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
	_uint_t* pNumIactPair     = new _uint_t[NP];
	_uint_t  NumLocalIactPair = (_uint_t)IactPairTab.size();

	#ifdef _INT32
		MPI_Allgather(&NumLocalIactPair, 1, MPI_UNSIGNED, pNumIactPair, 1, MPI_UNSIGNED, MPI_COMM_WORLD);
	#else
		#ifdef _INT64
			MPI_Allgather(&NumLocalIactPair, 1, MPI_UNSIGNED_LONG_LONG, pNumIactPair, 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#else
			MPI_Allgather(&NumLocalIactPair, 1, MPI_UNSIGNED_LONG_LONG, pNumIactPair, 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#endif   // _INT64
	#endif   // _INT32

	_uint_t NumAllIactPair = 0;
	_int_t* recvcounts     = new _int_t[NP];
	for (_uint_t i=0; i<NP; ++i)
	{
		NumAllIactPair +=         pNumIactPair[i];
		recvcounts[i]   = (_int_t)pNumIactPair[i] * 2;
	}

	_int_t* displs = new _int_t[NP];
	displs[0] = 0;
	for (_uint_t i=1; i<NP; ++i)
	{
		displs[i] = displs[i-1] + recvcounts[i-1];
	}

	_uint_t* pLocalIactPair = new _uint_t[NumLocalIactPair * 2];
	for (_uint_t i=0; i<NumLocalIactPair; ++i)
	{
		pLocalIactPair[2*i]   = IactPairTab[i].first;
		pLocalIactPair[2*i+1] = IactPairTab[i].second ;
	}

	_uint_t* pAllIactPair = new _uint_t[NumAllIactPair * 2];

	// MPI_Allgatherv only support "integer" type
	#ifdef _INT32
		MPI_Allgatherv(pLocalIactPair, 2*(_int_t)NumLocalIactPair, MPI_UNSIGNED, pAllIactPair, recvcounts, displs, MPI_UNSIGNED, MPI_COMM_WORLD);
	#else
		#ifdef _INT64
			MPI_Allgatherv(pLocalIactPair, 2*(_int_t)NumLocalIactPair, MPI_UNSIGNED_LONG_LONG, pAllIactPair, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#else
			MPI_Allgatherv(pLocalIactPair, 2*(_int_t)NumLocalIactPair, MPI_UNSIGNED_LONG_LONG, pAllIactPair, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#endif   // _INT64
	#endif   // _INT32

	this->IactPairTab.clear();
	for (_uint_t i=0; i<NumAllIactPair; ++i)
	{
		IactPairTab.push_back(std::make_pair(pAllIactPair[2*i], pAllIactPair[2*i+1]));
	}

	// Clear the LocMap data
	for (iter1 = locMap.begin(); iter1!=locMap.end() ; ++iter1)
	{
		delete iter1->second;
	}

	#ifdef _DEBUG
		if (rank == MASTER)
		{
			std::cout << "[" << rank << "] " << "Size of total IactTab = " << IactPairTab.size() << std::endl;
		}
	#endif   // _DEBUG

	/*
	stable_sort(IactPairTab.begin(),IactPairTab.end(),Y_Comp(vDOMap));
	_float_t MaxSpan=std::max(xSpan,std::max(ySpan,zSpan));
	if (MaxSpan == ySpan)
	{
		stable_sort( IactPairTab.begin(),IactPairTab.end(),Y_Comp(vDOMap) );
		#ifdef _DEBUG
			if (rank == MASTER)
				std::cout << "Partition at Y-dimension" << std::endl;
		#endif   // _DEBUG
	}
	else if (MaxSpan == xSpan)
	{
		stable_sort( IactPairTab.begin(),IactPairTab.end(),X_Comp(vDOMap) );
		#ifdef _DEBUG
			if (rank == MASTER)
				std::cout << "Partition at X-dimension" << std::endl;
		#endif   // _DEBUG
	}
	else
	{
		stable_sort( IactPairTab.begin(),IactPairTab.end(),Z_Comp(vDOMap) );
		#ifdef _DEBUG
			if (rank == MASTER)
				std::cout << "Partition at Z-dimension" << std::endl;
		#endif   // _DEBUG
	}
	*/

	std::time(&starttime);

	DistributeIactPair();

	std::time(&endtime);
	timePartitioning += (endtime - starttime);

	ConstructDOandOverLapTab();

	#ifdef _DEBUG
		std::cout
			<< "[" << rank << "] "
			<< "Size of Cutting Zone (ncelx, necly, ncelz) = ("
			<< ncelx << ", " << ncely << ", " << ncelz << ")" << std::endl
			<< "[" << rank << "] "
			<< "Size of Interaction = "
			<< (_uint_t)NBSParallelConsultant::IactPairTab.size() << std::endl
			<< "[" << rank << "] "
			<< "Size of Overlap Table = (" << overlapTab[0].size();

		for (_uint_t i=1; i<NP; ++i)
		{
			std::cout << ", " << overlapTab[i].size();
		}

		std::cout << ")" << std::endl;
	#endif   // _DEBUG

	delete[] pNumIactPair;
	delete[] pAllIactPair;
	delete[] pLocalIactPair;
	delete[] recvcounts;
	delete[] displs;
};

}   // namespace vedo

#endif // _NBS_PARALLEL_CONSULTANT_H
