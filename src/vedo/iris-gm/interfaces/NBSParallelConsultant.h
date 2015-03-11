#ifndef _NBS_PARALLEL_CONSULTANT_H
#define _NBS_PARALLEL_CONSULTANT_H

#include <mpi.h>
#include <vedo/framework/interfaces/Consultant.h>
#include <vedo/common/interfaces/NBSConsultant.h>
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
		(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, vedo_uint_t ulwrite, vedo_uint_t ulupdate);

	virtual ~NBSParallelConsultant();

	virtual vedo_uint_t GetDONum() const;

	virtual vedo_uint_t GetIactNum() const;

	virtual vedo_uint_t GetDO(vedo_uint_t) const;

	virtual vedo_uint_t GetIactMaster(vedo_uint_t) const;

	virtual vedo_uint_t GetIactSlave(vedo_uint_t) const;

	virtual void SyncDOContainer(DOContainer& cDO);

	void WriteDOSubWorld(DOContainer&) const;

	inline bool ISReset()
	{
		return ((ulRoundCount % culUpIact) == 0);
	}

	virtual bool Reset();

	virtual bool CleanUp(DOContainer& cDO, IactContainer& cIact);

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	inline vedo_uint_t getRank()
	{
		return rank;
	}

	const ImpactStatus* RetrieveImpactStatus(vedo_uint_t lcMaster, vedo_uint_t lcSlave) const;

	static const vedo_int_t MASTER = 0;

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
	std::vector<vedo_uint_t> DOTab;

	std::vector< std::vector<vedo_uint_t> > overlapTab;

	std::vector<vedo_uint_t> G2LTab;

	//RDManager* pRDManager;

	// The space for transfer Impact
	mutable std::vector< std::pair<vedo_uint_t, vedo_float_t*> > sendBufVec;

	mutable std::vector< std::pair<vedo_uint_t, vedo_float_t*> > recvBufVec;

	// The space for synchorize the world
	mutable std::vector< std::pair< vedo_uint_t, std::pair<vedo_uint_t*, vedo_float_t*> > > syncSendBufVec;

	mutable std::vector< std::pair< vedo_uint_t, std::pair<vedo_uint_t*, vedo_float_t*> > > syncRecvBufVec;

	std::vector<vedo_float_t> VecHitRatio;

//	template<class TrirComp> void subReset(TrirComp Comp);
	template<class TrirComp>
	void subReset
		(TrirComp,
		 vedo_float_t, vedo_float_t, vedo_float_t, vedo_float_t, vedo_float_t, vedo_float_t, vedo_float_t, vedo_float_t);
};

