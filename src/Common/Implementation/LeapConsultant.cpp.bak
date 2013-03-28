#include <NJR/Interfaces/Utility.h>
#include <FrameWork/Interfaces/DOMap.h>
#include <Common/Interfaces/LeapConsultant.h>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iterator>

using namespace std;

bool IactPairEq
	(pair<unsigned long, unsigned long> p1,
	 pair<unsigned long, unsigned long> p2 )
{
	return ( (p1.first == p2.first) && (p1.second == p2.second) );
};

bool IactPairLe
	(pair<unsigned long, unsigned long> p1,
	 pair<unsigned long, unsigned long> p2 )
{
	if (p1.first != p2.first)
	{
		return (p1.first < p2.first);
	}
	return (p1.second < p2.second);
};

LeapConsultant::LeapConsultant
	(DOWorld* DOWorld,
	IactRecordTab* pIactRecordTab,
	char filename[],
	unsigned long ulwrite,
	unsigned long ulupdate)
: Consultant(DOWorld, pIactRecordTab,filename,ulwrite)
{
	culUpIact = ulupdate;
	LeapConsultant::Reset();
};

unsigned long LeapConsultant::GetIactNum() const
{
	return (unsigned long) IactPairTab.size();
};

unsigned long LeapConsultant::GetIactMaster (unsigned long i) const
{
	return IactPairTab[i].first;
};

unsigned long LeapConsultant::GetIactSlave (unsigned long i) const
{
	return IactPairTab[i].second;
};

bool LeapConsultant::NextStep(DOContainer &cDO, IactContainer &cIact)
{
	ulRoundCount++;
	pDOWorld->NextStep();
	bool HasMobileElement = true;

	if (ISReset())
	{
		pDOWorld->UpdateDOStatus(cDO.GetDOStatus());
		RebuildIactRecordTab(cIact);
		CleanUp(cDO, cIact);   // Check "ZOI"
		HasMobileElement = LeapConsultant::Reset();
	}

	if (ISRecord())
	{
		pDOWorld->UpdateDOStatus(cDO.GetDOStatus());
		RebuildIactRecordTab(cIact);
		Consultant::RecordIDO();
	}

	const SystemParameter* csp = pDOWorld->GetSystemParameter();

	if ( csp->GetTimeCurrent() >= csp->GetTimeStop() )
	{
		pDOWorld->UpdateDOStatus(cDO.GetDOStatus());
		RebuildIactRecordTab(cIact);
//		pDOWorld->WriteXML("terminate.xml");
		pDOWorld->WriteIDO("terminate.ido");
		pIRTbl  ->WriteIRT("terminate.irt");
	}

	return HasMobileElement;
};

bool LeapConsultant::ISReset()
{
	return ((ulRoundCount % culUpIact) == 0);
};

