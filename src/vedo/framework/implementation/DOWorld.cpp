#include <vedo/njr/interfaces/Constants.h>
#include <vedo/njr/interfaces/Utility.h>
#include <vedo/framework/interfaces/Boundary.h>
#include <vedo/framework/interfaces/DOMap.h>
#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/DOModel.h>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

namespace vedo
{

DOWorld::DOWorld(): cDOStatus(0), cDOModel(0), cIactModel(0), pSystemParameter(0)
{
}

DOWorld::DOWorld(const DOWorld &dow): cDOStatus(0), cDOModel(0), cIactModel(0), pSystemParameter(0)
{
	*this = dow;
}

DOWorld::DOWorld
	(SystemParameter *sp,
	 const std::list<DOModel*>& nDOModel,
	 const std::list<IactModel*>& nIactModel,
	 const std::vector<DOStatus*>& nDOStatus )
{
	pSystemParameter = sp;
	cDOModel         = nDOModel;
	cIactModel       = nIactModel;
	cDOStatus        = nDOStatus;
	DOWorld::Check();
}

const DOWorld& DOWorld::operator = (const DOWorld& dow)
{
	DOWorld::Clear();
	pSystemParameter = new SystemParameter( *dow.pSystemParameter );
	transform(dow.cDOModel.begin(), dow.cDOModel.end(), back_inserter(cDOModel), njr::Copy_obj());
	transform(dow.cIactModel.begin(), dow.cIactModel.end(), back_inserter(cIactModel), njr::Copy_obj());
	transform(dow.cDOStatus.begin(), dow.cDOStatus.end(), back_inserter(cDOStatus), njr::Copy_obj());
	DOWorld::Check();
	return *this;
}

DOWorld::~DOWorld()
{
	DOWorld::Clear();
}

std::string DOWorld::GetDOName(const vedo::vedo_uint_t& odo) const
{
	if (odo < pSystemParameter->GetDONumber())
	{
		return cDOStatus[odo]->GetDOName();
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOWorld::GetDOName(const vedo::vedo_uint_t&) const" << std::endl
			<< "        Note: Element's ID wrong"                                 << std::endl;
		exit(-1);
	}
}

const DOStatus* DOWorld::GetDOStatus(const vedo::vedo_uint_t& odo) const
{
	return(odo < pSystemParameter->GetDONumber()) ? cDOStatus[odo] : 0;
}

void DOWorld::SetDOStatus(const vedo::vedo_uint_t& odo, const DOStatus& dos)
{
	if (odo < pSystemParameter->GetDONumber())
	{
		*(cDOStatus[odo]) = dos;
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOWorld::SetDOStatus(const vedo::vedo_uint_t&, const DOStatus&)" << std::endl
			<< "        Note: Element's ID wrong"                                              << std::endl;
		exit(-1);
	}
}

void DOWorld::SetDOStatusVelocityAndAngularVelocity
	(const vedo::vedo_uint_t& odo, const njr::Vector3d& vVelocity, const njr::Vector3d& vAngularVelocity)
{
	if (odo < pSystemParameter->GetDONumber())
	{
		cDOStatus[odo]->SetVelocity(vVelocity);
		cDOStatus[odo]->SetAngularVelocity(vAngularVelocity);
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOWorld::SetDOStatusVelocityAndAngularVelocity(const vedo::vedo_uint_t&, const DOStatus&)" << std::endl
			<< "        Note: Element's ID wrong" << std::endl;
		exit(-1);
	}
}

/*
void DOWorld::EraseDOStatus(const std::vector<vedo::vedo_uint_t>& EraseList, const vedo_int_t& rank)
{
	std::vector<DOStatus*>::size_type numberDO      = cDOStatus.size();
	std::vector<DOStatus*>::size_type ErasenumberDO = EraseList.size();
	std::vector<DOStatus*>::size_type NewnumberDO   = numberDO - ErasenumberDO;
	DOStatus** NewcDOStatus = new DOStatus* [NewnumberDO];

	std::vector<DOStatus*>::size_type ul;
	std::vector<DOStatus*>::size_type ulEraseCounter = 0;

	for(ul=0; ul<numberDO; ul++)
	{
		if(ulEraseCounter < ErasenumberDO)
		{
			if(ul < EraseList[ulEraseCounter])
			{
				NewcDOStatus[ul-ulEraseCounter] = cDOStatus[ul];
			}
			else
			{
				cDOStatus[ul]->~DOStatus();
				ulEraseCounter++;
			}
		}
		else
		{
			NewcDOStatus[ul-ulEraseCounter] = cDOStatus[ul];
		}
	}

	cDOStatus.clear();
	for(ul=0; ul<NewnumberDO; ul++)
	{
		cDOStatus.push_back(NewcDOStatus[ul]);
	}
	delete[] NewcDOStatus;
}
*/

const std::vector<vedo::vedo_uint_t> DOWorld::FindDO (std::string name) const
{
	std::vector<vedo::vedo_uint_t> List;
	std::vector<DOStatus*>::iterator cDOSp;
	for (vedo::vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		if (cDOStatus[ul]->GetDOName() == name)
		{
			List.push_back(ul);
		}
	}
	return List;
}

void DOWorld::TurnMonitor(bool m)
{
	for (vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]->SetMonitored(m);
	}
}

