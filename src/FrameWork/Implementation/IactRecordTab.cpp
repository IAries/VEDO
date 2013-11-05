#include <NJR/Interfaces/Utility.h>
#include <Framework/Interfaces/Constants.h>
#include <Framework/Interfaces/IactRecordTab.h>
#include <cstring>
#include <fstream>

namespace vedo
{

IactRecordTab::IactRecordTab()
{
}

IactRecordTab::IactRecordTab
	(const std::map<std::pair<unsigned long,unsigned long>, ImpactStatus>& m)
{
	mapImStatus = m;
}

bool IactRecordTab::ReadIRT2010(const char* filename)
{
	mapImStatus.clear();

	std::ifstream irtif;
	irtif.open(filename, std::ios::in | std::ios::binary);

	if (!irtif.is_open())
	{
		std::cout << "IRT read file error" << std::endl;
		return false;
	}

	unsigned long ulInteractionNumber;
	std::pair<unsigned long,unsigned long> pElements;
	bool          bContact, bBond;
	double        dKn;
	int           iStage;
	njr::Vector3d vShearForce;
	ImpactStatus  is;

	irtif.read( (char*) &ulInteractionNumber, sizeof(unsigned long) );
	for(unsigned long ul=0; ul<ulInteractionNumber; ++ul)
	{
        irtif.read((char*) &pElements  , 2*sizeof(unsigned long));

		bContact = true;
		is.SetContact(bContact);
		dKn = 0.0;
		is.SetKn(dKn);

		irtif.read((char*) &vShearForce, 3*sizeof(double));
		is.SetShearForce(vShearForce);

		irtif.read((char*) &iStage, sizeof(int));
		if (iStage == 1)
		{
			bBond = true;
		}
		else
		{
			bBond = false;
		}

		is.SetBond(bBond);

		mapImStatus.insert(std::make_pair(pElements, is));
	}
	irtif.close();

	return true;
}

bool IactRecordTab::ReadIRT2011(const char* filename)
{
	mapImStatus.clear();

	std::ifstream irtif;
	irtif.open(filename, std::ios::in | std::ios::binary);

	if (!irtif.is_open())
	{
		std::cout << "IRT read file error" << std::endl;
		return false;
	}

	unsigned long ulInteractionNumber;
	std::pair<unsigned long,unsigned long> pElements;
	bool          bContact, bBond;
	double        dKn;
	int           iStage;
	njr::Vector3d vShearForce;
	ImpactStatus  is;

	irtif.read( (char*) &ulInteractionNumber, sizeof(unsigned long) );
	for (unsigned long ul=0; ul<ulInteractionNumber; ++ul)
	{
        irtif.read((char*) &pElements  , 2*sizeof(unsigned long));

		irtif.read((char*) &bContact   , sizeof(bool));
		is.SetContact(bContact);

		irtif.read((char*) &bBond      , sizeof(bool));
		is.SetBond(bBond);

		irtif.read((char*) &dKn        , sizeof(double));
		is.SetKn(dKn);

		irtif.read((char*) &vShearForce, 3*sizeof(double));
		is.SetShearForce(vShearForce);

		mapImStatus.insert(std::make_pair(pElements, is));
	}
	irtif.close();

	return true;
};

const ImpactStatus* IactRecordTab::GetImpactStatus
	(unsigned long master,unsigned long slave)
{
	std::map<std::pair<unsigned long,unsigned long>, ImpactStatus>::const_iterator iter
		= mapImStatus.find(std::make_pair(master, slave));
	return (iter != mapImStatus.end()) ? &(iter->second) : 0;
}

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
			<< iter->second.ShearForce()
			<< std::endl;
	}
}

/*
bool IactRecordTab::ReadTextIRT(const char* filename)
{
	mapImStatus.clear();

	std::ifstream irtif(filename);
	//irtif.open(filename, std::ios::in | std::ios::binary);

	if (!irtif.is_open())
	{
		std::cout << "IRT read file error" << std::endl;
		return false;
	}

	unsigned long ulInteractionNumber, ulElement1, ulElement2;
	bool          bContact, bBond;
	njr::Vector3d   vShearForce, vUDV;
	double        dKn, dVx, dVy, dVz, dUDV, vUDVx, vUDVy, vUDVz;
	ImpactStatus  is;

	irtif >> ulInteractionNumber;
	for (unsigned long ul=0; ul<ulInteractionNumber; ++ul)
	{
		irtif
			>> ulElement1 >> ulElement2
			>> bContact
			>> bBond
			>> dKn
			>> dVx >> dVy >> dVz;

		is.SetContact(bContact);
		is.SetBond(bBond);
		is.SetKn(dKn);
		vShearForce.Set(dVx, dVy, dVz);
		is.SetShearForce(vShearForce);

		for(unsigned u=0; u<6; u++)
		{
			irtif >> dUDV;
			is.SetUserDefinedValue(u, dUDV);
		}

		for(unsigned u=0; u<6; u++)
		{
			irtif >> dUDV;
			is.SetAccumulativeUserDefinedValue(u, dUDV);
		}

		for(unsigned u=0; u<3; u++)
		{
			irtif >> vUDVx >> vUDVy >> vUDVz;
			vUDV.Set(vUDVx, vUDVy, vUDVz);
			is.SetUserDefinedVector3d(u, vUDV);
		}

		for(unsigned u=0; u<3; u++)
		{
			irtif >> vUDVx >> vUDVy >> vUDVz;
			vUDV.Set(vUDVx, vUDVy, vUDVz);
			is.SetAccumulativeUserDefinedVector3d(u, vUDV);
		}

		mapImStatus.insert(std::make_pair(std::make_pair(ulElement1, ulElement2), is));
	}
	irtif.close();
	return true;
}
*/