bool LeapConsultant::Reset()
{
	vcDO.clear();
	IactPairTab.clear();

	//	NJR::RunTime("Enter Leap Reset");
	const         SystemParameter* csp = pDOWorld->GetSystemParameter();
	unsigned long numberDO             = csp->GetDONumber();
	NJRvector3d   vFieldForce          = csp->GetFieldForce();
	double        dt                   = culUpIact * (csp->GetTimeInterval());

//	vector<DOMap> vDOMap(numberDO);
	vector<DOMap> FTab;   // The table of "Fixed Element"
	vector<DOMap> MTab;   // The table of "Mobile Element"
	vector<DOMap> WorkTab;

	unsigned long ul, uj;
	if (vcDO.size() == 0)
	{
		for (ul=0; ul<numberDO; ++ul)
		{
			vcDO.push_back(ul);
		}
	}

	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	for (ul=0; ul<numberDO; ++ul)
	{
		cpdos   = pDOWorld->GetDOStatus(ul);
		cpdoml  = pDOWorld->GetDOModel(cpdos->GetDOName());
		DOMap m = DOMap(ul, cpdos, cpdoml, 0.0);
		if (DOMap::ISMobile(m))
		{
			MTab.push_back
				(DOMap
					(ul,
					cpdos,
					cpdoml,
					DOMap::CalSafeDistance(m, vFieldForce, dt)));
		}
//		if (DOMap::ISConstrained(m) || DOMap::ISFixed(m))
		else
		{
			FTab.push_back
				(DOMap
					(ul,
					cpdos,
					cpdoml,
					DOMap::CalSafeDistance(m, vFieldForce, dt)));
		};
/*
			vDOMap[ul]
				= DOMap
					(ul, cpdos, cpdoml, DOMap::CalSafeDistance(m, vFieldForce, dt));
*/
	}

	//if(MTab.size() == 0)
	//{
	//	return false;
	//}
/*
	remove_copy_if
		(vDOMap.begin(), vDOMap.end(), back_inserter(FTab), DOMap::ISMobile);
	remove_copy_if
		(vDOMap.begin(), vDOMap.end(), back_inserter(MTab), DOMap::ISFixed);
*/

	double maxX
		= max_element
			(MTab.begin(), MTab.end(), DOMap::ComX)->cpdos()->GetPosition().x();

	double minX
		= min_element
			(MTab.begin(), MTab.end(), DOMap::ComX)->cpdos()->GetPosition().x();

	double maxY
		= max_element
			(MTab.begin(), MTab.end(), DOMap::ComY)->cpdos()->GetPosition().y();

	double minY
		= min_element
			(MTab.begin(), MTab.end(), DOMap::ComY)->cpdos()->GetPosition().y();

	double maxZ
		= max_element
			(MTab.begin(), MTab.end(), DOMap::ComZ)->cpdos()->GetPosition().z();

	double minZ
		= min_element
			(MTab.begin(), MTab.end(), DOMap::ComZ)->cpdos()->GetPosition().z();

	double maxV
		= max_element
			(MTab.begin(), MTab.end(), DOMap::ComV)
				->cpdos()->GetVelocity().length();

//	double maxR
//		= max_element
//			(MTab.begin(), MTab.end(), DOMap::ComR)
//				->cpdoml()->GetShapeAttributes().sphere.radius;
	double maxR
		= max_element
			(MTab.begin(), MTab.end(), DOMap::ComR)->cpdoml()->GetRange();
	double maxS
		= max_element(MTab.begin(), MTab.end(), DOMap::ComS)->SafeLength();

	double SD = maxV * dt + (0.5 * vFieldForce * dt * dt).length() + 1.1 * maxR;

/******************************************************************************
 * Aries' Comment (2006/04/26)
 *    We have to avoid the situation that "Range = 0"
 ******************************************************************************/
	double Range = 8.0 * SD;
	double dX    = maxX - minX;
	double dY    = maxY - minY;
	double dZ    = maxZ - minZ;
	if (dX != 0.0)
	{
		Range = min(Range, dX);
	}
	if (dY != 0.0)
	{
		Range = min(Range, dY);
	}
	if (dZ != 0.0)
	{
		Range = min(Range, dZ);
	}
/*
	double Range = min(min(maxX-minX, maxY-minY), min(maxZ-minZ, 8.0*SD));
*/

	int zone = 0;

	for(double X=minX; X<=maxX; X+=Range)
	{
		for(double Y=minY; Y<=maxY; Y+=Range)
		{
			for(double Z=minZ; Z<=maxZ; Z+=Range)
			{
				NJRvector3d zoneLower(X-SD, Y-SD, Z-SD);
				NJRvector3d zoneUpper(X+Range+SD, Y+Range+SD, Z+Range+SD);

				NJRvector3d zoneLowerPre = zoneLower;
				NJRvector3d zoneUpperPre = zoneUpper;
				double rangeLen = Range + 2.*SD;
				const Boundary& pbc = csp->GetPeriodicBoundaryConditions();
				pbc.EnforceBoundaryConditions(&zoneLower);
				pbc.EnforceBoundaryConditions(&zoneUpper);
				double lowX = (rangeLen >= pbc.GetRange().x()) ? zoneLowerPre.x() : zoneLower.x();
				double lowY = (rangeLen >= pbc.GetRange().y()) ? zoneLowerPre.y() : zoneLower.y();
				double lowZ = (rangeLen >= pbc.GetRange().z()) ? zoneLowerPre.z() : zoneLower.z();
				double uppX = (rangeLen >= pbc.GetRange().x()) ? zoneUpperPre.x() : zoneUpper.x();
				double uppY = (rangeLen >= pbc.GetRange().y()) ? zoneUpperPre.y() : zoneUpper.y();
				double uppZ = (rangeLen >= pbc.GetRange().z()) ? zoneUpperPre.z() : zoneUpper.z();
				zoneLower.Set(lowX, lowY, lowZ);
				zoneUpper.Set(uppX, uppY, uppZ);

				int Len
					= (int)
						(MTab.size()
						- count_if
							(MTab.begin(),
							 MTab.end(),
							 OutOfArea
								(zoneLower.x(), zoneUpper.x(),
								 zoneLower.y(), zoneUpper.y(),
								 zoneLower.z(), zoneUpper.z()) ) );

				if (Len == 0)
				{
					continue;
				}

				zone++;
				WorkTab.clear();
				remove_copy_if
					(MTab.begin(),
					 MTab.end(),
					 back_inserter(WorkTab),
					 OutOfArea
						(zoneLower.x(), zoneUpper.x(),
						 zoneLower.y(), zoneUpper.y(),
						 zoneLower.z(), zoneUpper.z()));

				copy(FTab.begin(), FTab.end(), back_inserter(WorkTab));

				for (ul=0; ul<WorkTab.size(); ++ul)
				{
					for (uj=ul+1; uj<WorkTab.size(); ++uj)
					{
						const IactModel* cpiactml
							= pDOWorld->GetIactModel
								(WorkTab[ul].cpdoml()->GetDOGroup(),
								WorkTab[uj].cpdoml()->GetDOGroup()  );

						if (cpiactml == 0)
						{
							continue;
						}

						if (  DOMap::CalDistance(WorkTab[ul], WorkTab[uj], &pbc) // PBC by Liao 2009/5/27
							> (WorkTab[ul].SafeLength()+WorkTab[uj].SafeLength()))
						{
							continue;
						}

						if (WorkTab[ul].id() < WorkTab[uj].id())
						{
							LeapConsultant::IactPairTab.push_back
								(make_pair(WorkTab[ul].id(), WorkTab[uj].id()));
						}
						else
						{
							LeapConsultant::IactPairTab.push_back
								(make_pair(WorkTab[uj].id(), WorkTab[ul].id()));
						}
					}
				}
			}
		}
	}

	cout
		<< "Number of cutting zone = "
		<< zone
		<< "; "
		<< "Size of interaction = "
		<< (unsigned int) LeapConsultant::IactPairTab.size()
		<< '\n';

	sort
		(LeapConsultant::IactPairTab.begin(),
		 LeapConsultant::IactPairTab.end(),
		 IactPairLe                          );

	LeapConsultant::IactPairTab.erase
		(unique
			(LeapConsultant::IactPairTab.begin(),
			 LeapConsultant::IactPairTab.end(),
			 IactPairEq                          ),
		LeapConsultant::IactPairTab.end()          );

	cout
		<< "Safety Distance = "
		<< SD
		<< "; "
		<< "Cutting range = "
		<< Range
		<< "; "
		<< "Uniqued interaction size = "
		<< (unsigned int)(IactPairTab.size())
		<< '\n';

//	NJR::RunTime("Leap Reset Accomplished");
	return true;
};

void LeapConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	CollectUserDefinedData(cIact);
};