//void DOWorld::ClearSeparatedElements(const IactRecordTab* pIactRecordTab)
void DOWorld::ClearSeparatedElements()
{
	std::vector<vedo::vedo_uint_t> RedundantElement;
	vedo::vedo_uint_t numberDO = pSystemParameter->GetDONumber();
	const DOStatus* pdos  = 0;

	vedo::vedo_uint_t ul;
	std::list<DOModel*>::const_iterator idoml;
	std::list<IactModel*>::const_iterator cimp;
	for (ul=0; ul<numberDO; ul++)
	{
		cDOStatus[ul]->SetMonitored(true);
	}
	for (cimp = cIactModel.begin(); cimp != cIactModel.end(); cimp++)
	{
	}

	for (ul=0; ul<numberDO; ul++)
	{
		if (!(pdos->ISMonitored()))
		{
			RedundantElement.push_back(ul);
		}
	}
	if (!RedundantElement.empty())
	{
		std::cout << "Number of redundant elements = " << RedundantElement.size() << std::endl;
//		cDO.Erase(RedundantElement);
//		pDOWorld->EraseDOStatus(RedundantElement);
//		vcIactMaster.clear();
//		vcIactSlave.clear();
//		IactPairTab.clear();
//		cIact.Clear();
	}
}

void DOWorld::EraseDOStatus(const vedo::vedo_uint_t& ulID)
{
	cDOStatus.erase(cDOStatus.begin()+ulID);
	pSystemParameter->SetDONumber(cDOStatus.size());
}

void DOWorld::EraseDOStatus(const std::vector<vedo::vedo_uint_t>& EraseList)
{
	if(!EraseList.empty())
	{
		const vedo::vedo_uint_t numberDO      = cDOStatus.size();
		const vedo::vedo_uint_t ErasenumberDO = EraseList.size();
		vedo::vedo_uint_t ulEraseCounter = 0;
		for (vedo::vedo_uint_t ul=EraseList[0]; ul<numberDO; ul++)
		{
			if (ulEraseCounter < ErasenumberDO)
			{
				if (ul < EraseList[ulEraseCounter])
				{
					cDOStatus[ul-ulEraseCounter] = cDOStatus[ul];
				}
				else
				{
					ulEraseCounter++;
				}
			}
			else
			{
				cDOStatus[ul-ulEraseCounter] = cDOStatus[ul];
			}
		}
		cDOStatus.resize(numberDO-ErasenumberDO);
		pSystemParameter->SetDONumber(numberDO-ErasenumberDO);
	}
}

/*
void DOWorld::EraseDOStatus
	(const std::vector<vedo::vedo_uint_t>& EraseList, const vedo_int_t& rank)
{
	const vedo::vedo_uint_t numberDO      = cDOStatus.size();
	const vedo::vedo_uint_t ErasenumberDO = EraseList.size();
	const vedo::vedo_uint_t NewnumberDO   = numberDO - ErasenumberDO;
	std::vector<DOStatus*> NewcDOStatus(NewnumberDO);

	vedo::vedo_uint_t ul;
	vedo::vedo_uint_t ulEraseCounter = 0;
	for(ul=0; ul<numberDO; ul++)
	{
		if(ulEraseCounter < ErasenumberDO)
		{
			if(ul < EraseList[ulEraseCounter])
			{
				NewcDOStatus[ul-ulEraseCounter] = cDOStatus[ul];
			}
			else
			{
				ulEraseCounter++;
			}
		}
		else
		{
			NewcDOStatus[ul-ulEraseCounter] = cDOStatus[ul];
		}
		cDOStatus[ul]->~DOStatus();
	}
	cDOStatus.clear();
	cDOStatus.resize(NewnumberDO);
	for(ul=0; ul<NewnumberDO; ul++)
	{
		cDOStatus[ul] = NewcDOStatus[ul];
		NewcDOStatus[ul]->~DOStatus();
	}
	NewcDOStatus.clear();
}
*/

