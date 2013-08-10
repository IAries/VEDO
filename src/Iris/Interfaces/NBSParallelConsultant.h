#ifndef _NBS_PARALLEL_CONSULTANT_H
#define _NBS_PARALLEL_CONSULTANT_H

#include <mpi.h>
#include <Framework/Interfaces/Consultant.h>
#include <Common/Interfaces/NBSConsultant.h>
//#include <Iris/Interfaces/MPIAssistant.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iterator>
#include <limits>
#include <map>
#include <utility>
#include <vector>

/******************************************************************************
 * The two undef prevents confliction min and max in std namespace conflicts
 * with the macros in <mpi.h>
 ******************************************************************************/
#undef max
#undef min

namespace vedo
{

class NBSParallelConsultant : public Consultant
{

public:

	NBSParallelConsultant
		(DOWorld* DOWorld,
		IactRecordTab* pIactRecordTab,
		char filename[],
		unsigned long ulwrite ,
		unsigned long ulupdate        );

	~NBSParallelConsultant();

	virtual unsigned long GetDONum() const;

	virtual unsigned long GetIactNum() const;

	virtual unsigned long GetDO(unsigned long i) const;

	virtual unsigned long GetIactMaster (unsigned long i) const;

	virtual unsigned long GetIactSlave (unsigned long i) const;

	virtual void SyncDOContainer(DOContainer& cDO);

	void WriteDOSubWorld(DOContainer&) const ;

	inline bool ISReset()
	{
		return ((ulRoundCount % culUpIact) == 0);
	};

	virtual bool Reset();

	virtual void CleanUp(DOContainer& cDO, IactContainer& cIact);

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	inline int getRank()
	{
		return rank;
	};

	const ImpactStatus* RetrieveImpactStatus
		(unsigned long lcMaster, unsigned long lcSlave) const;

	static const int MASTER = 0;

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
	std::vector<unsigned long> DOTab;

	std::vector< std::vector<unsigned long> > overlapTab;

	std::vector<unsigned long> G2LTab;

	//RDManager* pRDManager;

	// The space for transfer Impact
	mutable std::vector< std::pair<unsigned long, double*> > sendBufVec;
	mutable std::vector< std::pair<unsigned long, double*> > recvBufVec;

	// The space for synchorize the world
	mutable std::vector< std::pair< unsigned long, std::pair<unsigned long*, double*> > > syncSendBufVec;
	mutable std::vector< std::pair< unsigned long, std::pair<unsigned long*, double*> > > syncRecvBufVec;

