#include <NJR/Interfaces/Utility.h>
#include <Common/Interfaces/LeapConsultant.h>
#include <Common/Interfaces/NBSConsultant.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <iterator>
#include <map>

namespace VEDO
{

LeapConsultant::LeapConsultant
	(DOWorld* DOWorld,
	IactRecordTab* pIactRecordTab,
	char filename[],
	unsigned long ulwrite,
	unsigned long ulupdate)
: Consultant(DOWorld, pIactRecordTab, filename, ulwrite)
{
	culUpIact = ulupdate;
	NP = 1;
	rank = 0;
	Reset();
}

unsigned long LeapConsultant::GetIactNum() const
{
	return (unsigned long)IactPairTab.size();
};

unsigned long LeapConsultant::GetIactMaster(unsigned long i) const
{
	return IactPairTab[i].first;
};

unsigned long LeapConsultant::GetIactSlave(unsigned long i) const
{
	return IactPairTab[i].second;
};

void LeapConsultant::SyncWorld(DOContainer& vDO)
{
	const DOStatus* SourceDOStatus = 0;
	      DOStatus* TargetDOStatus = 0;
	for (unsigned long i=0; i<vDO.size(); ++i)
    {

		SourceDOStatus = vDO[i]->GetDOStatus();
		TargetDOStatus = (pDOWorld->GetDOStatus())[i];
		TargetDOStatus->SetPosition(SourceDOStatus->GetPosition());
		TargetDOStatus->SetVelocity(SourceDOStatus->GetVelocity());
		TargetDOStatus
			->SetOrientation
				(SourceDOStatus->GetOrientationX(),
				SourceDOStatus->GetOrientationZ());
		TargetDOStatus
			->SetAngularVelocity(SourceDOStatus->GetAngularVelocity());
	}
}

bool LeapConsultant::NextStep(DOContainer& vDO, IactContainer& cIact)
{
	bool rebuild = false;
	ulRoundCount++;
	pDOWorld->NextStep();
	bool HasMobileElement = true;

	if (ISReset())
	{
		SyncWorld(vDO);
		RebuildIactRecordTab(cIact);
		rebuild = true;
		CleanUp(vDO, cIact);
		HasMobileElement = LeapConsultant::Reset();
	}

	if (ISRecord())
	{
		if (!rebuild)
		{
			SyncWorld(vDO);
			RebuildIactRecordTab(cIact);
			rebuild = true;
		}

		RecordIDO();
	}

	const SystemParameter* csp = pDOWorld->GetSystemParameter();

	if ( csp->GetTimeCurrent() >= csp->GetTimeStop() )
    {
		if (!rebuild)
        {
			SyncWorld(vDO);
			RebuildIactRecordTab(cIact);
			rebuild = true;
		}

//		pDOWorld->WriteXML("terminate.xml");
		pDOWorld->WriteIDO("terminate.ido");
		pIRTbl  ->WriteIRT("terminate.irt");
	}

	return HasMobileElement;
}

bool LeapConsultant::ISReset()
{
	return ((ulRoundCount % culUpIact) == 0);
};

bool LeapConsultant::Reset()
{
	vcDO.clear();
	IactPairTab.clear();

    unsigned long numberDO = pDOWorld->GetSystemParameter()->GetDONumber();

	std::vector<double> vecxloc;
	std::vector<double> vecyloc;
	std::vector<double> veczloc;
	std::vector<double> vecvloc;
	std::vector<double> vecrloc;

	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	for (unsigned long i=0; i<numberDO; ++i)
	{
		vcDO.push_back(i);
		cpdos  = pDOWorld->GetDOStatus(i);
		cpdoml = pDOWorld->GetDOModel(cpdos->GetDOName());

		if (cpdoml->GetScope() == "local")
		{
			vecxloc.push_back(cpdos->GetPosition().x());
			vecyloc.push_back(cpdos->GetPosition().y());
			veczloc.push_back(cpdos->GetPosition().z());
			vecvloc.push_back(cpdos->GetVelocity().length());
			vecrloc.push_back(cpdoml->GetRange());
		}
	}

	//if(vecxloc.size() == 0)
	//{
	//	return false;
	//}

	// Determine the longest direction - "Cutting Direction"
	sort(vecxloc.begin(), vecxloc.end());
	sort(vecyloc.begin(), vecyloc.end());
	sort(veczloc.begin(), veczloc.end());
	sort(vecvloc.begin(), vecvloc.end());
	sort(vecrloc.begin(), vecrloc.end());

/******************************************************************************
 * Aries' Comment (2006/03/31)
 *
 *    I modified these codes. Although they are uglier, but faster.
 ******************************************************************************/
	unsigned int NumOfSphere = vecxloc.size();
	double SphereXMin        = vecxloc[0];
	double SphereXMax        = vecxloc[NumOfSphere-1];
	double SphereYMin        = vecyloc[0];
	double SphereYMax        = vecyloc[NumOfSphere-1];
	double SphereZMin        = veczloc[0];
	double SphereZMax        = veczloc[NumOfSphere-1];
	double SphereVMax        = vecvloc[NumOfSphere-1];
	double SphereRMax        = vecrloc[NumOfSphere-1];
	double DiscardRatio      = 0.02;
	double TempDouble        = 1.0 - DiscardRatio;

	double xSpan
		= vecxloc[TempDouble*NumOfSphere-1] - vecxloc[DiscardRatio*NumOfSphere];

	double ySpan
		= vecyloc[TempDouble*NumOfSphere-1] - vecyloc[DiscardRatio*NumOfSphere];

	double zSpan
		= veczloc[TempDouble*NumOfSphere-1] - veczloc[DiscardRatio*NumOfSphere];

	if (xSpan >= ySpan)
	{
		if (xSpan >= zSpan)
		{
			subReset
				(TrirLeX(),
				SphereXMin,
				SphereXMax,
				SphereYMin,
				SphereYMax,
				SphereZMin,
				SphereZMax,
				SphereVMax,
				SphereRMax);
		}
		else
		{
			subReset
				(TrirLeZ(),
				SphereXMin,
				SphereXMax,
				SphereYMin,
				SphereYMax,
				SphereZMin,
				SphereZMax,
				SphereVMax,
				SphereRMax);
		}
	}
	else
	{
		if (ySpan >= zSpan)
		{
			subReset
				(TrirLeY(),
				SphereXMin,
				SphereXMax,
				SphereYMin,
				SphereYMax,
				SphereZMin,
				SphereZMax,
				SphereVMax,
				SphereRMax);
		}
		else
		{
			subReset
				(TrirLeZ(),
				SphereXMin,
				SphereXMax,
				SphereYMin,
				SphereYMax,
				SphereZMin,
				SphereZMax,
				SphereVMax,
				SphereRMax);
		}
	}

	return true;
};

void LeapConsultant::BuildIactTab
	(std::vector<DOMap>& v1, std::vector<DOMap>& v2)
{
	const Boundary* pbc
		= &(pDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions());

	for (unsigned long i=0; i<v1.size(); ++i)
	{
		for (unsigned long j=0; j<v2.size(); ++j)
    	{
			const IactModel* cpiactml
				= pDOWorld
					->GetIactModel
						(v1[i].cpdoml()->GetDOGroup(),
						v2[j].cpdoml()->GetDOGroup()  );

			if (cpiactml == 0)
    	    {
				continue;
        	}

			if (  (DOMap::CalDistance(v1[i], v2[j], pbc))
				> (v1[i].SafeLength() + v2[j].SafeLength() ) )
	        {
				continue;
    	    }

			if (v1[i].id() < v2[j].id())
        	{
				IactPairTab.push_back(std::make_pair(v1[i].id(), v2[j].id()));
	        }
			else
    	    {
				IactPairTab.push_back(std::make_pair(v2[j].id(), v1[i].id()));
        	}
		}
	}
}

void LeapConsultant::BuildIactTab(std::vector<DOMap>& v)
{
	//PBC by Liao 2009/5/28
	const Boundary* pbc
		= &(pDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions());

	for (unsigned long i=0; i<v.size(); ++i)
    {
		for (unsigned long j=i+1; j<v.size(); ++j)
        {
			const IactModel* cpiactml
				= pDOWorld
					->GetIactModel(v[i].cpdoml()->GetDOGroup(), v[j].cpdoml()->GetDOGroup());

			if (cpiactml == 0)
	        {
				continue;
	        }

			if ((DOMap::CalDistance(v[i], v[j], pbc))
				> (v[i].SafeLength() + v[j].SafeLength()) )
	        {
				continue;
        	}

			if ( v[i].id() < v[j].id())
            {
				IactPairTab.push_back(std::make_pair(v[i].id(), v[j].id()));
            }
			else
            {
				IactPairTab.push_back(std::make_pair(v[j].id(), v[i].id()));
            }
		}
    }
}

void LeapConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	CollectUserDefinedData(cIact);

	pIRTbl->Clear();
	for (unsigned int i=0; i<cIact.size(); ++i)
    {
		const Interaction* pInt = cIact.GetInteraction(i);

		if (pInt->IsActive())
			pIRTbl
				->PushRecord
					(IactPairTab[i].first,
					 IactPairTab[i].second,
					 *pInt->GetImpactStatus());
	}

	#ifdef _VEDO_DEBUG
		std::cout << "Size of IactRecordTab = " << pIRTbl->GetTabSize() << std::endl;
	#endif   // _VEDO_DEBUG
};

};   // namespace VEDO