const DOModel* DOWorld::GetDOModel(const std::string& DOName) const
{
	std::list<DOModel*>::const_iterator idoml;
	idoml = find_if(cDOModel.begin(), cDOModel.end(), DOModel(DOName));
	return (idoml != cDOModel.end()) ? *idoml : 0;
}

const IactModel* DOWorld::GetIactModel(const std::string& MG, const std::string& SG) const
{
	std::list<IactModel*>::const_iterator iiactml;
	iiactml = find_if(cIactModel.begin(), cIactModel.end(), IactModel(MG, SG));
	return (iiactml != cIactModel.end()) ? *iiactml : 0;
}

bool DOWorld::UpdateDOStatus(const std::vector<const DOStatus *>& nDOStatus)
{
	if (cDOStatus.size() != nDOStatus.size())
	{
		std::cout << "Warning!!! DO(" << cDOStatus.size() << ")don't equal to->" << nDOStatus.size() << std::endl;\
	}

	std::vector<DOStatus *> vnDOS;
	transform(nDOStatus.begin(), nDOStatus.end(), back_inserter(vnDOS), njr::Copy_obj());
	for_each(cDOStatus.begin(), cDOStatus.end(), njr::Delete_ptr());
	cDOStatus = vnDOS;
	pSystemParameter->SetDONumber(cDOStatus.size());

	return DOWorld::Check();
}

bool DOWorld::DelDOModel(const std::string& DOName)
{
	std::list<DOModel*>::iterator idoml;
    std::vector<DOStatus*>::iterator idos;
	std::vector<DOStatus*>cnDOStatus(0);

	idoml = find_if(cDOModel.begin(),cDOModel.end(),DOModel(DOName));

	if (idoml == cDOModel.end())
	{
		return false;
	}

	delete (*idoml);
	cDOModel.erase(idoml);

	for (idos = cDOStatus.begin(); idos != cDOStatus.end(); ++idos)
	{
		if ((*idos)->GetDOName() != DOName)
		{
			cnDOStatus.push_back(*idos);
		}
		else
		{
			delete *idos;
		}
	}

	cDOStatus = cnDOStatus;
	pSystemParameter->SetDONumber(cDOStatus.size());

	return DOWorld::Check();
}

void DOWorld::SetFieldAcceleration(const njr::Vector3d& ff)
{
	pSystemParameter->SetFieldAcceleration(ff);
}

void DOWorld::SetModels(const DOWorld* dow)
{
	// Set pSystemParameter
	pSystemParameter = dow->GetSystemParameter();
	pSystemParameter->SetDONumber(cDOStatus.size());

	// Set cDOModel
	std::list<DOModel*>::iterator cdmp;
	for (cdmp=(cDOModel.begin()); cdmp!=(cDOModel.end()); cdmp++)
	{
		if ((dow->GetDOModel((*cdmp)->GetDOName())))
		{
			*cdmp = new DOModel(*(dow->GetDOModel((*cdmp)->GetDOName())));
		}
	}

	// Set cIactModel
	std::list<IactModel*>::iterator cimp;
	for (cimp=(cIactModel.begin()); cimp!=(cIactModel.end()); cimp++)
	{
		if (dow->GetIactModel((*cimp)->GetMasterDOGroup(), (*cimp)->GetSlaveDOGroup()))
		{
			*cimp = new IactModel(*(dow->GetIactModel((*cimp)->GetMasterDOGroup(), (*cimp)->GetSlaveDOGroup())));
		}
	}
}

void DOWorld::FreezeAllElements()
{
	for (vedo::vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
//		cDOStatus[ul]->SetOrientation(njr::Vector3d(AXIALX), njr::Vector3d(AXIALZ));
		cDOStatus[ul]->SetVelocity(njr::ZERO);
		cDOStatus[ul]->SetAngularVelocity(njr::ZERO);
	}
}

void DOWorld::FreezeElements(std::string& sDOName)
{
	for (vedo::vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		if (cDOStatus[ul]->GetDOName() == sDOName)
		{
//			cDOStatus[ul]->SetOrientation(njr::AXIALX, njr::AXIALZ));
			cDOStatus[ul]->SetVelocity(njr::ZERO);
			cDOStatus[ul]->SetAngularVelocity(njr::ZERO);
		}
	}
}

