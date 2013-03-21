#include <FrameWork/Interfaces/Constants.h>
#include <FrameWork/Interfaces/IactRecordTab.h>
#include <fstream>

namespace VEDO
{

IactRecordTab::IactRecordTab()
{
}

IactRecordTab::IactRecordTab(const char* filename)
{
	this->ReadIRT(filename);
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
	bool         bContact, bBond;
	double       dKn;
	int          iStage;
	NJR::NJRvector3d  vShearForce;
	ImpactStatus is;

	double dudv[4*VEDO::uNumUserDefinedData];
	for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
		dudv[u] = 0.0;
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

		is.SetAllUserDefinedValue(&dudv[0]);

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
	bool         bContact, bBond;
	double       dKn;
	NJR::NJRvector3d  vShearForce;
	ImpactStatus is;

	double dudv[4*VEDO::uNumUserDefinedData];
	for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
		dudv[u] = 0.0;

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

		is.SetAllUserDefinedValue(&dudv[0]);

		mapImStatus.insert(std::make_pair(pElements, is));
	}
	irtif.close();

	return true;
}

bool IactRecordTab::ReadIRT(const char* filename)
{
	mapImStatus.clear();

	std::ifstream irtif;
	irtif.open(filename, std::ios::in | std::ios::binary);

	if (!irtif.is_open())
	{
		std::cout << "IRT read file error" << std::endl;
		return false;
	}

    std::string sPublish;
    irtif.read((char*) &sPublish, sizeof(std::string));

	unsigned long ulInteractionNumber;
	irtif.read( (char*) &ulInteractionNumber, sizeof(unsigned long) );

	std::pair<unsigned long, unsigned long> pElements;
	bool         bContact, bBond;
	double       dKn;
	NJR::NJRvector3d  vShearForce;
	double       dudv[4*VEDO::uNumUserDefinedData];
	ImpactStatus is;

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

		for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
			irtif.read((char*) &dudv[u], sizeof(double));

		is.SetAllUserDefinedValue(&dudv[0]);

		mapImStatus.insert(std::make_pair(pElements, is));
	}
	irtif.close();

	return true;
}

void IactRecordTab::WriteIRT(const char* filename) const
{
	std::ofstream irtof;
	irtof.open(filename, std::ios::out | std::ios::binary);

	irtof.write((char*) (&VEDO::sPublish), sizeof(std::string));

	unsigned long ulInteractionNumber = mapImStatus.size();
	irtof.write((char*) (&ulInteractionNumber), sizeof(unsigned long));

	bool   bContact, bBond;
	double dKn;
	const double* cdpudv;
	NJR::NJRvector3d vShearForce;
	std::map<std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator iter;
	for (iter=mapImStatus.begin(); iter!=mapImStatus.end(); ++iter)
	{
		irtof.write( (char*) &(iter->first)      , 2*sizeof(unsigned long));

		bContact = iter->second.Contact();
		irtof.write( (char*) &bContact           , sizeof(bool));

		bBond = iter->second.Bond();
		irtof.write( (char*) &bBond              , sizeof(bool));

		dKn = iter->second.Kn();
		irtof.write( (char*) &dKn                , sizeof(double));

        vShearForce = iter->second.ShearForce();
		irtof.write( (char*) &vShearForce        , 3*sizeof(double));

		cdpudv = iter->second.RetrieveAllUserDefinedValue();
		for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
			irtof.write( (char*) (cdpudv+u), sizeof(double));
	}
	irtof.close();
}

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
	NJR::NJRvector3d   vShearForce, vUDV;
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

void IactRecordTab::WriteTextIRT(const char* filename) const
{
	std::ofstream irtof(filename);
	//irtof.open (filename, std::ios::out | std::ios::binary);

	std::map< std::pair<unsigned long, unsigned long>, ImpactStatus>::const_iterator iter;
	unsigned long ulInteractionNumber = mapImStatus.size();
	//irtof.write( (char*) (&tabSize) , sizeof(unsigned long) );
	irtof
		<< "Interactions=" << ulInteractionNumber << std::endl
		<< "Master discrete object ID, Slave discrete object ID, "
		<< "Contact, Bond, Normal Stiffness, "
		<< "Shear force (x), Shear force (y), Shear force (z)";

	for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
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
		for(unsigned u=0; u<4*VEDO::uNumUserDefinedData; u++)
			irtof << ", " << *(cdpudv+u);

		irtof << std::endl;
	}
	irtof.close();
};

};   // namespace VEDO
