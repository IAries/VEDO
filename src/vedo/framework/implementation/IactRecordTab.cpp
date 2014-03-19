#include <vedo/Constants.h>
#include <vedo/njr/interfaces/Utility.h>
#include <vedo/framework/interfaces/IactRecordTab.h>
#include <cstring>
#include <fstream>

#include <stdint.h>

namespace vedo
{

IactRecordTab::IactRecordTab()
{
};

IactRecordTab::IactRecordTab
	(const std::map<std::pair<unsigned long,unsigned long>, ImpactStatus>& m)
{
	mapImStatus = m;
};

const ImpactStatus* IactRecordTab::GetImpactStatus
	(unsigned long master,unsigned long slave)
{
	std::map<std::pair<unsigned long,unsigned long>, ImpactStatus>::const_iterator iter
		= mapImStatus.find(std::make_pair(master, slave));
	return (iter != mapImStatus.end()) ? &(iter->second) : 0;
};

void IactRecordTab::CleanUserDefinedValueInImpactStatus()
{
	for(std::map<std::pair<unsigned long,unsigned long>, ImpactStatus>::iterator iter = mapImStatus.begin();
		iter != mapImStatus.end();
		iter++)
		iter->second.CleanAllUserDefinedValue();
};

void IactRecordTab::PushRecord
	(unsigned long master, unsigned long slave, const ImpactStatus& s)
{
	mapImStatus.insert(std::make_pair(std::make_pair(master,slave),s));
}

void IactRecordTab::ModifyPair(std::map<unsigned long, long> mNewPairList)
{
	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::iterator mmapImStatus;
	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus> mapImStatusExtra;
	std::vector<std::pair<unsigned long, unsigned long> > vErasePair;
	unsigned long ulFirstOriginal, ulSecondOriginal, ulFirstNew, ulSecondNew;
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

	for (unsigned ul=0; ul<vErasePair.size(); ul++)
		mapImStatus.erase(mapImStatus.find(vErasePair[ul]));

	for (mmapImStatus=mapImStatusExtra.begin();
		 mmapImStatus!=mapImStatusExtra.end();
		 mmapImStatus++                        )
		mapImStatus[mmapImStatus->first] = mmapImStatus->second;
};

unsigned long IactRecordTab::ContactNumber() const
{
	unsigned long ulContactNumber = 0;
	for (std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator
			pmapImStatus=mapImStatus.begin();
		pmapImStatus!=mapImStatus.end();
		pmapImStatus++                                                        )
	{
		if (pmapImStatus->second.Contact())
			ulContactNumber++;
	}
	return ulContactNumber;
};

void IactRecordTab::EraseElement(const unsigned long& ulID)
{
	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus> NewmapImStatus;
	unsigned long ulNew1, ulNew2;
	for(std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::iterator
		itr = mapImStatus.begin(); itr != mapImStatus.end(); itr++)
	{
		if((itr->first.first != ulID) && (itr->first.second != ulID))
		{
			if(itr->first.first > ulID)
			{
				ulNew1 = itr->first.first - 1;
			}
			else
			{
				ulNew1 = itr->first.first;
			}

			if(itr->first.second > ulID)
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
};

void IactRecordTab::EraseElements
	(const std::vector<unsigned long>& ulIDList, const unsigned long& ulDONum)
{
	std::vector<bool> bActived;
	unsigned long ulIDListSize = ulIDList.size();
	unsigned long ul, ul2;
	unsigned long ulID = 0;
	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::iterator itr;

	for(itr = mapImStatus.begin(); itr != mapImStatus.end(); itr++, ulID++)
	{
		bActived.push_back(true);
		for(ul=0; ul<ulIDListSize; ul++)
		{
			if(itr->first.first == ulIDList[ul])
			{
				bActived[ulID] = false;
				break;
			}
			else
			{
				if(itr->first.second == ulIDList[ul])
				{
					bActived[ulID] = false;
					break;
				}
			}
		}
	}

	std::vector<unsigned long> ulNewID;
	bool bActivedMark;
	ulID = 0;
	for(ul=0; ul<ulDONum; ul++)
	{
		bActivedMark = true;
		for(ul2=0; ul2<ulIDListSize; ul2++)
		{
			if(ul == ulIDList[ul2])
			{
				bActivedMark = false;
				break;
			}
		}
		if(bActivedMark)
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
	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus> NewmapImStatus;
	for(itr = mapImStatus.begin(); itr != mapImStatus.end(); itr++, ulID++)
	{
		if(bActived[ulID])
		{
			NewmapImStatus
				[std::make_pair
					(ulNewID[itr->first.first],
					 ulNewID[itr->first.second])] = itr->second;
		}
	}
	mapImStatus = NewmapImStatus;
};

void IactRecordTab::print() const
{
	std::map<std::pair<unsigned long,unsigned long>, ImpactStatus>::const_iterator iter;
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
};

void IactRecordTab::DumpIactStatus(const char* filename) const
{
    std::ofstream oCSVFile(filename, std::ios::out);

	oCSVFile
		<< "MasterDOStatusSN, SlaveDOStatusSN, "
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

	for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
		oCSVFile << ", User-defined-Value-" << u+1;

    oCSVFile << std::endl;

    const std::pair<unsigned long, unsigned long> * ppulul;
    const ImpactStatus* pis;
    njr::Vector3d vV;
    for(std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator
        pmapImStatus  = mapImStatus.begin();
        pmapImStatus != mapImStatus.end();
        pmapImStatus++                      )
    {
        ppulul = &(pmapImStatus->first);
        pis    = &(pmapImStatus->second);

        oCSVFile << ppulul->first << ", " << ppulul->second << ", ";

        if(pis->Contact())
        {
            oCSVFile << "1, ";
        }
        else
        {
            oCSVFile << "0, ";
        }

        if(pis->Bond())
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

        const double* cdpudv = pis->RetrieveAllUserDefinedValue();

		for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
			oCSVFile << ", " << *(cdpudv+u);

		oCSVFile << std::endl;
    }

    oCSVFile.close();
};

std::ofstream& IactRecordTab::operator >> (std::ofstream& idof) const
{
    unsigned long ulInteractionNumber = mapImStatus.size();
	idof.write((char*) &ulInteractionNumber, sizeof(_VEDO_unsigned_long));
	if(ulInteractionNumber == 0)
		return idof;

	const ImpactStatus* isp;
	bool                bTemp;
	double              dTemp;
	njr::Vector3d       vTemp;

	if(uNumUDDImpactStatus != 0)
	{
		const double* dpUDV;
		for(std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator
			mapImStatusP = mapImStatus.begin();
			mapImStatusP != mapImStatus.end();
			mapImStatusP++                                                                 )
		{
			idof.write((char*) &(mapImStatusP->first.first)       , sizeof(_VEDO_unsigned_long));
			idof.write((char*) &(mapImStatusP->first.second)      , sizeof(_VEDO_unsigned_long));

			isp   = &(mapImStatusP->second);
			bTemp = isp->Contact();
			idof.write((char*) &bTemp                             , sizeof(bool));
			bTemp = isp->Bond();
			idof.write((char*) &bTemp                             , sizeof(bool));
			dTemp =isp->Kn();
			idof.write((char*) &dTemp                             , sizeof(double));
			dTemp =isp->InitialVelocity();
			idof.write((char*) &dTemp                             , sizeof(double));
			vTemp = isp->ShearForce();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			vTemp = isp->ImpactPoint();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			vTemp = isp->ImpactDirection();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			vTemp = isp->ImpactToMaster();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			vTemp = isp->AngularImpactToMaster();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			dTemp =isp->Overlap();
			idof.write((char*) &dTemp                             , sizeof(double));
			dpUDV = isp->RetrieveAllUserDefinedValue();
			idof.write((char*) dpUDV                              , uNumUDDImpactStatus*sizeof(double));
			idof.write((char*) (dpUDV+3*vedo::uNumUDDImpactStatus), uNumUDDImpactStatus*sizeof(double));
		}
	}
	else
	{
		for(std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator
			mapImStatusP = mapImStatus.begin();
			mapImStatusP != mapImStatus.end();
			mapImStatusP++                                                                 )
		{
			idof.write((char*) &(mapImStatusP->first.first)       , sizeof(_VEDO_unsigned_long));
			idof.write((char*) &(mapImStatusP->first.second)      , sizeof(_VEDO_unsigned_long));
			isp   = &(mapImStatusP->second);
			bTemp = isp->Contact();
			idof.write((char*) &bTemp                             , sizeof(bool));
			bTemp = isp->Bond();
			idof.write((char*) &bTemp                             , sizeof(bool));
			dTemp =isp->Kn();
			idof.write((char*) &dTemp                             , sizeof(double));
			dTemp =isp->InitialVelocity();
			idof.write((char*) &dTemp                             , sizeof(double));
			vTemp = isp->ShearForce();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			vTemp = isp->ImpactPoint();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			vTemp = isp->ImpactDirection();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			vTemp = isp->ImpactToMaster();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			vTemp = isp->AngularImpactToMaster();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			dTemp =isp->Overlap();
			idof.write((char*) &dTemp                             , sizeof(double));
		}
	}

	return idof;
};

std::ifstream& IactRecordTab::operator << (std::ifstream& idof)
{
	mapImStatus.clear();
	unsigned long ulInteractionNumber;

	idof.read((char*) &ulInteractionNumber, sizeof(_VEDO_unsigned_long));
	if(ulInteractionNumber == 0)
		return idof;

	unsigned long ulElementsMaster, ulElementsSlave;
	bool          bContact, bBond;
	double        dKn, dInitialVelocity, dOverlap;
	njr::Vector3d
        vShearForce, vImpactPoint, vImpactDirection,
        vImpactToMaster, vAngularImpactToMaster;
	ImpactStatus  is;

	if(uNumUDDImpactStatus != 0)
	{
		double* dpudv = new double[4*vedo::uNumUDDImpactStatus];
		memcpy(dpudv, is.RetrieveAllUserDefinedValue(), 4*uNumUDDImpactStatus*sizeof(double));
		for (unsigned long ul=0; ul<ulInteractionNumber; ++ul)
		{
			idof.read((char*) &ulElementsMaster          , sizeof(_VEDO_unsigned_long));
			idof.read((char*) &ulElementsSlave           , sizeof(_VEDO_unsigned_long));

			idof.read((char*) &bContact                  , sizeof(bool));
			is.SetContact(bContact);

			idof.read((char*) &bBond                     , sizeof(bool));
			is.SetBond(bBond);

			idof.read((char*) &dKn                       , sizeof(double));
			is.SetKn(dKn);

			idof.read((char*) &dInitialVelocity          , sizeof(double));
			is.SetInitialVelocity(dInitialVelocity);

			idof.read((char*) &vShearForce               , 3*sizeof(double));
			is.SetShearForce(vShearForce);

			idof.read((char*) &vImpactPoint              , 3*sizeof(double));
			is.SetImpactPoint(vImpactPoint);

			idof.read((char*) &vImpactDirection          , 3*sizeof(double));
			is.SetImpactDirection(vImpactDirection);

			idof.read((char*) &vImpactToMaster           , 3*sizeof(double));
			is.SetImpactToMaster(vImpactToMaster);

			idof.read((char*) &vAngularImpactToMaster    , 3*sizeof(double));
			is.SetAngularImpactToMaster(vAngularImpactToMaster);

			idof.read((char*) &dOverlap                  , sizeof(double));
			is.SetOverlap(dOverlap);

			idof.read((char*) dpudv                      , uNumUDDImpactStatus*sizeof(double));
			idof.read((char*) dpudv+3*uNumUDDImpactStatus, uNumUDDImpactStatus*sizeof(double));

			mapImStatus.insert
                (std::make_pair
                    (std::make_pair(ulElementsMaster, ulElementsSlave), is));
		}
	}
	else
	{
		for (unsigned long ul=0; ul<ulInteractionNumber; ++ul)
		{
			idof.read((char*) &ulElementsMaster          , sizeof(_VEDO_unsigned_long));
			idof.read((char*) &ulElementsSlave           , sizeof(_VEDO_unsigned_long));

			idof.read((char*) &bContact                  , sizeof(bool));
			is.SetContact(bContact);

			idof.read((char*) &bBond                     , sizeof(bool));
			is.SetBond(bBond);

			idof.read((char*) &dKn                       , sizeof(double));
			is.SetKn(dKn);

			idof.read((char*) &dInitialVelocity          , sizeof(double));
			is.SetInitialVelocity(dInitialVelocity);

			idof.read((char*) &vShearForce               , 3*sizeof(double));
			is.SetShearForce(vShearForce);

			idof.read((char*) &vImpactPoint              , 3*sizeof(double));
			is.SetImpactPoint(vImpactPoint);

			idof.read((char*) &vImpactDirection          , 3*sizeof(double));
			is.SetImpactDirection(vImpactDirection);

			idof.read((char*) &vImpactToMaster           , 3*sizeof(double));
			is.SetImpactToMaster(vImpactToMaster);

			idof.read((char*) &vAngularImpactToMaster    , 3*sizeof(double));
			is.SetAngularImpactToMaster(vAngularImpactToMaster);

			idof.read((char*) &dOverlap                  , sizeof(double));
			is.SetOverlap(dOverlap);

			mapImStatus.insert
                (std::make_pair
                    (std::make_pair(ulElementsMaster, ulElementsSlave), is));
		}
	}

	return idof;
};

};   // namespace vedo