void DOWorld::SortingDOStatus()
{
	std::vector<DOStatus*> cDOStatusNew;
	std::string str;
	std::vector<std::string>              cDOStatusTempName;
	std::vector< std::vector<DOStatus*> > cDOStatusTempData;
	bool hasInsert;
	DOStatus* dos;
	for (vedo::vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		dos = cDOStatus[ul];
		str = dos->GetDOName();
		hasInsert = false;
		for (vedo::vedo_uint_t ul=0; ul<cDOStatusTempName.size(); ul++)
		{
			if (cDOStatusTempName[ul] == str)
			{
				cDOStatusTempData[ul].push_back(new DOStatus(*dos));
				hasInsert = true;
			}
		}
		if (!hasInsert)
		{
			cDOStatusTempName.push_back(str);
			std::vector<DOStatus*> vdos;
			vdos.push_back(new DOStatus(*dos));
			cDOStatusTempData.push_back(vdos);
		}
	}
	for (vedo::vedo_uint_t ul=0; ul<cDOStatusTempName.size(); ul++)
	{
		for (vedo::vedo_uint_t vl=0; vl<(cDOStatusTempData[ul]).size(); vl++)
		{
			cDOStatusNew.push_back(new DOStatus(*(cDOStatusTempData[ul][vl])));
		}

	}
	cDOStatus = cDOStatusNew;
}

void DOWorld::Clear()
{
	if (pSystemParameter != 0)
	{
		delete pSystemParameter;
		pSystemParameter = 0;
	}

	for_each(cDOModel.begin()  , cDOModel.end()  , njr::Delete_ptr());
	for_each(cIactModel.begin(), cIactModel.end(), njr::Delete_ptr());
	for_each(cDOStatus.begin() , cDOStatus.end() , njr::Delete_ptr());

	cDOModel.clear();
	cIactModel.clear();
	cDOStatus.clear();
}

bool DOWorld::IsEmpty() const
{
	if (pSystemParameter == 0)
	{
		return true;
	}

	if (cDOStatus.size() == 0)
	{
		return true;
	}

	return false;
}

