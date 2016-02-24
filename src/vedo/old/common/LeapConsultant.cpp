#include <vedo/common/LeapConsultant.h>
#include <vedo/common/NBSConsultant.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <iterator>
#include <map>

namespace vedo
{

LeapConsultant::LeapConsultant
	(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, _uint_t ulwrite, _uint_t ulupdate):
		Consultant(DOWorld, pIactRecordTab, filename, ulwrite)
{
	culUpIact = ulupdate;
	NP = 1;
	rank = 0;
	Reset();
}

_uint_t LeapConsultant::GetIactNum() const
{
	return (_uint_t)IactPairTab.size();
}

_uint_t LeapConsultant::GetIactMaster(_uint_t i) const
{
	return IactPairTab[i].first;
}

_uint_t LeapConsultant::GetIactSlave(_uint_t i) const
{
	return IactPairTab[i].second;
}

void LeapConsultant::SyncWorld(DOContainer& vDO)
{
	const DOStatus* SourceDOStatus = 0;
	      DOStatus* TargetDOStatus = 0;
	for (_uint_t i=0; i<vDO.size(); ++i)
    {
		SourceDOStatus = vDO[i]->GetDOStatus();
		TargetDOStatus = (pDOWorld->GetDOStatus())[i];
		TargetDOStatus->SetPosition(SourceDOStatus->GetPosition());
		TargetDOStatus->SetVelocity(SourceDOStatus->GetVelocity());
		TargetDOStatus->SetImpact(SourceDOStatus->GetImpact());
		TargetDOStatus->SetAngularImpact(SourceDOStatus->GetAngularImpact());
		TargetDOStatus->SetOrientation(SourceDOStatus->GetOrientationX(), SourceDOStatus->GetOrientationZ());
		TargetDOStatus->SetAngularVelocity(SourceDOStatus->GetAngularVelocity());
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

	cIact.BackupImpactStatus();

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
		pDOWorld->WriteIDO("terminate.ido", pIRTbl);
	}

	return HasMobileElement;
}

bool LeapConsultant::ISReset()
{
	return ((ulRoundCount % culUpIact) == 0);
}

bool LeapConsultant::Reset()
{
	vcDO.clear();
	IactPairTab.clear();

    _uint_t numberDO = pDOWorld->GetSystemParameter()->GetDONumber();

	std::vector<_float_t> vecxloc;
	std::vector<_float_t> vecyloc;
	std::vector<_float_t> veczloc;
	std::vector<_float_t> vecvloc;
	std::vector<_float_t> vecrloc;

	const DOStatus* cpdos  = 0;
	const DOModel*  cpdoml = 0;
	for (_uint_t i=0; i<numberDO; ++i)
	{
		vcDO.push_back(i);
		cpdos  = pDOWorld->GetDOStatus(i);
		cpdoml = pDOWorld->GetDOModel(cpdos->GetDOName());

		if (cpdoml->GetScope() == local)
		{
			vecxloc.push_back(cpdos->GetPosition().x()     );
			vecyloc.push_back(cpdos->GetPosition().y()     );
			veczloc.push_back(cpdos->GetPosition().z()     );
			vecvloc.push_back(cpdos->GetVelocity().length());
			vecrloc.push_back(cpdoml->GetRange()           );
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
	_uint_t  NumOfSphere  = vecxloc.size();
	_float_t SphereXMin   = vecxloc[0];
	_float_t SphereXMax   = vecxloc[NumOfSphere-1];
	_float_t SphereYMin   = vecyloc[0];
	_float_t SphereYMax   = vecyloc[NumOfSphere-1];
	_float_t SphereZMin   = veczloc[0];
	_float_t SphereZMax   = veczloc[NumOfSphere-1];
	_float_t SphereVMax   = vecvloc[NumOfSphere-1];
	_float_t SphereRMax   = vecrloc[NumOfSphere-1];
	_float_t DiscardRatio = 0.02;
	_float_t TempDouble   = 1.0 - DiscardRatio;
	_float_t xSpan        = vecxloc[TempDouble*NumOfSphere-1] - vecxloc[DiscardRatio*NumOfSphere];
	_float_t ySpan        = vecyloc[TempDouble*NumOfSphere-1] - vecyloc[DiscardRatio*NumOfSphere];
	_float_t zSpan        = veczloc[TempDouble*NumOfSphere-1] - veczloc[DiscardRatio*NumOfSphere];

	if (xSpan >= ySpan)
	{
		if (xSpan >= zSpan)
		{
			subReset(TrirLeX(), SphereXMin, SphereXMax, SphereYMin, SphereYMax, SphereZMin, SphereZMax, SphereVMax, SphereRMax);
		}
		else
		{
			subReset(TrirLeZ(), SphereXMin, SphereXMax, SphereYMin, SphereYMax, SphereZMin, SphereZMax, SphereVMax, SphereRMax);
		}
	}
	else
	{
		if (ySpan >= zSpan)
		{
			subReset(TrirLeY(), SphereXMin, SphereXMax, SphereYMin, SphereYMax, SphereZMin, SphereZMax, SphereVMax, SphereRMax);
		}
		else
		{
			subReset(TrirLeZ(), SphereXMin, SphereXMax, SphereYMin, SphereYMax, SphereZMin, SphereZMax, SphereVMax, SphereRMax);
		}
	}

	return true;
}

void LeapConsultant::BuildIactTab(std::vector<DOMap>& v1, std::vector<DOMap>& v2)
{
	const Boundary* pbc = &(pDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions());

	for (_uint_t i=0; i<v1.size(); ++i)
	{
		for (_uint_t j=0; j<v2.size(); ++j)
    	{
			const IactModel* cpiactml = pDOWorld->GetIactModel(v1[i].cpdoml()->GetDOGroup(), v2[j].cpdoml()->GetDOGroup());

			if (cpiactml == 0)
    	    {
				continue;
        	}

			if ((DOMap::CalDistance(v1[i], v2[j], pbc)) > (v1[i].SafeLength() + v2[j].SafeLength()))
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
	const Boundary* pbc = &(pDOWorld->GetSystemParameter()->GetPeriodicBoundaryConditions());

	for (_uint_t i=0; i<v.size(); ++i)
    {
		for (_uint_t j=i+1; j<v.size(); ++j)
        {
			const IactModel* cpiactml = pDOWorld->GetIactModel(v[i].cpdoml()->GetDOGroup(), v[j].cpdoml()->GetDOGroup());

			if (cpiactml == 0)
	        {
				continue;
	        }

			if ((DOMap::CalDistance(v[i], v[j], pbc)) > (v[i].SafeLength() + v[j].SafeLength()))
	        {
				continue;
        	}

			if (v[i].id() < v[j].id())
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
	for (_uint_t ul=0; ul<cIact.size(); ++ul)
	{
		const Interaction* pInt = cIact.GetInteraction(ul);

		if (pInt->IsActive())
		{
			pIRTbl->PushRecord(IactPairTab[ul].first, IactPairTab[ul].second, *pInt->GetImpactStatus());
		}
	}

	#ifdef _DEBUG
		std::cout << "Size of IactRecordTab = " << pIRTbl->GetTabSize() << std::endl;
	#endif   // _DEBUG
}

}   // namespace vedo
