#include <aries/utility/Constants.h>
#include <vedo/framework/Boundary.h>
#include <vedo/framework/DOMap.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/framework/DOModel.h>
#include <njr/utility.h>
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

std::string DOWorld::GetDOName(const _uint_t& odo) const
{
	if (odo < pSystemParameter->GetDONumber())
	{
		return cDOStatus[odo]->GetDOName();
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOWorld::GetDOName(const _uint_t&) const" << std::endl
			<< "        Note: Element's ID wrong"                                 << std::endl;
		exit(-1);
	}
}

const DOStatus* DOWorld::GetDOStatus(const _uint_t& odo) const
{
	return(odo < pSystemParameter->GetDONumber()) ? cDOStatus[odo] : 0;
}

void DOWorld::SetDOStatus(const _uint_t& odo, const DOStatus& dos)
{
	if (odo < pSystemParameter->GetDONumber())
	{
		*(cDOStatus[odo]) = dos;
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOWorld::SetDOStatus(const _uint_t&, const DOStatus&)" << std::endl
			<< "        Note: Element's ID wrong"                                              << std::endl;
		exit(-1);
	}
}

void DOWorld::SetDOStatusVelocityAndAngularVelocity
	(const _uint_t& odo, const aries::Vector3df& vVelocity, const aries::Vector3df& vAngularVelocity)
{
	if (odo < pSystemParameter->GetDONumber())
	{
		cDOStatus[odo]->SetVelocity(vVelocity);
		cDOStatus[odo]->SetAngularVelocity(vAngularVelocity);
	}
	else
	{
		std::cerr
			<< "Error!! Code: DOWorld::SetDOStatusVelocityAndAngularVelocity(const _uint_t&, const DOStatus&)" << std::endl
			<< "        Note: Element's ID wrong" << std::endl;
		exit(-1);
	}
}