void DOWorld::CalculateSystemEnergy()
{
	vedo_float_t    dEnergySystem;
	vedo_float_t    dEnergyPotential;
	vedo_float_t    dEnergyKinetic;
	vedo_float_t    dEnergyTranslation;
	vedo_float_t    dEnergyRotation;
	njr::Vector3d   vMomentumAvg;
	njr::Vector3d   vAngularMomentumAvg;
	vedo_float_t    dMomentumNorm        = 0.0;
	vedo_float_t    dAngularMomentumNorm = 0.0;
	vedo_float_t    dVelocityMax;
	vedo_float_t    dVelocityMin;
	vedo_float_t    dAngularVelocityMax;
	vedo_float_t    dAngularVelocityMin;
	vedo_float_t    m;
	njr::Vector3d   mmi;
	vedo_float_t    v;
	njr::Vector3d   av;
	vedo_float_t    dav;
	bool            StartPoint           = true;
	bool            NoMobileElements     = false;
	vedo_uint_t     counter              = 0;
	vedo_uint_t     NumMobileDO          = 0;
	const DOModel*  dom                  = 0;
	const DOStatus* dos                  = 0;

	// Find "Start Point"
	while (StartPoint)
	{
		dos = GetDOStatus(counter);
		dom = GetDOModel(dos->GetDOName());
		if (dom->GetBehavior() == mobile)
		{
			m                   = dom->GetMass();
			mmi                 = dom->GetMassMomentInertia();
			v                   = dos->GetVelocity().length();
			av                  = dos->GetAngularVelocity();
			vMomentumAvg        = m * dos->GetVelocity();
			vAngularMomentumAvg = mmi * av;
			dEnergyPotential    = -m * (pSystemParameter->GetFieldAcceleration() % dos->GetPosition());
			dEnergyTranslation  = 0.5 * m * v * v;
			dEnergyRotation     = 0.5 * (mmi.x() * av.x() * av.x() +  mmi.y() * av.y() * av.y() +  mmi.z() * av.z() * av.z());
			dEnergyKinetic      = dEnergyTranslation + dEnergyRotation;
			dEnergySystem       = dEnergyPotential + dEnergyKinetic;
			dVelocityMax        = v;
			dVelocityMin        = v;
			dAngularVelocityMax = av.length();
			dAngularVelocityMin = dAngularVelocityMax;
			NumMobileDO         = 1;
			StartPoint          = false;
		}
		else
		{
			counter++;
		}

		if (counter == pSystemParameter->GetDONumber())
		{
			NumMobileDO      = 0;
			StartPoint       = false;
			NoMobileElements = true;
		}
	}

	if (!NoMobileElements)
	{
		for (vedo_uint_t i=counter+1; i<(pSystemParameter->GetDONumber()); i++)
		{
			dos = GetDOStatus(i);
			dom = GetDOModel(dos->GetDOName());
			if (dom->GetBehavior() == mobile)
			{
				NumMobileDO++;
				m                    = dom->GetMass();
				mmi                  = dom->GetMassMomentInertia();
				v                    = dos->GetVelocity().length();
				av                   = dos->GetAngularVelocity();
				dav                  = av.length();
				vMomentumAvg        += m * dos->GetVelocity();
				vAngularMomentumAvg += mmi * av;
				dEnergyPotential    -= m * (pSystemParameter->GetFieldAcceleration() % dos->GetPosition());
				dEnergyTranslation  += 0.5 * m * v * v;
				dEnergyRotation     += 0.5 * (mmi.x() * av.x() * av.x() +  mmi.y() * av.y() * av.y() +  mmi.z() * av.z() * av.z());
				dEnergyKinetic       = dEnergyTranslation + dEnergyRotation;
				dEnergySystem        = dEnergyPotential + dEnergyKinetic;

				if (v > dVelocityMax)
				{
					dVelocityMax = v;
				}
				else if (v < dVelocityMin)
				{
					dVelocityMin = v;
				}

				if (dav > dAngularVelocityMax)
				{
					dAngularVelocityMax = dav;
				}
				else if (dav < dAngularVelocityMin)
				{
					dAngularVelocityMin = dav;
				}
			}
		}
	}
	else
	{
		dEnergySystem       = 0.0;
		dEnergyKinetic      = 0.0;
		dEnergyPotential    = 0.0;
		dEnergyTranslation  = 0.0;
		dEnergyRotation     = 0.0;
		dVelocityMax        = 0.0;
		dVelocityMin        = 0.0;
		dAngularVelocityMax = 0.0;
		dAngularVelocityMin = 0.0;
	}

	vMomentumAvg        = 1.0 / NumMobileDO * vMomentumAvg;
	vAngularMomentumAvg = 1.0 / NumMobileDO * vAngularMomentumAvg;

	// Calculate the "Norm" of momentum and angular momentun
	if (!NoMobileElements)
	{
		for (vedo_uint_t i=counter; i<(pSystemParameter->GetDONumber()); i++)
		{
			dos = GetDOStatus(i);
			dom = GetDOModel(dos->GetDOName());
			if (dom->GetBehavior() == mobile)
			{
				m                     = dom->GetMass();
				mmi                   = dom->GetMassMomentInertia();
				v                     = dos->GetVelocity().length();
				av                    = dos->GetAngularVelocity();
				dMomentumNorm        += (m * dos->GetVelocity() - vMomentumAvg).length();
				dAngularMomentumNorm += (mmi * av - vAngularMomentumAvg).length();
			}
		}
	}
	else
	{
//		dMomentumNorm        = 0.0;
//		dAngularMomentumNorm = 0.0;
	}

	pSystemParameter->SetEnergyPotential(dEnergyPotential);
	pSystemParameter->SetEnergyTranslation(dEnergyTranslation);
	pSystemParameter->SetEnergyRotation(dEnergyRotation);
	pSystemParameter->SetMomentumAvg(vMomentumAvg);
	pSystemParameter->SetAngularMomentumAvg(vAngularMomentumAvg);
	pSystemParameter->SetMomentumNorm(dMomentumNorm);
	pSystemParameter->SetAngularMomentumNorm(dAngularMomentumNorm);
	pSystemParameter->SetVelocityMax(dVelocityMax);
	pSystemParameter->SetVelocityMin(dVelocityMin);
	pSystemParameter->SetAngularVelocityMax(dAngularVelocityMax);
	pSystemParameter->SetAngularVelocityMin(dAngularVelocityMin);
}

