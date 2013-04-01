#ifndef _LEAP_CONSULTANT2_H
#define _LEAP_CONSULTANT2_H

#include <FrameWork/Interfaces/Consultant.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iterator>
#include <limits>
#include <map>
#include <utility>
#include <vector>

class LeapConsultant2 : public Consultant
{

public:

	LeapConsultant2
		(DOWorld* DOWorld,
		IactRecordTab* pIactRecordTab,
		char filename[],
		unsigned long ulwrite ,
		unsigned long ulupdate        );

	~LeapConsultant2()
	{
	};

	virtual unsigned long GetIactNum() const;

	virtual unsigned long GetIactMaster (unsigned long i) const;

	virtual unsigned long GetIactSlave (unsigned long i) const;

	virtual bool ISReset();

	virtual bool Reset();

	virtual bool NextStep(DOContainer& cDO, IactContainer& cIact);

	virtual void RebuildIactRecordTab(IactContainer& cIact);

	virtual void SyncWorld(DOContainer& vDO);

private:

	virtual void BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2);

	virtual void BuildIactTab(std::vector<DOMap>& v);

	std::vector<unsigned long> overlapTab;

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
//void LeapConsultant2::subReset(TrirComp Comp)
void LeapConsultant2::subReset
	(TrirComp Comp, double SphereXMin, double SphereXMax,
					double SphereYMin, double SphereYMax,
					double SphereZMin, double SphereZMax,
					double SphereVMax, double SphereRMax )
{
	vcDO.clear();
	IactPairTab.clear();

	const SystemParameter* csp = Consultant::pDOWorld->GetSystemParameter();

	unsigned long numberDO     = csp->GetDONumber();
	NJRvector3d   vFieldForce  = csp->GetFieldForce();
	double        dt           = culUpIact * csp->GetTimeInterval();

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
	for (unsigned long i=0; i<numberDO; ++i)
	{
		Consultant::vcDO.push_back(i);
		cpdos  = Consultant::pDOWorld->GetDOStatus(i);
		cpdoml = Consultant::pDOWorld->GetDOModel(cpdos->GetDOName());
/******************************************************************************
 * Aries' Comment (2006/03/31)
 *
 *    The safety distance contains the influence of the radius, velocity, and
 * acceleration. However, the safety vector of sphere should be 1.1 ?
 ******************************************************************************/
		// «H¼ý
		switch(cpdoml->GetShapeType())
		{
			case QuasiPlate:
				safeD
					= 1.1 * (0.5 * cpdoml->GetShapeAttributes().quasiplate.height
						     + (cpdos->GetVelocity()).length() * dt              )
					+ 0.5 * dt * dt * vFieldForce.length();
				break;
			case QuasiCylinder:
				safeD
					= 1.1 * (0.5 * cpdoml->GetShapeAttributes().quasicylinder.radius
						     + (cpdos->GetVelocity()).length() * dt                 )
					+ 0.5 * dt * dt * vFieldForce.length();
				break;
			default:
				safeD
					= 1.1 * (cpdoml->GetRange() + (cpdos->GetVelocity()).length() * dt)
					+ 0.5 * dt * dt * vFieldForce.length();
		}
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

	// The maximal safe distance, Safety factor = 1.1 (1.1*2.0=2.2)
	double ZoneRange
		= SphereVMax * dt
		+ 2.2 * SphereRMax + 0.5 * dt * dt * vFieldForce.length();

	// Determine how many "safety region" per direction
	int ncelx = std::ceil((SphereXMax - SphereXMin) / ZoneRange);
	ncelx = std::max(ncelx, 1);
	int ncely = std::ceil((SphereYMax - SphereYMin) / ZoneRange);
	ncely = std::max(ncely, 1);
	int ncelz = std::ceil((SphereZMax - SphereZMin) / ZoneRange);
	ncelz = std::max(ncelz, 1);

	const Boundary* pbc = &(csp->GetPeriodicBoundaryConditions());
	bool pbc_x
		= pbc->GetSwitch(0)
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

	NJRvector3d pos;
	for (unsigned long i=0; i<numberDO; ++i)
	{
		if (vDOMap[i].cpdoml()->GetScope() == "local")
		{
			pos = vDOMap[i].cpdos()->GetPosition();
/******************************************************************************
 * Aries' Comment (2006/03/31)
 *
 *    What's the class of "Trir" ?
 ******************************************************************************/
			Trir zone
				(static_cast<int>((pos.x() - SphereXMin) / ZoneRange),
				 static_cast<int>((pos.y() - SphereYMin) / ZoneRange),
				 static_cast<int>((pos.z() - SphereZMin) / ZoneRange) );

			if (locMap.find(zone) == locMap.end())
			{
				locMap.insert(make_pair(zone, new std::vector<DOMap>()));
			}
			locMap[zone]->push_back(vDOMap[i]);
		}
		else
		{
			GlobalElement.push_back(vDOMap[i]);
		}
	}

	typename std::map<Trir, std::vector<DOMap>*, TrirComp>::iterator iter1, iter2;
	int ix, iy, iz;

	std::vector<DOMap>* pvec1 = 0;
	//for (unsigned long ulCounter=0, iter1=locMap.begin(); iter1!=locMap.end(); ++iter1, ++ulCounter)
	for (iter1=locMap.begin(); iter1!=locMap.end(); ++iter1)
	{
		ix = iter1->first.xIdx;
		iy = iter1->first.yIdx;
		iz = iter1->first.zIdx;
		//std::cout << "ulCounter = " << ulCounter << " ; zone = (" << ix << ',' << iy << ',' << iz << ")\n";
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
/*
	unsigned int NumLocalIactPair = IactPairTab.size();
	unsigned int NumIactPair      = NumLocalIactPair;

	unsigned int NumAllIactPair = 0;
	int recvcounts;
	NumAllIactPair += NumIactPair;
	recvcounts      = NumIactPair * 2;

	int displs = 0;

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
*/

	// Clear the LocMap data
	for (iter1 = locMap.begin(); iter1!=locMap.end() ; ++iter1)
	{
		delete iter1->second;
	}

/*
//	std::cerr << "Size of total IactTab = " << IactPairTab.size() << std::endl;

	std::time(&starttime);

	DistributeIactPair();

	std::time(&endtime);
	timePartitioning += (endtime - starttime);

	ConstructDOandOverLapTab();

	std::cerr
		<< "Size of Cutting Zone (ncelx, necly, ncelz) = ("
		<< ncelx << ", " << ncely << ", " << ncelz << ")" << std::endl
		<< "Size of Interaction = "
		<< (unsigned int)LeapConsultant2::IactPairTab.size() << std::endl
		<< "Size of Overlap Table = (" << overlapTab.size() << ")" << std::endl;

	delete[] pAllIactPair;
	delete[] pLocalIactPair;
*/
}

#endif // _LEAP_CONSULTANT2_H