/*
void DOWorld::EraseDOStatus(const std::vector<_uint_t>& EraseList, const _uint_t& rank)
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

const std::vector<_uint_t> DOWorld::FindDO (std::string name) const
{
	std::vector<_uint_t> List;
	std::vector<DOStatus*>::iterator cDOSp;
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
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
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]->SetMonitored(m);
	}
}

//void DOWorld::ClearSeparatedElements(const IactRecordTab* pIactRecordTab)
void DOWorld::ClearSeparatedElements()
{
	std::vector<_uint_t> RedundantElement;
	_uint_t numberDO = pSystemParameter->GetDONumber();
	const DOStatus* pdos  = 0;

	_uint_t ul;
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

void DOWorld::EraseDOStatus(const _uint_t& ulID)
{
	cDOStatus.erase(cDOStatus.begin()+ulID);
	pSystemParameter->SetDONumber(cDOStatus.size());
}

void DOWorld::EraseDOStatus(const std::vector<_uint_t>& EraseList)
{
	if(!EraseList.empty())
	{
		const _uint_t numberDO      = cDOStatus.size();
		const _uint_t ErasenumberDO = EraseList.size();
		_uint_t ulEraseCounter = 0;
		for (_uint_t ul=EraseList[0]; ul<numberDO; ul++)
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
	(const std::vector<_uint_t>& EraseList, const _uint_t& rank)
{
	const _uint_t numberDO      = cDOStatus.size();
	const _uint_t ErasenumberDO = EraseList.size();
	const _uint_t NewnumberDO   = numberDO - ErasenumberDO;
	std::vector<DOStatus*> NewcDOStatus(NewnumberDO);

	_uint_t ul;
	_uint_t ulEraseCounter = 0;
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

void DOWorld::SetFieldAcceleration(const aries::Vector3df& ff)
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
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
//		cDOStatus[ul]->SetOrientation(aries::Vector3df(AXIALX), aries::Vector3df(AXIALZ));
		cDOStatus[ul]->SetVelocity(aries::Vector3df());
		cDOStatus[ul]->SetAngularVelocity(aries::Vector3df());
	}
}

void DOWorld::FreezeElements(std::string& sDOName)
{
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		if (cDOStatus[ul]->GetDOName() == sDOName)
		{
//			cDOStatus[ul]->SetOrientation(njr::AXIALX, njr::AXIALZ));
			cDOStatus[ul]->SetVelocity(aries::Vector3df());
			cDOStatus[ul]->SetAngularVelocity(aries::Vector3df());
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
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		dos = cDOStatus[ul];
		str = dos->GetDOName();
		hasInsert = false;
		for (_uint_t ul=0; ul<cDOStatusTempName.size(); ul++)
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
	for (_uint_t ul=0; ul<cDOStatusTempName.size(); ul++)
	{
		for (_uint_t vl=0; vl<(cDOStatusTempData[ul]).size(); vl++)
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
	_float_t    dEnergySystem;
	_float_t    dEnergyPotential;
	_float_t    dEnergyKinetic;
	_float_t    dEnergyTranslation;
	_float_t    dEnergyRotation;
	aries::Vector3df   vMomentumAvg;
	aries::Vector3df   vAngularMomentumAvg;
	_float_t    dMomentumNorm        = 0.0;
	_float_t    dAngularMomentumNorm = 0.0;
	_float_t    dVelocityMax;
	_float_t    dVelocityMin;
	_float_t    dAngularVelocityMax;
	_float_t    dAngularVelocityMin;
	_float_t    m;
	aries::Vector3df   mmi;
	_float_t    v;
	aries::Vector3df   av;
	_float_t    dav;
	bool            StartPoint           = true;
	bool            NoMobileElements     = false;
	_uint_t     counter              = 0;
	_uint_t     NumMobileDO          = 0;
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
			vAngularMomentumAvg = mmi.cross(av);
			dEnergyPotential    = -m * (pSystemParameter->GetFieldAcceleration().dot(dos->GetPosition()));
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
		for (_uint_t i=counter+1; i<(pSystemParameter->GetDONumber()); i++)
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
				vAngularMomentumAvg += mmi.cross(av);
				dEnergyPotential    -= m * (pSystemParameter->GetFieldAcceleration().dot(dos->GetPosition()));
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
		for (_uint_t i=counter; i<(pSystemParameter->GetDONumber()); i++)
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
				dAngularMomentumNorm += (mmi.cross(av) - vAngularMomentumAvg).length();
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
const std::pair<aries::Vector3df, aries::Vector3df>
	DOWorld::Distribution(_float_t& dMeshLength) const
{
	const Boundary      pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*     pdos    = 0;
	const DOModel*      pdoml   = 0;
	      _uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<_uint_t> TargetedSerialNumber;
	for(_uint_t ul=0; ul<ulNumDO; ul++)
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

	_float_t        dXMin = pZOI.GetUpperPoint().x();
	_float_t        dYMin = pZOI.GetUpperPoint().y();
	_float_t        dZMin = pZOI.GetUpperPoint().z();
	_float_t        dXMax = pZOI.GetLowerPoint().x();
	_float_t        dYMax = pZOI.GetLowerPoint().y();
	_float_t        dZMax = pZOI.GetLowerPoint().z();
	_float_t        dRadius;
	_uint_t ulCounter;
	bool          bMeshFilled;
	aries::Vector3df   vMeshCenter;
	for(_float_t dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength;
		dX<pZOI.GetUpperPoint().x();
		dX+=dMeshLength                                   )
	{
		for(_float_t dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength;
			dY<pZOI.GetUpperPoint().y();
			dY+=dMeshLength                                   )
		{
			for(_float_t dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength;
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
			(aries::Vector3df(dXMin, dYMin, dZMin),
			 aries::Vector3df(dXMax, dYMax, dZMax) );
}
*/

void DOWorld::Shift(const aries::Vector3df& shift)
{
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]->SetPosition(cDOStatus[ul]->GetPosition() + shift);
	}
}

void DOWorld::Shift(const aries::Vector3df& shift, const std::string& DOName)
{
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		if (cDOStatus[ul]->GetDOName() == DOName)
		{
			cDOStatus[ul]->SetPosition(cDOStatus[ul]->GetPosition() + shift);
		}
	}
}