/*
const std::pair<njr::Vector3d, njr::Vector3d>
	DOWorld::Distribution(vedo_float_t& dMeshLength) const
{
	const Boundary      pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*     pdos    = 0;
	const DOModel*      pdoml   = 0;
	      vedo::vedo_uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<vedo::vedo_uint_t> TargetedSerialNumber;
	for(vedo::vedo_uint_t ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
			}
		}
	}

	vedo_float_t        dXMin = pZOI.GetUpperPoint().x();
	vedo_float_t        dYMin = pZOI.GetUpperPoint().y();
	vedo_float_t        dZMin = pZOI.GetUpperPoint().z();
	vedo_float_t        dXMax = pZOI.GetLowerPoint().x();
	vedo_float_t        dYMax = pZOI.GetLowerPoint().y();
	vedo_float_t        dZMax = pZOI.GetLowerPoint().z();
	vedo_float_t        dRadius;
	vedo::vedo_uint_t ulCounter;
	bool          bMeshFilled;
	njr::Vector3d   vMeshCenter;
	for(vedo_float_t dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength;
		dX<pZOI.GetUpperPoint().x();
		dX+=dMeshLength                                   )
	{
		for(vedo_float_t dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength;
			dY<pZOI.GetUpperPoint().y();
			dY+=dMeshLength                                   )
		{
			for(vedo_float_t dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength;
				dZ<pZOI.GetUpperPoint().z();
				dZ+=dMeshLength                                   )
			{
				vMeshCenter.Set(dX, dY, dZ);
				ulCounter   = 0;
				bMeshFilled = false;
				while((ulCounter<TargetedSerialNumber.size()) && (!bMeshFilled))
				{
					pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
					pdoml   = GetDOModel(pdos->GetDOName());
					dRadius = pdoml->GetShapeAttributes().sphere.radius;
					if((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
					{
						if(dX>dXMax) dXMax = dX;
						if(dX<dXMin) dXMin = dX;
						if(dY>dYMax) dYMax = dY;
						if(dY<dYMin) dYMin = dY;
						if(dZ>dZMax) dZMax = dZ;
						if(dZ<dZMin) dZMin = dZ;
						bMeshFilled        = true;
					}
					ulCounter++;
				}
			}
		}
	}

	if(dXMax < dXMin)
	{
		dXMin = dXMax;
	}

	if(dYMax < dYMin)
	{
		dYMin = dYMax;
	}

	if(dZMax < dZMin)
	{
		dZMin = dZMax;
	}

	return
		std::make_pair
			(njr::Vector3d(dXMin, dYMin, dZMin),
			 njr::Vector3d(dXMax, dYMax, dZMax) );
}
*/

void DOWorld::Shift(const njr::Vector3d& shift)
{
	for (vedo::vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]->SetPosition(cDOStatus[ul]->GetPosition() + shift);
	}
}

void DOWorld::Shift(const njr::Vector3d& shift, const std::string& DOName)
{
	for (vedo::vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		if (cDOStatus[ul]->GetDOName() == DOName)
		{
			cDOStatus[ul]->SetPosition(cDOStatus[ul]->GetPosition() + shift);
		}
	}
}

std::pair<vedo_float_t, vedo_float_t> Rotate1Axis(vedo_float_t Axis1, vedo_float_t Axis2, vedo_float_t dAnglePlus)
{
	if (dAnglePlus == 0.0)
	{
		return std::make_pair(Axis1, Axis2);
	}
	else
	{
		vedo_float_t dRadius = sqrt(Axis1 * Axis1 + Axis2 * Axis2);
		vedo_float_t dAngle;
		if (Axis1 == 0.0)
		{
			if (Axis2 > 0.0)
			{
				dAngle = 1.570796335;
			}
			else
			{
				dAngle = -1.570796335;
			}
		}
		else
		{
			dAngle = atan(Axis2 / Axis1);
		}

		if ((Axis1 < 0.0) && (Axis2 > 0.0))
		{
			dAngle += njr::dPI;
		}
		else if ((Axis1 < 0.0) && (Axis2 < 0.0))
		{
			dAngle += njr::dPI;
		}

		dAngle += dAnglePlus;

		if (dAngle >= njr::dDoublePI)
		{
			dAngle -= njr::dDoublePI;
		}

		return std::make_pair(dRadius*cos(dAngle), dRadius*sin(dAngle));
	}
}

njr::Vector3d Rotate3Axis
	(njr::Vector3d vTarget, const vedo_float_t& Angle2XAxis, const vedo_float_t& Angle2YAxis, const vedo_float_t& Angle2ZAxis)
{
	std::pair<vedo_float_t, vedo_float_t> pNewAxisValues;

	pNewAxisValues = Rotate1Axis(vTarget.y(), vTarget.z(), Angle2XAxis);
	vTarget.Set(vTarget.x(), pNewAxisValues.first, pNewAxisValues.second);

	pNewAxisValues = Rotate1Axis(vTarget.z(), vTarget.x(), Angle2YAxis);
	vTarget.Set(pNewAxisValues.second, vTarget.y(), pNewAxisValues.first);

	pNewAxisValues = Rotate1Axis(vTarget.x(), vTarget.y(), Angle2ZAxis);
	vTarget.Set(pNewAxisValues.first, pNewAxisValues.second, vTarget.z());

	return vTarget;
}