void IactRecordTab::WriteCSV(std::string filename) const
{
	std::ofstream irtof(filename.c_str(), std::ios::out);

	std::map< std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator iter;
	unsigned long ulInteractionNumber = mapImStatus.size();
	//irtof.write( (char*) (&tabSize) , sizeof(unsigned long) );
	irtof
		<< "Master discrete object ID, Slave discrete object ID, "
		<< "Contact, Bond, Normal Stiffness, "
		<< "Shear force (x), Shear force (y), Shear force (z)";

	for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
		irtof << ", User-defined value " << u+1;

	irtof << std::endl;

	const double* cdpudv;
	for (iter=mapImStatus.begin(); iter!=mapImStatus.end(); ++iter)
	{
		irtof
			<< iter->first.first
			<< ", " << iter->first.second
			<< ", " << iter->second.Contact()
			<< ", " << iter->second.Bond()
			<< ", " << iter->second.Kn()
			<< ", " << iter->second.ShearForce().x()
			<< ", " << iter->second.ShearForce().y()
			<< ", " << iter->second.ShearForce().z();

		cdpudv = iter->second.RetrieveAllUserDefinedValue();
		for(unsigned u=0; u<4*uNumUDDImpactStatus; u++)
			irtof << ", " << *(cdpudv+u);

		irtof << std::endl;
	}
	irtof.close();
};

std::ofstream& IactRecordTab::operator >> (std::ofstream& idof) const
{
    unsigned long ulSize = mapImStatus.size();
	idof.write((char*) &ulSize, sizeof(unsigned long));

	const ImpactStatus* isp;
	bool                bTemp;
	double              dTemp;
	njr::Vector3d       vTemp;

	if(uNumUDDImpactStatus != 0)
	{
		const double*       dpUDV;
		for(std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator mapImStatusP = mapImStatus.begin();
			mapImStatusP != mapImStatus.end();
			mapImStatusP++                                                                                              )
		{
			idof.write((char*) &(mapImStatusP->first)             , 2*sizeof(unsigned long));

			isp   = &(mapImStatusP->second);
			bTemp = isp->Contact();
			idof.write((char*) &bTemp                             , sizeof(bool));
			bTemp = isp->Bond();
			idof.write((char*) &bTemp                             , sizeof(bool));
			dTemp =isp->Kn();
			idof.write((char*) &dTemp                             , sizeof(double));
			vTemp = isp->ShearForce();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
			dpUDV = isp->RetrieveAllUserDefinedValue();
			idof.write((char*) dpUDV                              , uNumUDDImpactStatus*sizeof(double));
			idof.write((char*) (dpUDV+3*vedo::uNumUDDImpactStatus), uNumUDDImpactStatus*sizeof(double));
		}
	}
	else
	{
		for(std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator mapImStatusP = mapImStatus.begin();
			mapImStatusP != mapImStatus.end();
			mapImStatusP++                                                                                              )
		{
			idof.write((char*) &(mapImStatusP->first)             , 2*sizeof(unsigned long));

			isp   = &(mapImStatusP->second);
			bTemp = isp->Contact();
			idof.write((char*) &bTemp                             , sizeof(bool));
			bTemp = isp->Bond();
			idof.write((char*) &bTemp                             , sizeof(bool));
			dTemp =isp->Kn();
			idof.write((char*) &dTemp                             , sizeof(double));
			vTemp = isp->ShearForce();
			idof.write((char*) &vTemp                             , 3*sizeof(double));
		}
	}

	return idof;
};

std::ifstream& IactRecordTab::operator << (std::ifstream& idof)
{
	mapImStatus.clear();
	unsigned long ulInteractionNumber;
	idof.read( (char*) &ulInteractionNumber, sizeof(unsigned long) );

	std::pair<unsigned long, unsigned long> pElements;
	bool          bContact, bBond;
	double        dKn;
	njr::Vector3d vShearForce;
	ImpactStatus  is;

	if(uNumUDDImpactStatus != 0)
	{
		double* dpudv = new double[4*vedo::uNumUDDImpactStatus];
		memcpy(dpudv, is.RetrieveAllUserDefinedValue(), 4*uNumUDDImpactStatus*sizeof(double));
		for (unsigned long ul=0; ul<ulInteractionNumber; ++ul)
		{
			idof.read((char*) &pElements                 , 2*sizeof(unsigned long));

			idof.read((char*) &bContact                  , sizeof(bool));
			is.SetContact(bContact);

			idof.read((char*) &bBond                     , sizeof(bool));
			is.SetBond(bBond);

			idof.read((char*) &dKn                       , sizeof(double));
			is.SetKn(dKn);

			idof.read((char*) &vShearForce               , 3*sizeof(double));
			is.SetShearForce(vShearForce);

			idof.read((char*) dpudv                      , uNumUDDImpactStatus*sizeof(double));
			idof.read((char*) dpudv+3*uNumUDDImpactStatus, uNumUDDImpactStatus*sizeof(double));

			mapImStatus.insert(std::make_pair(pElements, is));
		}
	}
	else
	{
		for (unsigned long ul=0; ul<ulInteractionNumber; ++ul)
		{
			idof.read((char*) &pElements                 , 2*sizeof(unsigned long));

			idof.read((char*) &bContact                  , sizeof(bool));
			is.SetContact(bContact);

			idof.read((char*) &bBond                     , sizeof(bool));
			is.SetBond(bBond);

			idof.read((char*) &dKn                       , sizeof(double));
			is.SetKn(dKn);

			idof.read((char*) &vShearForce               , 3*sizeof(double));
			is.SetShearForce(vShearForce);

			mapImStatus.insert(std::make_pair(pElements, is));
		}
	}

	return idof;
};


};   // namespace vedo