	std::vector<double> VecHitRatio;

//	template<class TrirComp> void subReset(TrirComp Comp);
	template<class TrirComp> void subReset
		(TrirComp,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double);
};

template<class TrirComp>
//void NBSParallelConsultant::subReset(TrirComp Comp)
void NBSParallelConsultant::subReset
	(TrirComp Comp, double SphereXMin, double SphereXMax,
					double SphereYMin, double SphereYMax,
					double SphereZMin, double SphereZMax,
					double SphereVMax, double SphereRMax )
{
	vcDO.clear();
	IactPairTab.clear();

	const SystemParameter* csp = Consultant::pDOWorld->GetSystemParameter();

	unsigned long numberDO           = csp->GetDONumber();
	njr::Vector3d vFieldAcceleration = csp->GetFieldAcceleration();
	double        dt                 = culUpIact * csp->GetTimeInterval();

	std::vector<DOMap> vDOMap;
/*
	std::vector<DOMap> FTab;
	std::vector<DOMap> MTab;
	std::vector<DOMap> WorkTab;
	double xmin = std::numeric_limits<double>::max();
	double xmax = std::numeric_limits<double>::min();

	double ymin = std::numeric_limits<double>::max();
	double ymax = std::numeric_limits<double>::min();

	double zmin = std::numeric_limits<double>::max();
	double zmax = std::numeric_limits<double>::min();
*/
/******************************************************************************
 * Aries' Comment (2006/03/31)
 *
 *    If there are no spheres, is it right ?
 ******************************************************************************/
/*
	double xmin = SphereXMin;
	double xmax = SphereXMax;
	double ymin = SphereYMin;
	double ymax = SphereYMax;
	double zmin = SphereZMin;
	double zmax = SphereZMax;
	double vmax = SphereVMax;
	double rmax = SphereRMax;

	std::vector<double> vecxloc;
	std::vector<double> vecyloc;
	std::vector<double> veczloc;
*/

	double safeD;   // The safe distance of each element
	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	// Aries: The following loop can be merged with the next loop
	for (unsigned long i=0; i<numberDO; ++i)
	{
		Consultant::vcDO.push_back(i);
		cpdos  = Consultant::pDOWorld->GetDOStatus(i);
		cpdoml = Consultant::pDOWorld->GetDOModel(cpdos->GetDOName());
		safeD
			= vedo::dSafetyFactor
            * (   2.0 * cpdoml->GetRange()
                + dt  * (cpdos->GetVelocity()).length()
                + 0.5 * dt * dt * vFieldAcceleration.length() );
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

	double ZoneRange
        = vedo::dSafetyFactor
        * (2.0 * SphereRMax + dt * SphereVMax + 0.5 * dt * dt * vFieldAcceleration.length());

	// Determine how many "safety region" per direction
	int ncelx = std::ceil((SphereXMax - SphereXMin) / ZoneRange);
	ncelx = std::max(ncelx, 1);
	int ncely = std::ceil((SphereYMax - SphereYMin) / ZoneRange);
	ncely = std::max(ncely, 1);
	int ncelz = std::ceil((SphereZMax - SphereZMin) / ZoneRange);
	ncelz = std::max(ncelz, 1);

	const Boundary* pbc = &(csp->GetPeriodicBoundaryConditions());
	bool pbc_x
		=    pbc->GetSwitch(0)
		  && ((SphereXMax - SphereXMin) >= 0.5 * pbc->GetLength().x());
	bool pbc_y
		= pbc->GetSwitch(1)
		  && ((SphereYMax - SphereYMin) >= 0.5 * pbc->GetLength().y());
	bool pbc_z
		= pbc->GetSwitch(2)
		  && ((SphereZMax - SphereZMin) >= 0.5 * pbc->GetLength().z());
	bool has_pbc = pbc_x || pbc_y || pbc_z;
	int maxZoneID_x = ncelx - 1;
	int maxZoneID_y = ncely - 1;
	int maxZoneID_z = ncelz - 1;

	std::map<Trir, std::vector<DOMap>*, TrirComp> locMap;
	//std::map<Trir, std::vector<DOMap>* > locMap;
	std::vector<DOMap> GlobalElement;

	njr::Vector3d pos;
	// Aries: The following loop can be merged with the least loop
	for (unsigned long i=0; i<numberDO; ++i)
	{
		if (vDOMap[i].cpdoml()->GetScope() == "local")
		{
			pos = vDOMap[i].cpdos()->GetPosition();
			Trir zone
				(static_cast<int>((pos.x() - SphereXMin) / ZoneRange),
				 static_cast<int>((pos.y() - SphereYMin) / ZoneRange),
				 static_cast<int>((pos.z() - SphereZMin) / ZoneRange) );

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

	int ct = 0;   // Counter

	// The number of "safety region" which distributed to each processor
	int avgZone = locMap.size() / NP;

	typename std::map<Trir, std::vector<DOMap>*, TrirComp>::iterator iter1, iter2;
	int ix, iy, iz;

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

/*
		std::cout
			<< "["
			<< rank
			<< "] ct = "
			<< ct
			<< " ; zone = ("
			<< ix
			<< ','
			<< iy
			<< ','
			<< iz
			<< ')'
			<< std::endl;
*/

		pvec1 = iter1->second;
		// Self and neightbor zone check
		if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if(has_pbc && (ix==0 || iy==0 || iz==0) )
		{
			int pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated
				= (pbc_xID == ix+1) || (pbc_yID == iy+1) || (pbc_zID == iz+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz  ))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==0 || iy==0) )
		{
			int pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = iz;
			bool duplicated = (pbc_xID == ix+1) || (pbc_yID == iy+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix-1, iy-1, iz+1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==0 || iy==0 || iz==maxZoneID_z) )
		{
			int pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = pbc_z ? 0 : iz+1;
			bool duplicated
				= (pbc_xID == ix+1) || (pbc_yID == iy+1) || (pbc_zID == iz-1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix  , iy-1, iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (iy==0 || iz==0) )
		{
			int pbc_xID = ix;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_yID == iy+1) || (pbc_zID == iz+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix  , iy-1, iz  ))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (iy==0) )
		{
			int pbc_xID = ix;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = iz;
			bool duplicated = (pbc_yID == iy+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix  , iy-1, iz+1))) != locMap.end() )
		{
			BuildIactTab( *pvec1 , *(iter2->second));
		}
		else if( has_pbc && (iy==0 || iz==maxZoneID_z) )
		{
			int pbc_xID = ix;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = pbc_z ? 0 : iz+1;
			bool duplicated = (pbc_yID == iy+1) || (pbc_zID == iz-1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==maxZoneID_x || iy==0 || iz==0) )
		{
			int pbc_xID = pbc_x ? 0 : ix+1;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated
				= (pbc_xID == ix-1) || (pbc_yID == iy+1) || (pbc_zID == iz+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz  ))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==maxZoneID_x || iy==0) )
		{
			int pbc_xID = pbc_x ? 0 : ix+1;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = iz;
			bool duplicated = (pbc_xID == ix-1) || (pbc_yID == iy+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix+1, iy-1, iz+1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==maxZoneID_x || iy==0 || iz==maxZoneID_z ) )
		{
			int pbc_xID = pbc_x ? 0 : ix+1;
			int pbc_yID = pbc_y ? maxZoneID_y : iy-1;
			int pbc_zID = pbc_z ? 0 : iz+1;
			bool duplicated = (pbc_xID == ix-1) || (pbc_yID == iy+1) || (pbc_zID == iz-1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix-1, iy  , iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==0 || iz==0) )
		{
			int pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			int pbc_yID = iy;
			int pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_xID == ix+1) || (pbc_zID == iz+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
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
			int pbc_xID = pbc_x ? maxZoneID_x : ix-1;
			int pbc_yID = iy;
			int pbc_zID = iz;
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

		if ( (iter2 = locMap.find(Trir(ix  , iy  , iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (iz==0) )
		{
			int pbc_xID = ix;
			int pbc_yID = iy;
			int pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_zID == iz+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
				{
					BuildIactTab(*pvec1, *(iter2->second));
				}
			}
		}

		if ( (iter2 = locMap.find(Trir(ix+1, iy  , iz-1))) != locMap.end() )
		{
			BuildIactTab(*pvec1, *(iter2->second));
		}
		else if( has_pbc && (ix==maxZoneID_x || iz==0) )
		{
			int pbc_xID = pbc_x ? 0 : ix+1;
			int pbc_yID = iy;
			int pbc_zID = pbc_z ? maxZoneID_z : iz-1;
			bool duplicated = (pbc_xID == ix-1) || (pbc_zID == iz+1);
			if(!duplicated)
			{
				if(   (iter2 = locMap.find(Trir(pbc_xID, pbc_yID, pbc_zID)))
					 != locMap.end()                                          )
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
	unsigned int* pNumIactPair     = new unsigned int[NP];
	unsigned int  NumLocalIactPair = IactPairTab.size();

	MPI_Allgather( &NumLocalIactPair, 1, MPI_UNSIGNED,
					pNumIactPair    , 1, MPI_UNSIGNED, MPI_COMM_WORLD);

	unsigned int NumAllIactPair = 0;
	int* recvcounts = new int[NP];
	for (unsigned int i=0; i<NP; ++i)
	{
		NumAllIactPair += pNumIactPair[i];
		recvcounts[i]   = pNumIactPair[i] * 2;
	}

	int* displs = new int[NP];
	displs[0] = 0;
	for (unsigned int i=1; i<NP; ++i)
	{
		displs[i] = displs[i-1] + recvcounts[i-1];
	}

	unsigned long* pLocalIactPair = new unsigned long[NumLocalIactPair * 2];
	for (unsigned int i=0; i<NumLocalIactPair; ++i)
	{
		pLocalIactPair[2*i]   = IactPairTab[i].first;
		pLocalIactPair[2*i+1] = IactPairTab[i].second ;
	}

	unsigned long* pAllIactPair = new unsigned long[NumAllIactPair * 2];

	MPI_Allgatherv( pLocalIactPair, 2*NumLocalIactPair, MPI_UNSIGNED_LONG,
					pAllIactPair  , recvcounts, displs, MPI_UNSIGNED_LONG,
					MPI_COMM_WORLD);

	this->IactPairTab.clear();
	for (unsigned int i=0; i<NumAllIactPair; ++i)
	{
		IactPairTab.push_back
			(std::make_pair(pAllIactPair[2*i], pAllIactPair[2*i+1]));
	}

	// Clear the LocMap data
	for (iter1 = locMap.begin(); iter1!=locMap.end() ; ++iter1)
	{
		delete iter1->second;
	}

	#ifdef _VEDO_DEBUG
		if (rank == MASTER)
		{
			std::cout
				<< "[" << rank << "] "
				<< "Size of total IactTab = "
				<< IactPairTab.size()
				<< std::endl;
		}
	#endif   // _VEDO_DEBUG

	/*
	stable_sort( IactPairTab.begin(),IactPairTab.end(),Y_Comp(vDOMap) );
	double MaxSpan=std::max(xSpan,std::max(ySpan,zSpan));
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
			<< (unsigned int)NBSParallelConsultant::IactPairTab.size() << std::endl
			<< "[" << rank << "] "
			<< "Size of Overlap Table = (" << overlapTab[0].size();

		for (unsigned int i=1; i<NP; ++i)
			std::cout << ", " << overlapTab[i].size();

		std::cout << ")" << std::endl;
	#endif   // _VEDO_DEBUG


	delete[] pNumIactPair;
	delete[] pAllIactPair;
	delete[] pLocalIactPair;
	delete[] recvcounts;
	delete[] displs;
};

};   // namespace vedo

#endif // _NBS_PARALLEL_CONSULTANT_H