void DOWorld::Rotate(const vedo_float_t& Angle2XAxis, const vedo_float_t& Angle2YAxis, const vedo_float_t& Angle2ZAxis)
{
	njr::Vector3d vV1, vV2;
	std::pair<vedo_float_t, vedo_float_t> pNewPosition, pNewVelocity, pNewAngularVelocity;
	for (vedo::vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]->SetPosition(Rotate3Axis(cDOStatus[ul]->GetPosition(), Angle2XAxis, Angle2YAxis, Angle2ZAxis));
		vV1 = Rotate3Axis(cDOStatus[ul]->GetOrientationX(), Angle2XAxis, Angle2YAxis, Angle2ZAxis);
		vV2 = Rotate3Axis(cDOStatus[ul]->GetOrientationZ(), Angle2XAxis, Angle2YAxis, Angle2ZAxis);
		cDOStatus[ul]->SetOrientation(vV1, vV2);
		cDOStatus[ul]->SetVelocity(Rotate3Axis(cDOStatus[ul]->GetVelocity(), Angle2XAxis, Angle2YAxis, Angle2ZAxis));
		cDOStatus[ul]->SetAngularVelocity(Rotate3Axis(cDOStatus[ul]->GetAngularVelocity(), Angle2XAxis, Angle2YAxis, Angle2ZAxis));
	}
}

void DOWorld::Rotate(const njr::Vector3d& eX, const njr::Vector3d& eZ, const std::string& DOName)
{
	njr::Vector3d eY(eZ * eX);
	njr::Vector3d position, orientationX, orientationZ;
	for (vedo::vedo_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		if (cDOStatus[ul]->GetDOName() == DOName)
		{
			position = cDOStatus[ul]->GetPosition();
			cDOStatus[ul]->SetPosition(njr::Vector3d(position % eX, position % eY, position % eZ));
			orientationX = cDOStatus[ul]->GetOrientationX();
			orientationZ = cDOStatus[ul]->GetOrientationZ();
			cDOStatus[ul]->SetOrientation
				(njr::Vector3d(orientationX % eX, orientationX % eY, orientationX % eZ),
				 njr::Vector3d(orientationZ % eX, orientationZ % eY, orientationZ % eZ) );
		}
	}
}

bool DOWorld::Check() const
{
	std::string doname;
	std::vector<DOStatus *>::const_iterator idos;

	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		doname=(*idos)->GetDOName();
		if (DOWorld::GetDOModel(doname) == 0)
		{
			std::cout <<"unknown model of Discrete Object->" << doname.c_str() << std::endl;
			return false;
		}
	}

	if (pSystemParameter->GetDONumber()!=cDOStatus.size())
	{
		pSystemParameter->SetDONumber(cDOStatus.size());
	}

	return true;
}