std::pair<_float_t, _float_t> Rotate1Axis(_float_t Axis1, _float_t Axis2, _float_t dAnglePlus)
{
	if (dAnglePlus == 0.0)
	{
		return std::make_pair(Axis1, Axis2);
	}
	else
	{
		_float_t dRadius = sqrt(Axis1 * Axis1 + Axis2 * Axis2);
		_float_t dAngle;
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
			dAngle += aries::math::_PI;
		}
		else if ((Axis1 < 0.0) && (Axis2 < 0.0))
		{
			dAngle += aries::math::_PI;
		}

		dAngle += dAnglePlus;

		if (dAngle >= aries::math::_DoublePI)
		{
			dAngle -= aries::math::_DoublePI;
		}

		return std::make_pair(dRadius*cos(dAngle), dRadius*sin(dAngle));
	}
}

aries::Vector3df Rotate3Axis
	(aries::Vector3df vTarget, const _float_t& Angle2XAxis, const _float_t& Angle2YAxis, const _float_t& Angle2ZAxis)
{
	std::pair<_float_t, _float_t> pNewAxisValues;

	pNewAxisValues = Rotate1Axis(vTarget.y(), vTarget.z(), Angle2XAxis);
	vTarget.set(vTarget.x(), pNewAxisValues.first, pNewAxisValues.second);

	pNewAxisValues = Rotate1Axis(vTarget.z(), vTarget.x(), Angle2YAxis);
	vTarget.set(pNewAxisValues.second, vTarget.y(), pNewAxisValues.first);

	pNewAxisValues = Rotate1Axis(vTarget.x(), vTarget.y(), Angle2ZAxis);
	vTarget.set(pNewAxisValues.first, pNewAxisValues.second, vTarget.z());

	return vTarget;
}

void DOWorld::Rotate(const _float_t& Angle2XAxis, const _float_t& Angle2YAxis, const _float_t& Angle2ZAxis)
{
	aries::Vector3df vV1, vV2;
	std::pair<_float_t, _float_t> pNewPosition, pNewVelocity, pNewAngularVelocity;
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]->SetPosition(Rotate3Axis(cDOStatus[ul]->GetPosition(), Angle2XAxis, Angle2YAxis, Angle2ZAxis));
		vV1 = Rotate3Axis(cDOStatus[ul]->GetOrientationX(), Angle2XAxis, Angle2YAxis, Angle2ZAxis);
		vV2 = Rotate3Axis(cDOStatus[ul]->GetOrientationZ(), Angle2XAxis, Angle2YAxis, Angle2ZAxis);
		cDOStatus[ul]->SetOrientation(vV1, vV2);
		cDOStatus[ul]->SetVelocity(Rotate3Axis(cDOStatus[ul]->GetVelocity(), Angle2XAxis, Angle2YAxis, Angle2ZAxis));
		cDOStatus[ul]->SetAngularVelocity(Rotate3Axis(cDOStatus[ul]->GetAngularVelocity(), Angle2XAxis, Angle2YAxis, Angle2ZAxis));
	}
}