template<class TrirComp>
//void NBSParallelConsultant::subReset(TrirComp Comp)
void NBSParallelConsultant::subReset
	(TrirComp Comp,
	 vedo_float_t SphereXMin, vedo_float_t SphereXMax, vedo_float_t SphereYMin, vedo_float_t SphereYMax,
	 vedo_float_t SphereZMin, vedo_float_t SphereZMax, vedo_float_t SphereVMax, vedo_float_t SphereRMax )
{
	vcDO.clear();
	IactPairTab.clear();

	const SystemParameter* csp = Consultant::pDOWorld->GetSystemParameter();

	vedo_uint_t   numberDO           = csp->GetDONumber();
	njr::Vector3d vFieldAcceleration = csp->GetFieldAcceleration();
	vedo_float_t  dt                 = culUpIact * csp->GetTimeInterval();

	std::vector<DOMap> vDOMap;
/*
	std::vector<DOMap> FTab;
	std::vector<DOMap> MTab;
	std::vector<DOMap> WorkTab;
	vedo_float_t xmin = std::numeric_limits<vedo_float_t>::max();
	vedo_float_t xmax = std::numeric_limits<vedo_float_t>::min();
	vedo_float_t ymin = std::numeric_limits<vedo_float_t>::max();
	vedo_float_t ymax = std::numeric_limits<vedo_float_t>::min();
	vedo_float_t zmin = std::numeric_limits<vedo_float_t>::max();
	vedo_float_t zmax = std::numeric_limits<vedo_float_t>::min();
*/
/******************************************************************************
 * Aries' Comment (2006/03/31)
 *
 *    If there are no spheres, is it right ?
 ******************************************************************************/
/*
	vedo_float_t xmin = SphereXMin;
	vedo_float_t xmax = SphereXMax;
	vedo_float_t ymin = SphereYMin;
	vedo_float_t ymax = SphereYMax;
	vedo_float_t zmin = SphereZMin;
	vedo_float_t zmax = SphereZMax;
	vedo_float_t vmax = SphereVMax;
	vedo_float_t rmax = SphereRMax;

	std::vector<vedo_float_t> vecxloc;
	std::vector<vedo_float_t> vecyloc;
	std::vector<vedo_float_t> veczloc;
*/

	vedo_float_t safeD;   // The safe distance of each element
	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	// Aries: The following loop can be merged with the next loop
	Constants* vedo_cp = Constants::Instance();
	vedo_float_t SafetyFactor = vedo_cp->SafetyFactor();
	for (vedo_uint_t i=0; i<numberDO; ++i)
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

	vedo_float_t ZoneRange = SafetyFactor * (2.0 * SphereRMax + dt * SphereVMax + 0.5 * dt * dt * vFieldAcceleration.length());

	// Determine how many "safety region" per direction
	vedo_int_t ncelx = std::max((vedo_int_t)(std::ceil((SphereXMax - SphereXMin) / ZoneRange)), (vedo_int_t)1);
	vedo_int_t ncely = std::max((vedo_int_t)(std::ceil((SphereYMax - SphereYMin) / ZoneRange)), (vedo_int_t)1);
	vedo_int_t ncelz = std::max((vedo_int_t)(std::ceil((SphereZMax - SphereZMin) / ZoneRange)), (vedo_int_t)1);

	const Boundary* pbc = &(csp->GetPeriodicBoundaryConditions());
	bool pbc_x   = (pbc->GetSwitch(0) && ((SphereXMax - SphereXMin) >= 0.5 * pbc->GetLength().x()));
	bool pbc_y   = (pbc->GetSwitch(1) && ((SphereYMax - SphereYMin) >= 0.5 * pbc->GetLength().y()));
	bool pbc_z   = (pbc->GetSwitch(2) && ((SphereZMax - SphereZMin) >= 0.5 * pbc->GetLength().z()));
	bool has_pbc = (pbc_x || pbc_y || pbc_z);
	vedo_int_t maxZoneID_x = ncelx - 1;
	vedo_int_t maxZoneID_y = ncely - 1;
	vedo_int_t maxZoneID_z = ncelz - 1;

	std::map<Trir, std::vector<DOMap>*, TrirComp> locMap;
	//std::map<Trir, std::vector<DOMap>* > locMap;
	std::vector<DOMap> GlobalElement;

	njr::Vector3d pos;
	// Aries: The following loop can be merged with the least loop
	for (vedo_uint_t i=0; i<numberDO; ++i)
	{
		if (vDOMap[i].cpdoml()->GetScope() == local)
		{
			pos = vDOMap[i].cpdos()->GetPosition();
			Trir zone
				(static_cast<vedo_int_t>((pos.x() - SphereXMin) / ZoneRange),
				 static_cast<vedo_int_t>((pos.y() - SphereYMin) / ZoneRange),
				 static_cast<vedo_int_t>((pos.z() - SphereZMin) / ZoneRange) );

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

	vedo_uint_t ct = 0;   // Counter

	// The number of "safety region" which distributed to each processor
	vedo_int_t avgZone = locMap.size() / NP;

	typename std::map<Trir, std::vector<DOMap>*, TrirComp>::iterator iter1, iter2;
	vedo_int_t ix, iy, iz;

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
			vedo_int_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
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
			vedo_int_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = iz;
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
			vedo_int_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = pbc_z ? 0 : iz+1;
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
			vedo_int_t pbc_xID = ix;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
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
			vedo_int_t pbc_xID = ix;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = iz;
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
			vedo_int_t pbc_xID = ix;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = pbc_z ? 0 : iz+1;
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
			vedo_int_t pbc_xID = pbc_x ? 0 : ix+1;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
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
			vedo_int_t pbc_xID = pbc_x ? 0 : ix+1;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = iz;
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
			vedo_int_t pbc_xID = pbc_x ? 0 : ix+1;
			vedo_int_t pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			vedo_int_t pbc_zID = pbc_z ? 0 : iz+1;
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
			vedo_int_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			vedo_int_t pbc_yID = iy;
			vedo_int_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
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
			vedo_int_t pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			vedo_int_t pbc_yID = iy;
			vedo_int_t pbc_zID = iz;
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
			vedo_int_t pbc_xID = ix;
			vedo_int_t pbc_yID = iy;
			vedo_int_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
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
			vedo_int_t pbc_xID = pbc_x ? 0 : ix+1;
			vedo_int_t pbc_yID = iy;
			vedo_int_t pbc_zID = pbc_z ? maxZoneID_z : iz-1;
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
	vedo_uint_t* pNumIactPair     = new vedo_uint_t[NP];
	vedo_uint_t  NumLocalIactPair = (vedo_uint_t)IactPairTab.size();

	#ifdef _VEDO_INT32
		MPI_Allgather(&NumLocalIactPair, 1, MPI_UNSIGNED, pNumIactPair, 1, MPI_UNSIGNED, MPI_COMM_WORLD);
	#else
		#ifdef _VEDO_INT64
			MPI_Allgather(&NumLocalIactPair, 1, MPI_UNSIGNED_LONG_LONG, pNumIactPair, 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#else
			MPI_Allgather(&NumLocalIactPair, 1, MPI_UNSIGNED_LONG_LONG, pNumIactPair, 1, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#endif   // _VEDO_INT64
	#endif   // _VEDO_INT32

	vedo_uint_t NumAllIactPair = 0;
	vedo_int_t* recvcounts = new vedo_int_t[NP];
	for (vedo_uint_t i=0; i<NP; ++i)
	{
		NumAllIactPair += pNumIactPair[i];
		recvcounts[i]   = pNumIactPair[i] * 2;
	}

	vedo_int_t* displs = new vedo_int_t[NP];
	displs[0] = 0;
	for (vedo_uint_t i=1; i<NP; ++i)
	{
		displs[i] = displs[i-1] + recvcounts[i-1];
	}

	vedo_uint_t* pLocalIactPair = new vedo_uint_t[NumLocalIactPair * 2];
	for (vedo_uint_t i=0; i<NumLocalIactPair; ++i)
	{
		pLocalIactPair[2*i]   = IactPairTab[i].first;
		pLocalIactPair[2*i+1] = IactPairTab[i].second ;
	}

	vedo_uint_t* pAllIactPair = new vedo_uint_t[NumAllIactPair * 2];

	// MPI_Allgatherv only support "integer" type
	#ifdef _VEDO_INT32
		MPI_Allgatherv(pLocalIactPair, 2*NumLocalIactPair, MPI_UNSIGNED, pAllIactPair, recvcounts, displs, MPI_UNSIGNED, MPI_COMM_WORLD);
	#else
		#ifdef _VEDO_INT64
			MPI_Allgatherv(pLocalIactPair, 2*NumLocalIactPair, MPI_UNSIGNED_LONG_LONG, pAllIactPair, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#else
			MPI_Allgatherv(pLocalIactPair, 2*NumLocalIactPair, MPI_UNSIGNED_LONG_LONG, pAllIactPair, recvcounts, displs, MPI_UNSIGNED_LONG_LONG, MPI_COMM_WORLD);
		#endif   // _VEDO_INT64
	#endif   // _VEDO_INT32

	this->IactPairTab.clear();
	for (vedo_uint_t i=0; i<NumAllIactPair; ++i)
	{
		IactPairTab.push_back(std::make_pair(pAllIactPair[2*i], pAllIactPair[2*i+1]));
	}

	// Clear the LocMap data
	for (iter1 = locMap.begin(); iter1!=locMap.end() ; ++iter1)
	{
		delete iter1->second;
	}

	#ifdef _VEDO_DEBUG
		if (rank == MASTER)
		{
			std::cout << "[" << rank << "] " << "Size of total IactTab = " << IactPairTab.size() << std::endl;
		}
	#endif   // _VEDO_DEBUG

	/*
	stable_sort(IactPairTab.begin(),IactPairTab.end(),Y_Comp(vDOMap));
	vedo_float_t MaxSpan=std::max(xSpan,std::max(ySpan,zSpan));
	if (MaxSpan == ySpan)
	{
		stable_sort( IactPairTab.begin(),IactPairTab.end(),Y_Comp(vDOMap) );
		#ifdef _VEDO_DEBUG
			if (rank == MASTER)
				std::cout << "Partition at Y-dimension" << std::endl;
		#endif   // _VEDO_DEBUG
	}
	else if (MaxSpan == xSpan)
	{
		stable_sort( IactPairTab.begin(),IactPairTab.end(),X_Comp(vDOMap) );
		#ifdef _VEDO_DEBUG
			if (rank == MASTER)
				std::cout << "Partition at X-dimension" << std::endl;
		#endif   // _VEDO_DEBUG
	}
	else
	{
		stable_sort( IactPairTab.begin(),IactPairTab.end(),Z_Comp(vDOMap) );
		#ifdef _VEDO_DEBUG
			if (rank == MASTER)
				std::cout << "Partition at Z-dimension" << std::endl;
		#endif   // _VEDO_DEBUG
	}
	*/

	std::time(&starttime);

	DistributeIactPair();

	std::time(&endtime);
	timePartitioning += (endtime - starttime);

	ConstructDOandOverLapTab();

	#ifdef _VEDO_DEBUG
		std::cout
			<< "[" << rank << "] "
			<< "Size of Cutting Zone (ncelx, necly, ncelz) = ("
			<< ncelx << ", " << ncely << ", " << ncelz << ")" << std::endl
			<< "[" << rank << "] "
			<< "Size of Interaction = "
			<< (vedo_uint_t)NBSParallelConsultant::IactPairTab.size() << std::endl
			<< "[" << rank << "] "
			<< "Size of Overlap Table = (" << overlapTab[0].size();

		for (vedo_uint_t i=1; i<NP; ++i)
		{
			std::cout << ", " << overlapTab[i].size();
		}

		std::cout << ")" << std::endl;
	#endif   // _VEDO_DEBUG

	delete[] pNumIactPair;
	delete[] pAllIactPair;
	delete[] pLocalIactPair;
	delete[] recvcounts;
	delete[] displs;
};

}   // namespace vedo

#endif // _NBS_PARALLEL_CONSULTANT_H