vedo_float_t DOWorld::Volume(vedo_float_t& dMeshLength) const
{
	const Boundary          pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*         pdos    = 0;
	const DOModel*          pdoml   = 0;
	      vedo::vedo_uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<vedo::vedo_uint_t> TargetedSerialNumber;
	for (vedo::vedo_uint_t ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
			}
		}
	}

	vedo_float_t      dMeshVolume = pow(dMeshLength, 3.0);
	vedo_float_t      dVolume     = 0.0;
	vedo_float_t      dRadius;
	vedo::vedo_uint_t ulCounter;
	bool              bVolumeFilled;
	njr::Vector3d     vMeshCenter;
	for (vedo_float_t dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength; dX<pZOI.GetUpperPoint().x(); dX+=dMeshLength)
	{
		for (vedo_float_t dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength; dY<pZOI.GetUpperPoint().y(); dY+=dMeshLength)
		{
			for (vedo_float_t dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength; dZ<pZOI.GetUpperPoint().z(); dZ+=dMeshLength)
			{
				vMeshCenter.Set(dX, dY, dZ);
				ulCounter     = 0;
				bVolumeFilled = false;
				while ((ulCounter<TargetedSerialNumber.size()) && (!bVolumeFilled))
				{
					pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
					pdoml   = GetDOModel(pdos->GetDOName());
					dRadius = pdoml->GetShapeAttributes().sphere.radius;
					if ((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
					{
						dVolume       += dMeshVolume;
						bVolumeFilled  = true;
					}
					ulCounter++;
				}
			}
		}
	}

	return dVolume;
}

vedo_float_t DOWorld::ProjectedAreaX(vedo_float_t& dMeshLength) const
{
	const Boundary          pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*         pdos    = 0;
	const DOModel*          pdoml   = 0;
	      vedo::vedo_uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<vedo::vedo_uint_t> TargetedSerialNumber;
	for (vedo::vedo_uint_t ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition(njr::Vector3d(0.0, pdos->GetPosition().y(), pdos->GetPosition().z()));
			}
		}
	}

	vedo_float_t      dMeshArea = dMeshLength * dMeshLength;
	vedo_float_t      dArea     = 0.0;
	vedo_float_t      dRadius;
	vedo::vedo_uint_t ulCounter;
	bool              bAreaFilled;
	njr::Vector3d     vMeshCenter;
	for (vedo_float_t dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength; dY<pZOI.GetUpperPoint().y(); dY+=dMeshLength)
	{
		for (vedo_float_t dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength; dZ<pZOI.GetUpperPoint().z(); dZ+=dMeshLength)
		{
			vMeshCenter.Set(0.0, dY, dZ);
			ulCounter   = 0;
			bAreaFilled = false;
			while ((ulCounter<TargetedSerialNumber.size()) && (!bAreaFilled))
			{
				pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
				pdoml   = GetDOModel(pdos->GetDOName());
				dRadius = pdoml->GetShapeAttributes().sphere.radius;
				if ((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
				{
					dArea       += dMeshArea;
					bAreaFilled  = true;
				}
				ulCounter++;
			}
		}
	}

	return dArea;
}

vedo_float_t DOWorld::ProjectedAreaY(vedo_float_t& dMeshLength) const
{
	const Boundary          pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*         pdos    = 0;
	const DOModel*          pdoml   = 0;
	      vedo::vedo_uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<vedo::vedo_uint_t> TargetedSerialNumber;
	for (vedo::vedo_uint_t ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition(njr::Vector3d(pdos->GetPosition().x(), 0.0, pdos->GetPosition().z()));
			}
		}
	}

	vedo_float_t        dMeshArea = dMeshLength * dMeshLength;
	vedo_float_t        dArea     = 0.0;
	vedo_float_t        dRadius;
	vedo::vedo_uint_t ulCounter;
	bool          bAreaFilled;
	njr::Vector3d   vMeshCenter;
	for(vedo_float_t dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength; dX<pZOI.GetUpperPoint().x(); dX+=dMeshLength)
	{
		for(vedo_float_t dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength; dZ<pZOI.GetUpperPoint().z(); dZ+=dMeshLength)
		{
			vMeshCenter.Set(dX, 0.0, dZ);
			ulCounter   = 0;
			bAreaFilled = false;
			while((ulCounter<TargetedSerialNumber.size()) && (!bAreaFilled))
			{
				pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
				pdoml   = GetDOModel(pdos->GetDOName());
				dRadius = pdoml->GetShapeAttributes().sphere.radius;
				if ((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
				{
					dArea       += dMeshArea;
					bAreaFilled  = true;
				}
				ulCounter++;
			}
		}
	}

	return dArea;
}

vedo_float_t DOWorld::ProjectedAreaZ(vedo_float_t& dMeshLength) const
{
	const Boundary          pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*         pdos    = 0;
	const DOModel*          pdoml   = 0;
	      vedo::vedo_uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<vedo::vedo_uint_t> TargetedSerialNumber;
	for (vedo::vedo_uint_t ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition(njr::Vector3d(pdos->GetPosition().x(), pdos->GetPosition().y(), 0.0));
			}
		}
	}

	vedo_float_t      dMeshArea = dMeshLength * dMeshLength;
	vedo_float_t      dArea     = 0.0;
	vedo_float_t      dRadius;
	vedo::vedo_uint_t ulCounter;
	bool              bAreaFilled;
	njr::Vector3d     vMeshCenter;
	for (vedo_float_t dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength; dX<pZOI.GetUpperPoint().x(); dX+=dMeshLength)
	{
		for (vedo_float_t dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength; dY<pZOI.GetUpperPoint().y(); dY+=dMeshLength)
		{
			vMeshCenter.Set(dX, dY, 0.0);
			ulCounter   = 0;
			bAreaFilled = false;
			while ((ulCounter<TargetedSerialNumber.size()) && (!bAreaFilled))
			{
				pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
				pdoml   = GetDOModel(pdos->GetDOName());
				dRadius = pdoml->GetShapeAttributes().sphere.radius;
				if ((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
				{
					dArea       += dMeshArea;
					bAreaFilled  = true;
				}
				ulCounter++;
			}
		}
	}

	return dArea;
}

const DOStatus DOWorld::GetDOStatus(const std::string& DOName, const vedo::vedo_uint_t ulSerialNumber) const
{
	if (ulSerialNumber < cDOStatus.size())
	{
		if (cDOStatus[ulSerialNumber]->GetDOName() == DOName)
		{
			return *(cDOStatus[ulSerialNumber]);
		}
		else
		{
			std::cout << "DOWorld::Trace Error (I) !! Can't find DOStatus [" << ulSerialNumber << "]" << std::endl;
			return *(cDOStatus[0]);
		}
	}
	else
	{
		std::cout << "DOWorld::Trace Error (II) !! Can't find DOStatus [" << ulSerialNumber << "]" << std::endl;
		return *(cDOStatus[0]);
	}
}

}   // namespace vedo