void DOWorld::Rotate(const aries::Vector3df& eX, const aries::Vector3df& eZ, const std::string& DOName)
{
	aries::Vector3df eY(eZ.cross(eX));
	aries::Vector3df position, orientationX, orientationZ;
	for (_uint_t ul=0; ul<cDOStatus.size(); ul++)
	{
		if (cDOStatus[ul]->GetDOName() == DOName)
		{
			position = cDOStatus[ul]->GetPosition();
			cDOStatus[ul]->SetPosition(aries::Vector3df(position.dot(eX), position.dot(eY), position.dot(eZ)));
			orientationX = cDOStatus[ul]->GetOrientationX();
			orientationZ = cDOStatus[ul]->GetOrientationZ();
			cDOStatus[ul]->SetOrientation
				(aries::Vector3df(orientationX.dot(eX), orientationX.dot(eY), orientationX.dot(eZ)),
				 aries::Vector3df(orientationZ.dot(eX), orientationZ.dot(eY), orientationZ.dot(eZ)) );
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

_float_t DOWorld::Volume(_float_t& dMeshLength) const
{
	const Boundary          pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*         pdos    = 0;
	const DOModel*          pdoml   = 0;
	      _uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<_uint_t> TargetedSerialNumber;
	for (_uint_t ul=0; ul<ulNumDO; ul++)
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

	_float_t      dMeshVolume = pow(dMeshLength, 3.0);
	_float_t      dVolume     = 0.0;
	_float_t      dRadius;
	_uint_t ulCounter;
	bool              bVolumeFilled;
	aries::Vector3df     vMeshCenter;
	for (_float_t dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength; dX<pZOI.GetUpperPoint().x(); dX+=dMeshLength)
	{
		for (_float_t dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength; dY<pZOI.GetUpperPoint().y(); dY+=dMeshLength)
		{
			for (_float_t dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength; dZ<pZOI.GetUpperPoint().z(); dZ+=dMeshLength)
			{
				vMeshCenter.set(dX, dY, dZ);
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

_float_t DOWorld::ProjectedAreaX(_float_t& dMeshLength) const
{
	const Boundary          pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*         pdos    = 0;
	const DOModel*          pdoml   = 0;
	      _uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<_uint_t> TargetedSerialNumber;
	for (_uint_t ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition(aries::Vector3df(0.0, pdos->GetPosition().y(), pdos->GetPosition().z()));
			}
		}
	}

	_float_t      dMeshArea = dMeshLength * dMeshLength;
	_float_t      dArea     = 0.0;
	_float_t      dRadius;
	_uint_t ulCounter;
	bool              bAreaFilled;
	aries::Vector3df     vMeshCenter;
	for (_float_t dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength; dY<pZOI.GetUpperPoint().y(); dY+=dMeshLength)
	{
		for (_float_t dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength; dZ<pZOI.GetUpperPoint().z(); dZ+=dMeshLength)
		{
			vMeshCenter.set(0.0, dY, dZ);
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

_float_t DOWorld::ProjectedAreaY(_float_t& dMeshLength) const
{
	const Boundary          pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*         pdos    = 0;
	const DOModel*          pdoml   = 0;
	      _uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<_uint_t> TargetedSerialNumber;
	for (_uint_t ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition(aries::Vector3df(pdos->GetPosition().x(), 0.0, pdos->GetPosition().z()));
			}
		}
	}

	_float_t        dMeshArea = dMeshLength * dMeshLength;
	_float_t        dArea     = 0.0;
	_float_t        dRadius;
	_uint_t ulCounter;
	bool          bAreaFilled;
	aries::Vector3df   vMeshCenter;
	for(_float_t dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength; dX<pZOI.GetUpperPoint().x(); dX+=dMeshLength)
	{
		for(_float_t dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength; dZ<pZOI.GetUpperPoint().z(); dZ+=dMeshLength)
		{
			vMeshCenter.set(dX, 0.0, dZ);
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

_float_t DOWorld::ProjectedAreaZ(_float_t& dMeshLength) const
{
	const Boundary          pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*         pdos    = 0;
	const DOModel*          pdoml   = 0;
	      _uint_t ulNumDO = pSystemParameter->GetDONumber();

	std::vector<_uint_t> TargetedSerialNumber;
	for (_uint_t ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition(aries::Vector3df(pdos->GetPosition().x(), pdos->GetPosition().y(), 0.0));
			}
		}
	}

	_float_t      dMeshArea = dMeshLength * dMeshLength;
	_float_t      dArea     = 0.0;
	_float_t      dRadius;
	_uint_t ulCounter;
	bool              bAreaFilled;
	aries::Vector3df     vMeshCenter;
	for (_float_t dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength; dX<pZOI.GetUpperPoint().x(); dX+=dMeshLength)
	{
		for (_float_t dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength; dY<pZOI.GetUpperPoint().y(); dY+=dMeshLength)
		{
			vMeshCenter.set(dX, dY, 0.0);
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

const DOStatus DOWorld::GetDOStatus(const std::string& DOName, const _uint_t ulSerialNumber) const
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
