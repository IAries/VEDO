#include <aries/utility/Constants.h>
#include <vedo/framework/IactRecordTab.h>
#include <cstring>
#include <fstream>

#include <stdint.h>

namespace vedo
{

IactRecordTab::IactRecordTab()
{
}

IactRecordTab::IactRecordTab(const std::map<std::pair<_uint_t,_uint_t>, ImpactStatus>& m)
{
	mapImStatus = m;
}

const ImpactStatus* IactRecordTab::GetImpactStatus(_uint_t master,_uint_t slave)
{
	std::map<std::pair<_uint_t,_uint_t>, ImpactStatus>::const_iterator iter
		= mapImStatus.find(std::make_pair(master, slave));
	return (iter != mapImStatus.end()) ? &(iter->second) : 0;
}

void IactRecordTab::CleanUserDefinedValueInImpactStatus()
{
	for (std::map<std::pair<_uint_t,_uint_t>, ImpactStatus>::iterator
		 iter = mapImStatus.begin(); iter != mapImStatus.end(); iter++)
	{
		iter->second.CleanAllUserDefinedValue();
	}
}

void IactRecordTab::PushRecord(_uint_t master, _uint_t slave, const ImpactStatus& s)
{
	mapImStatus.insert(std::make_pair(std::make_pair(master,slave),s));
}

void IactRecordTab::ModifyPair(std::map<_uint_t, _uint_t> mNewPairList)
{
	std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::iterator mmapImStatus;
	std::map<std::pair<_uint_t, _uint_t>, ImpactStatus> mapImStatusExtra;
	std::vector<std::pair<_uint_t, _uint_t> > vErasePair;
	_uint_t ulFirstOriginal, ulSecondOriginal, ulFirstNew, ulSecondNew;
	for (mmapImStatus=mapImStatus.begin(); mmapImStatus!=mapImStatus.end(); mmapImStatus++)
	{
		ulFirstOriginal  = mmapImStatus->first.first;
		ulSecondOriginal = mmapImStatus->first.second;
		ulFirstNew       = ulFirstOriginal  + mNewPairList[ulFirstOriginal];
		ulSecondNew      = ulSecondOriginal + mNewPairList[ulSecondOriginal];
		if ((ulFirstOriginal != ulFirstNew) || (ulSecondOriginal != ulSecondNew))
		{
			vErasePair.push_back(std::make_pair(ulFirstOriginal, ulSecondOriginal));
			mapImStatusExtra[std::make_pair(ulFirstNew, ulSecondNew)] = mmapImStatus->second;
		}
	}

	for (_uint_t ul=0; ul<vErasePair.size(); ul++)
	{
		mapImStatus.erase(mapImStatus.find(vErasePair[ul]));
	}

	for (mmapImStatus=mapImStatusExtra.begin(); mmapImStatus!=mapImStatusExtra.end(); mmapImStatus++)
	{
		mapImStatus[mmapImStatus->first] = mmapImStatus->second;
	}
}

_uint_t IactRecordTab::ContactNumber() const
{
	_uint_t ulContactNumber = 0;
	for (std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::const_iterator
		 pmapImStatus = mapImStatus.begin(); pmapImStatus != mapImStatus.end(); pmapImStatus++)
	{
		if (pmapImStatus->second.Contact())
		{
			ulContactNumber++;
		}
	}
	return ulContactNumber;
}

_uint_t IactRecordTab::BondNumber() const
{
	_uint_t ulBondNumber = 0;
	for (std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::const_iterator
		 pmapImStatus = mapImStatus.begin(); pmapImStatus != mapImStatus.end(); pmapImStatus++)
	{
		if (pmapImStatus->second.Contact())
		{
			ulBondNumber++;
		}
	}
	return ulBondNumber;
}

void IactRecordTab::EraseElement(const _uint_t& ulID)
{
	std::map<std::pair<_uint_t, _uint_t>, ImpactStatus> NewmapImStatus;
	_uint_t ulNew1, ulNew2;
	for (std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::iterator
		 itr = mapImStatus.begin(); itr != mapImStatus.end(); itr++)
	{
		if ((itr->first.first != ulID) && (itr->first.second != ulID))
		{
			if (itr->first.first > ulID)
			{
				ulNew1 = itr->first.first - 1;
			}
			else
			{
				ulNew1 = itr->first.first;
			}

			if (itr->first.second > ulID)
			{
				ulNew2 = itr->first.second - 1;
			}
			else
			{
				ulNew2 = itr->first.second;
			}

			NewmapImStatus[std::make_pair(ulNew1, ulNew2)] = itr->second;
		}
	}
	mapImStatus = NewmapImStatus;
}

void IactRecordTab::EraseElements(const std::vector<_uint_t>& ulIDList, const _uint_t& ulDONum)
{
	std::vector<bool> bActived;
	_uint_t ulIDListSize = ulIDList.size();
	_uint_t ul, ul2;
	_uint_t ulID = 0;
	std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::iterator itr;

	for (itr = mapImStatus.begin(); itr != mapImStatus.end(); itr++, ulID++)
	{
		bActived.push_back(true);
		for (ul=0; ul<ulIDListSize; ul++)
		{
			if (itr->first.first == ulIDList[ul])
			{
				bActived[ulID] = false;
				break;
			}
			else
			{
				if (itr->first.second == ulIDList[ul])
				{
					bActived[ulID] = false;
					break;
				}
			}
		}
	}

	std::vector<_uint_t> ulNewID;
	bool bActivedMark;
	ulID = 0;
	for (ul=0; ul<ulDONum; ul++)
	{
		bActivedMark = true;
		for (ul2=0; ul2<ulIDListSize; ul2++)
		{
			if (ul == ulIDList[ul2])
			{
				bActivedMark = false;
				break;
			}
		}
		if (bActivedMark)
		{
			ulNewID.push_back(ulID);
			ulID++;
		}
		else
		{
			ulNewID.push_back(ulDONum);   // Invalid ID
		}
	}

	ulID = 0;
	std::map<std::pair<_uint_t, _uint_t>, ImpactStatus> NewmapImStatus;
	for (itr = mapImStatus.begin(); itr != mapImStatus.end(); itr++, ulID++)
	{
		if (bActived[ulID])
		{
			NewmapImStatus[std::make_pair(ulNewID[itr->first.first], ulNewID[itr->first.second])] = itr->second;
		}
	}
	mapImStatus = NewmapImStatus;
}

void IactRecordTab::print() const
{
	std::map<std::pair<_uint_t,_uint_t>, ImpactStatus>::const_iterator iter;
	for (iter=mapImStatus.begin(); iter!=mapImStatus.end(); ++iter)
	{
		std::cout
			<< "( "
			<< iter->first.first
			<< ", "
			<< iter->first.second
			<< "): "
			<< iter->second.Contact()
			<< " "
			<< iter->second.Bond()
			<< " "
			<< iter->second.Kn()
			<< " "
			<< iter->second.InitialVelocity()
			<< " ("
			<< iter->second.ShearForce().x()
			<< ", "
			<< iter->second.ShearForce().y()
			<< ", "
			<< iter->second.ShearForce().z()
			<< ") ("
			<< iter->second.ImpactPoint().x()
			<< ", "
			<< iter->second.ImpactPoint().y()
			<< ", "
			<< iter->second.ImpactPoint().z()
			<< ") ("
			<< iter->second.ImpactDirection().x()
			<< ", "
			<< iter->second.ImpactDirection().y()
			<< ", "
			<< iter->second.ImpactDirection().z()
			<< ") ("
			<< iter->second.ImpactToMaster().x()
			<< ", "
			<< iter->second.ImpactToMaster().y()
			<< ", "
			<< iter->second.ImpactToMaster().z()
			<< ") ("
			<< iter->second.AngularImpactToMaster().x()
			<< ", "
			<< iter->second.AngularImpactToMaster().y()
			<< ", "
			<< iter->second.AngularImpactToMaster().z()
			<< ") "
			<< iter->second.Overlap()
			<< std::endl;
	}
}

void IactRecordTab::DumpIactStatus(const _float_t& time, const char* filename) const
{
    std::ofstream oCSVFile(filename, std::ios::out);

	oCSVFile
		<< "Time, MasterDOStatusSN, SlaveDOStatusSN, "
		<< "Contact, Bond, "
		<< "RememberedNormalStiffness, RememberedInitialVelocity, Overlap, "
		<< "RememberedShearForce-X, "
		<< "RememberedShearForce-Y, "
		<< "RememberedShearForce-Z, "
		<< "ImpactPoint-X, ImpactPoint-Y, ImpactPoint-Z, "
		<< "ImpactDirection-X, ImpactDirection-Y, ImpactDirection-Z, "
		<< "ImpactToMaster-X, ImpactToMaster-Y, ImpactToMaster-Z, "
		<< "AngularImpactToMaster-X, "
		<< "AngularImpactToMaster-Y, "
		<< "AngularImpactToMaster-Z";

	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	for(_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
	{
		oCSVFile << ", User-defined-Value-" << u+1;
	}

    oCSVFile << std::endl;

    const std::pair<_uint_t, _uint_t> * ppulul;
    const ImpactStatus* pis;
    Vector3df vV;
    for(std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::const_iterator
        pmapImStatus = mapImStatus.begin(); pmapImStatus != mapImStatus.end(); pmapImStatus++)
    {
        ppulul = &(pmapImStatus->first);
        pis    = &(pmapImStatus->second);

        oCSVFile
			<< time           << ", "
			<< ppulul->first  << ", "
			<< ppulul->second << ", ";

        if (pis->Contact())
        {
            oCSVFile << "1, ";
        }
        else
        {
            oCSVFile << "0, ";
        }

        if (pis->Bond())
        {
            oCSVFile << "1, ";
        }
        else
        {
            oCSVFile << "0, ";
        }

        oCSVFile << pis->Kn() << ", ";

        oCSVFile << pis->InitialVelocity() << ", ";

        oCSVFile << pis->Overlap() << ", ";

        vV = pis->ShearForce();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

        vV = pis->ImpactPoint();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

        vV = pis->ImpactDirection();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

        vV = pis->ImpactToMaster();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

        vV = pis->AngularImpactToMaster();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z();

        const _float_t* cdpudv = pis->RetrieveAllUserDefinedValue();

		for (_uint_t u=0; u<4*uNumUDDImpactStatus; u++)
		{
			oCSVFile << ", " << *(cdpudv+u);
		}

		oCSVFile << std::endl;
    }

    oCSVFile.close();
}

std::ofstream& IactRecordTab::operator >> (std::ofstream& idof) const
{
    _uint_t ulInteractionNumber = mapImStatus.size();
	idof.write((char*) &ulInteractionNumber, sizeof(_uint_t));
	if(ulInteractionNumber == 0)
	{
		return idof;
	}

	const ImpactStatus* isp;
	bool                bTemp;
	_float_t        dTemp;
	Vector3df       vTemp;

	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	if(uNumUDDImpactStatus != 0)
	{
		const _float_t* dpUDV;
		for (std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::const_iterator
			 mapImStatusP = mapImStatus.begin(); mapImStatusP != mapImStatus.end(); mapImStatusP++)
		{
			idof.write((char*) &(mapImStatusP->first.first) , sizeof(_uint_t));
			idof.write((char*) &(mapImStatusP->first.second), sizeof(_uint_t));

			isp   = &(mapImStatusP->second);
			bTemp = isp->Contact();
			idof.write((char*) &bTemp                       , sizeof(bool));
			bTemp = isp->Bond();
			idof.write((char*) &bTemp                       , sizeof(bool));
			dTemp =isp->Kn();
			idof.write((char*) &dTemp                       , sizeof(_float_t));
			dTemp =isp->InitialVelocity();
			idof.write((char*) &dTemp                       , sizeof(_float_t));
			vTemp = isp->ShearForce();
			idof.write((char*) &vTemp                       , 3*sizeof(_float_t));
			vTemp = isp->ImpactPoint();
			idof.write((char*) &vTemp                       , 3*sizeof(_float_t));
			vTemp = isp->ImpactDirection();
			idof.write((char*) &vTemp                       , 3*sizeof(_float_t));
			vTemp = isp->ImpactToMaster();
			idof.write((char*) &vTemp                       , 3*sizeof(_float_t));
			vTemp = isp->AngularImpactToMaster();
			idof.write((char*) &vTemp                       , 3*sizeof(_float_t));
			dTemp =isp->Overlap();
			idof.write((char*) &dTemp                       , sizeof(_float_t));
			dpUDV = isp->RetrieveAllUserDefinedValue();
			idof.write((char*) dpUDV                        , uNumUDDImpactStatus*sizeof(_float_t));
			idof.write((char*) (dpUDV+3*uNumUDDImpactStatus), uNumUDDImpactStatus*sizeof(_float_t));
		}
	}
	else
	{
		for(std::map<std::pair<_uint_t, _uint_t>, ImpactStatus>::const_iterator
			mapImStatusP = mapImStatus.begin(); mapImStatusP != mapImStatus.end(); mapImStatusP++)
		{
			idof.write((char*) &(mapImStatusP->first.first)       , sizeof(_uint_t));
			idof.write((char*) &(mapImStatusP->first.second)      , sizeof(_uint_t));
			isp   = &(mapImStatusP->second);
			bTemp = isp->Contact();
			idof.write((char*) &bTemp                             , sizeof(bool));
			bTemp = isp->Bond();
			idof.write((char*) &bTemp                             , sizeof(bool));
			dTemp =isp->Kn();
			idof.write((char*) &dTemp                             , sizeof(_float_t));
			dTemp =isp->InitialVelocity();
			idof.write((char*) &dTemp                             , sizeof(_float_t));
			vTemp = isp->ShearForce();
			idof.write((char*) &vTemp                             , 3*sizeof(_float_t));
			vTemp = isp->ImpactPoint();
			idof.write((char*) &vTemp                             , 3*sizeof(_float_t));
			vTemp = isp->ImpactDirection();
			idof.write((char*) &vTemp                             , 3*sizeof(_float_t));
			vTemp = isp->ImpactToMaster();
			idof.write((char*) &vTemp                             , 3*sizeof(_float_t));
			vTemp = isp->AngularImpactToMaster();
			idof.write((char*) &vTemp                             , 3*sizeof(_float_t));
			dTemp =isp->Overlap();
			idof.write((char*) &dTemp                             , sizeof(_float_t));
		}
	}

	return idof;
}

std::ifstream& IactRecordTab::operator << (std::ifstream& idof)
{
	mapImStatus.clear();
	_uint_t ulInteractionNumber;

	idof.read((char*) &ulInteractionNumber, sizeof(_uint_t));
	if(ulInteractionNumber == 0)
	{
		return idof;
	}

	_uint_t   ulElementsMaster, ulElementsSlave;
	bool          bContact, bBond;
	_float_t  dKn, dInitialVelocity, dOverlap;
	Vector3df vShearForce, vImpactPoint, vImpactDirection, vImpactToMaster, vAngularImpactToMaster;
	ImpactStatus  is;

	aries::Constants* aries_cp = aries::Constants::Instance();
	_uint_t uNumUDDImpactStatus = aries_cp->NumUDDImpactStatus();

	if(uNumUDDImpactStatus != 0)
	{
		_float_t* dpudv = new _float_t[4*uNumUDDImpactStatus];
		memcpy(dpudv, is.RetrieveAllUserDefinedValue(), 4*uNumUDDImpactStatus*sizeof(_float_t));
		for (_uint_t ul=0; ul<ulInteractionNumber; ++ul)
		{
			idof.read((char*) &ulElementsMaster          , sizeof(_uint_t));
			idof.read((char*) &ulElementsSlave           , sizeof(_uint_t));

			idof.read((char*) &bContact                  , sizeof(bool));
			is.SetContact(bContact);

			idof.read((char*) &bBond                     , sizeof(bool));
			is.SetBond(bBond);

			idof.read((char*) &dKn                       , sizeof(_float_t));
			is.SetKn(dKn);

			idof.read((char*) &dInitialVelocity          , sizeof(_float_t));
			is.SetInitialVelocity(dInitialVelocity);

			idof.read((char*) &vShearForce               , 3*sizeof(_float_t));
			is.SetShearForce(vShearForce);

			idof.read((char*) &vImpactPoint              , 3*sizeof(_float_t));
			is.SetImpactPoint(vImpactPoint);

			idof.read((char*) &vImpactDirection          , 3*sizeof(_float_t));
			is.SetImpactDirection(vImpactDirection);

			idof.read((char*) &vImpactToMaster           , 3*sizeof(_float_t));
			is.SetImpactToMaster(vImpactToMaster);

			idof.read((char*) &vAngularImpactToMaster    , 3*sizeof(_float_t));
			is.SetAngularImpactToMaster(vAngularImpactToMaster);

			idof.read((char*) &dOverlap                  , sizeof(_float_t));
			is.SetOverlap(dOverlap);

			idof.read((char*) dpudv                      , uNumUDDImpactStatus*sizeof(_float_t));
			idof.read((char*) dpudv+3*uNumUDDImpactStatus, uNumUDDImpactStatus*sizeof(_float_t));

			mapImStatus.insert(std::make_pair(std::make_pair(ulElementsMaster, ulElementsSlave), is));
		}
	}
	else
	{
		for (_uint_t ul=0; ul<ulInteractionNumber; ++ul)
		{
			idof.read((char*) &ulElementsMaster          , sizeof(_uint_t));
			idof.read((char*) &ulElementsSlave           , sizeof(_uint_t));

			idof.read((char*) &bContact                  , sizeof(bool));
			is.SetContact(bContact);

			idof.read((char*) &bBond                     , sizeof(bool));
			is.SetBond(bBond);

			idof.read((char*) &dKn                       , sizeof(_float_t));
			is.SetKn(dKn);

			idof.read((char*) &dInitialVelocity          , sizeof(_float_t));
			is.SetInitialVelocity(dInitialVelocity);

			idof.read((char*) &vShearForce               , 3*sizeof(_float_t));
			is.SetShearForce(vShearForce);

			idof.read((char*) &vImpactPoint              , 3*sizeof(_float_t));
			is.SetImpactPoint(vImpactPoint);

			idof.read((char*) &vImpactDirection          , 3*sizeof(_float_t));
			is.SetImpactDirection(vImpactDirection);

			idof.read((char*) &vImpactToMaster           , 3*sizeof(_float_t));
			is.SetImpactToMaster(vImpactToMaster);

			idof.read((char*) &vAngularImpactToMaster    , 3*sizeof(_float_t));
			is.SetAngularImpactToMaster(vAngularImpactToMaster);

			idof.read((char*) &dOverlap                  , sizeof(_float_t));
			is.SetOverlap(dOverlap);

			mapImStatus.insert(std::make_pair(std::make_pair(ulElementsMaster, ulElementsSlave), is));
		}
	}

	return idof;
}

}   // namespace vedo
