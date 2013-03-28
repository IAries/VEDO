#include <FrameWork/Interfaces/Constants.h>
#include <FrameWork/Interfaces/Boundary.h>
#include <FrameWork/Interfaces/DOMap.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <NJR/Interfaces/Utility.h>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>

namespace VEDO
{

DOWorld::DOWorld()
	: cDOStatus(0), cDOModel(0), cIactModel(0), pSystemParameter(0)
{
};

DOWorld::DOWorld(const DOWorld &dow)
	: cDOStatus(0), cDOModel(0), cIactModel(0), pSystemParameter(0)
{
	*this = dow;
};

DOWorld::DOWorld
	(SystemParameter *sp,
	const std::list<DOModel*>& nDOModel,
	const std::list<IactModel*>& nIactModel,
	const std::vector<DOStatus*>& nDOStatus)
{
	pSystemParameter = sp;
	cDOModel         = nDOModel;
	cIactModel       = nIactModel;
	cDOStatus        = nDOStatus;
	DOWorld::Check();
};

const DOWorld& DOWorld::operator = (const DOWorld& dow)
{
	DOWorld::Clear();

	pSystemParameter = new SystemParameter( *dow.pSystemParameter );

	transform
		(dow.cDOModel.begin(),
		 dow.cDOModel.end(),
		 back_inserter(cDOModel),
		 NJR::Copy_obj()         );

	transform
		(dow.cIactModel.begin(),
		 dow.cIactModel.end(),
		 back_inserter(cIactModel),
		 NJR::Copy_obj()           );

	transform
		(dow.cDOStatus.begin(),
		 dow.cDOStatus.end(),
		 back_inserter(cDOStatus),
		 NJR::Copy_obj()          );

	DOWorld::Check();

	return *this;
};

DOWorld::~DOWorld()
{
	DOWorld::Clear();
};

const DOStatus* DOWorld::GetDOStatus(const unsigned long& odo) const
{
	return(odo < pSystemParameter->GetDONumber()) ? cDOStatus[odo] : 0;
};

void DOWorld::SetDOStatus(const unsigned long& odo, const DOStatus& dos)
{
	if(odo < pSystemParameter->GetDONumber())
	{
		*(cDOStatus[odo]) = dos;
	}
};

/*
void DOWorld::EraseDOStatus
	(const std::vector<unsigned long>& EraseList, const int& rank)
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
};
*/

const std::vector<unsigned long> DOWorld::FindDO (std::string name) const
{
	std::vector<unsigned long> List;
	std::vector<DOStatus*>::iterator cDOSp;
	for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
		if(cDOStatus[ul]->GetDOName() == name)
		{
			List.push_back(ul);
		}
	}
	return List;
};

void DOWorld::TurnMonitor(bool m)
{
	for(unsigned ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]->SetMonitored(m);
	};
};

//void DOWorld::ClearSeparatedElements(const IactRecordTab* pIactRecordTab)
void DOWorld::ClearSeparatedElements()
{
	std::vector<unsigned long> RedundantElement;
	unsigned long numberDO = pSystemParameter->GetDONumber();
	const DOStatus* pdos  = 0;
	const DOModel*  pdoml = 0;

	unsigned long ul;
	std::list<DOModel*>::const_iterator idoml;
	std::list<IactModel*>::const_iterator cimp;
	for(ul=0; ul<numberDO; ul++)
	{
		cDOStatus[ul]->SetMonitored(true);
	}
	for(cimp = cIactModel.begin(); cimp != cIactModel.end(); cimp++)
	{
	}

	for(ul=0; ul<numberDO; ul++)
	{
		if(!(pdos->ISMonitored()))
		{
			RedundantElement.push_back(ul);
		}
	}
	if (!RedundantElement.empty())
	{
		std::cout
			<< "Number of redundant elements = "
			<< RedundantElement.size()
			<< std::endl;
//		cDO.Erase(RedundantElement);
//		pDOWorld->EraseDOStatus(RedundantElement);
//		vcIactMaster.clear();
//		vcIactSlave.clear();
//		IactPairTab.clear();
//		cIact.Clear();
	}
};

void DOWorld::EraseDOStatus(const std::vector<unsigned long>& EraseList)
{
	if(!EraseList.empty())
	{
		const unsigned long numberDO      = cDOStatus.size();
		const unsigned long ErasenumberDO = EraseList.size();
		unsigned long ul;
		unsigned long ulEraseCounter = 0;
		for(ul=EraseList[0]; ul<numberDO; ul++)
		{
			if(ulEraseCounter < ErasenumberDO)
			{
				if(ul < EraseList[ulEraseCounter])
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
};

/*
void DOWorld::EraseDOStatus
	(const std::vector<unsigned long>& EraseList, const int& rank)
{
	const unsigned long numberDO      = cDOStatus.size();
	const unsigned long ErasenumberDO = EraseList.size();
	const unsigned long NewnumberDO   = numberDO - ErasenumberDO;
	std::vector<DOStatus*> NewcDOStatus(NewnumberDO);

	unsigned long ul;
	unsigned long ulEraseCounter = 0;
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
};
*/

const DOModel* DOWorld::GetDOModel(const std::string& DOName) const
{
	std::list<DOModel*>::const_iterator idoml;
	idoml = find_if(cDOModel.begin(), cDOModel.end(), DOModel(DOName));
	return(idoml != cDOModel.end()) ? *idoml : 0;
};

const IactModel* DOWorld::GetIactModel
	(const std::string& MG, const std::string& SG) const
{
	std::list<IactModel*>::const_iterator iiactml;
	iiactml = find_if(cIactModel.begin(), cIactModel.end(), IactModel(MG, SG));
	return(iiactml != cIactModel.end()) ? *iiactml : 0;
};

bool DOWorld::UpdateDOStatus(const std::vector<const DOStatus *>& nDOStatus)
{
	if ( cDOStatus.size() != nDOStatus.size() )
	{
		std::cout
			<< "Warning!!! DO("
			<< (unsigned int) cDOStatus.size()
			<< ")don't equal to->"
			<< (unsigned int) nDOStatus.size()
			<< std::endl;
	}

	std::vector<DOStatus *> vnDOS;
	transform
		(nDOStatus.begin(),
		 nDOStatus.end(),
		 back_inserter(vnDOS),
		 NJR::Copy_obj()      );
	for_each
		(cDOStatus.begin(),
		 cDOStatus.end(),
		 NJR::Delete_ptr() );

	cDOStatus = vnDOS;

	SystemParameter* sp = pSystemParameter;
	pSystemParameter
		= new SystemParameter
			(sp->GetTitle(),
			sp->GetNote(),
			sp->GetTimeStart(),
			sp->GetTimeStop(),
			sp->GetTimeInterval(),
			sp->GetTimeCurrent(),
			(unsigned int) cDOStatus.size(),
			sp->GetFieldAcceleration(),
			sp->GetZoneOfInterest(),
			sp->GetPeriodicBoundaryConditions());

	delete sp;
	return DOWorld::Check();
};

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

	for(idos = cDOStatus.begin(); idos != cDOStatus.end(); ++idos)
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

	SystemParameter* sp = pSystemParameter;
	pSystemParameter
		= new SystemParameter
			(sp->GetTitle(),
			sp->GetNote(),
			sp->GetTimeStart(),
			sp->GetTimeStop(),
			sp->GetTimeInterval(),
			sp->GetTimeCurrent(),
			(unsigned int) cDOStatus.size(),
			sp->GetFieldAcceleration(),
			sp->GetZoneOfInterest(),
			sp->GetPeriodicBoundaryConditions());

	delete sp;

	return DOWorld::Check();
};

void DOWorld::SetFieldAcceleration(const NJR::Vector3d& ff)
{
	pSystemParameter->SetFieldAcceleration(ff);
};

void DOWorld::SetModels(const DOWorld* dow)
{
	// Set pSystemParameter
	pSystemParameter = dow->GetSystemParameter();
	pSystemParameter->SetDONumber(cDOStatus.size());

	// Set cDOModel
	std::list<DOModel*>::iterator cdmp;
	for(cdmp=(cDOModel.begin()); cdmp!=(cDOModel.end()); cdmp++)
	{
		if((dow->GetDOModel((*cdmp)->GetDOName())))
		{
			*cdmp = new DOModel(*(dow->GetDOModel((*cdmp)->GetDOName())));
		}
	}

	// Set cIactModel
	std::list<IactModel*>::iterator cimp;
	for(cimp=(cIactModel.begin()); cimp!=(cIactModel.end()); cimp++)
	{
		if(dow
			->GetIactModel
				((*cimp)->GetMasterDOGroup(),
				 (*cimp)->GetSlaveDOGroup()  ) )
		{
			*cimp
				= new IactModel
					(*(dow
						->GetIactModel
							((*cimp)->GetMasterDOGroup(),
							 (*cimp)->GetSlaveDOGroup()) ) );
		}
	}
}

void DOWorld::FreezeAllElements()
{
	for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
//		cDOStatus[ul]->SetOrientation(NJR::Vector3d(NJRDXF::AXIALX), NJR::Vector3d(NJRDXF::AXIALZ));
		cDOStatus[ul]->SetVelocity(NJR::Vector3d(NJRDXF::ZERO));
		cDOStatus[ul]->SetAngularVelocity(NJR::Vector3d(NJRDXF::ZERO));
	}
};

void DOWorld::FreezeElements(std::string& sDOName)
{
	for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
		if (cDOStatus[ul]->GetDOName() == sDOName)
		{
//			cDOStatus[ul]->SetOrientation(NJR::Vector3d(NJRDXF::AXIALX), NJR::Vector3d(NJRDXF::AXIALZ));
			cDOStatus[ul]->SetVelocity(NJR::Vector3d(NJRDXF::ZERO));
			cDOStatus[ul]->SetAngularVelocity(NJR::Vector3d(NJRDXF::ZERO));
		}
	}
};

void DOWorld::SortingDOStatus()
{
	std::vector<DOStatus*> cDOStatusNew;
	std::string str;
	std::vector<std::string>              cDOStatusTempName;
	std::vector< std::vector<DOStatus*> > cDOStatusTempData;
	bool hasInsert;
	DOStatus* dos;
	for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
		dos = cDOStatus[ul];
		str = dos->GetDOName();
		hasInsert = false;
		for(unsigned long ul=0; ul<cDOStatusTempName.size(); ul++)
		{
			if(cDOStatusTempName[ul] == str)
			{
				cDOStatusTempData[ul].push_back(new DOStatus(*dos));
				hasInsert = true;
			}
		}
		if(!hasInsert)
		{
			cDOStatusTempName.push_back(str);
			std::vector<DOStatus*> vdos;
			vdos.push_back(new DOStatus(*dos));
			cDOStatusTempData.push_back(vdos);
		}
	}
	for(unsigned long ul=0; ul<cDOStatusTempName.size(); ul++)
	{
		for(unsigned long vl=0; vl<(cDOStatusTempData[ul]).size(); vl++)
		{
			cDOStatusNew.push_back(new DOStatus(*(cDOStatusTempData[ul][vl])));
		}

	}
	cDOStatus = cDOStatusNew;
};

void DOWorld::Clear()
{
	if (pSystemParameter != 0)
	{
		delete pSystemParameter;
		pSystemParameter = 0;
	}

	for_each(cDOModel.begin()  , cDOModel.end()  , NJR::Delete_ptr());
	for_each(cIactModel.begin(), cIactModel.end(), NJR::Delete_ptr());
	for_each(cDOStatus.begin() , cDOStatus.end() , NJR::Delete_ptr());

	cDOModel.clear();
	cIactModel.clear();
	cDOStatus.clear();
};

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
};

void DOWorld::CalculateSystemEnergy()
{
	double          dEnergySystem;
	double          dEnergyPotential;
	double          dEnergyKinetic;
	double          dEnergyTranslation;
	double          dEnergyRotation;
	NJR::Vector3d     vMomentumAvg;
	NJR::Vector3d     vAngularMomentumAvg;
	double          dMomentumNorm        = 0.0;
	double          dAngularMomentumNorm = 0.0;
	double          dVelocityMax;
	double          dVelocityMin;
	double          dAngularVelocityMax;
	double          dAngularVelocityMin;
	double          m;
	NJR::Vector3d     mmi;
	double          v;
	NJR::Vector3d     av;
	double          dav;
	bool            StartPoint       = true;
	bool            NoMobileElements = false;
	unsigned        counter          = 0;
	unsigned        NumMobileDO      = 0;
	const DOModel*  dom              = 0;
	const DOStatus* dos              = 0;

	// Find "Start Point"
	while(StartPoint)
	{
		dos = GetDOStatus(counter);
		dom = GetDOModel(dos->GetDOName());
		if (dom->GetBehavior() == "mobile")
		{
			m   = dom->GetMass();
			mmi = dom->GetMassMomentInertia();
			v   = dos->GetVelocity().length();
			av  = dos->GetAngularVelocity();

			vMomentumAvg        = m * dos->GetVelocity();
			vAngularMomentumAvg = mmi * av;

			dEnergyPotential
				= -m * (pSystemParameter->GetFieldAcceleration() % dos->GetPosition());
			dEnergyTranslation
				= 0.5 * m * v * v;
			dEnergyRotation
				= 0.5
				* (mmi.x() * av.x() * av.x()
				+  mmi.y() * av.y() * av.y()
				+  mmi.z() * av.z() * av.z());

			dEnergyKinetic        = dEnergyTranslation + dEnergyRotation;
			dEnergySystem         = dEnergyPotential + dEnergyKinetic;
			dVelocityMax          = v;
			dVelocityMin          = v;
			dAngularVelocityMax   = av.length();
			dAngularVelocityMin   = dAngularVelocityMax;
			NumMobileDO           = 1;
			StartPoint = false;
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
		for(unsigned i=counter+1; i<(pSystemParameter->GetDONumber()); i++)
		{
			dos = GetDOStatus(i);
			dom = GetDOModel(dos->GetDOName());
			if (dom->GetBehavior() == "mobile")
			{
				NumMobileDO++;
				m   = dom->GetMass();
				mmi = dom->GetMassMomentInertia();
				v   = dos->GetVelocity().length();
				av  = dos->GetAngularVelocity();
				dav = av.length();

				vMomentumAvg        += m * dos->GetVelocity();
				vAngularMomentumAvg += mmi * av;

				dEnergyPotential
					-= m
					* (pSystemParameter->GetFieldAcceleration() % dos->GetPosition());
				dEnergyTranslation
					+= 0.5 * m * v * v;
				dEnergyRotation
					+= 0.5
					* (mmi.x() * av.x() * av.x()
					+  mmi.y() * av.y() * av.y()
					+  mmi.z() * av.z() * av.z());
				dEnergyKinetic = dEnergyTranslation + dEnergyRotation;
				dEnergySystem  = dEnergyPotential + dEnergyKinetic;

				if(v > dVelocityMax)
				{
					dVelocityMax = v;
				}
				else if(v < dVelocityMin)
				{
					dVelocityMin = v;
				}

				if(dav > dAngularVelocityMax)
				{
					dAngularVelocityMax = dav;
				}
				else if(dav < dAngularVelocityMin)
				{
					dAngularVelocityMin = dav;
				}
			}
		}
	}
	else
	{
		dEnergySystem         = 0.0;
		dEnergyKinetic        = 0.0;
		dEnergyPotential      = 0.0;
		dEnergyTranslation    = 0.0;
		dEnergyRotation       = 0.0;
		dVelocityMax          = 0.0;
		dVelocityMin          = 0.0;
		dAngularVelocityMax   = 0.0;
		dAngularVelocityMin   = 0.0;
	}

	vMomentumAvg        = 1.0 / NumMobileDO * vMomentumAvg;
	vAngularMomentumAvg = 1.0 / NumMobileDO * vAngularMomentumAvg;

	// Calculate the "Norm" of momentum and angular momentun
	if (!NoMobileElements)
	{
		for(unsigned i=counter; i<(pSystemParameter->GetDONumber()); i++)
		{
			dos = GetDOStatus(i);
			dom = GetDOModel(dos->GetDOName());
			if (dom->GetBehavior() == "mobile")
			{
				m   = dom->GetMass();
				mmi = dom->GetMassMomentInertia();
				v   = dos->GetVelocity().length();
				av  = dos->GetAngularVelocity();
				dMomentumNorm
					+= (m * dos->GetVelocity() - vMomentumAvg).length();
				dAngularMomentumNorm
					+= (mmi * av - vAngularMomentumAvg).length();
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
};

const std::pair<NJR::Vector3d, NJR::Vector3d>
	DOWorld::Distribution(double& dMeshLength) const
{
	const Boundary      pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*     pdos    = 0;
	const DOModel*      pdoml   = 0;
	      unsigned long ulNumDO = pSystemParameter->GetDONumber();

	std::vector<unsigned long> TargetedSerialNumber;
	for(unsigned long ul=0; ul<ulNumDO; ul++)
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
		};
	};

	double        dXMin = pZOI.GetUpperPoint().x();
	double        dYMin = pZOI.GetUpperPoint().y();
	double        dZMin = pZOI.GetUpperPoint().z();
	double        dXMax = pZOI.GetLowerPoint().x();
	double        dYMax = pZOI.GetLowerPoint().y();
	double        dZMax = pZOI.GetLowerPoint().z();
	double        dRadius;
	unsigned long ulCounter;
	bool          bMeshFilled;
	NJR::Vector3d   vMeshCenter;
	for(double dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength;
		dX<pZOI.GetUpperPoint().x();
		dX+=dMeshLength                                   )
	{
		for(double dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength;
			dY<pZOI.GetUpperPoint().y();
			dY+=dMeshLength                                   )
		{
			for(double dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength;
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
					};
					ulCounter++;
				};
			};
		};
	};

	if(dXMax < dXMin)
	{
		dXMin = dXMax;
	};

	if(dYMax < dYMin)
	{
		dYMin = dYMax;
	};

	if(dZMax < dZMin)
	{
		dZMin = dZMax;
	};

	return
		std::make_pair
			(NJR::Vector3d(dXMin, dYMin, dZMin),
			 NJR::Vector3d(dXMax, dYMax, dZMax) );
};

void DOWorld::Shift(const NJR::Vector3d& shift)
{
	for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]->SetPosition(cDOStatus[ul]->GetPosition() + shift);
	};
};

void DOWorld::Shift(const NJR::Vector3d& shift, const std::string& DOName)
{
	for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
		if(cDOStatus[ul]->GetDOName() == DOName)
		{
			cDOStatus[ul]->SetPosition(cDOStatus[ul]->GetPosition() + shift);
		};
	};
};

std::pair<double, double> Rotate1Axis
	(double Axis1, double Axis2, double dAnglePlus)
{
	if (dAnglePlus == 0.0)
	{
		return std::make_pair(Axis1, Axis2);
	}
	else
	{
		double dRadius = sqrt(Axis1 * Axis1 + Axis2 * Axis2);
		double dAngle;
		if(Axis1 == 0.0)
		{
			if(Axis2 > 0.0)
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

		if((Axis1 < 0.0) && (Axis2 > 0.0))
		{
			dAngle += NJR::dPI;
		}
		else if((Axis1 < 0.0) && (Axis2 < 0.0))
		{
			dAngle += NJR::dPI;
		}

		dAngle += dAnglePlus;

		if(dAngle >= NJR::dDoublePI)
		{
			dAngle -= NJR::dDoublePI;
		}

		return std::make_pair(dRadius*cos(dAngle), dRadius*sin(dAngle));
	}
}

NJR::Vector3d& Rotate3Axis
	(NJR::Vector3d vTarget,
	 const double& Angle2XAxis,
	 const double& Angle2YAxis,
	 const double& Angle2ZAxis )
{
	std::pair<double, double> pNewAxisValues;

	pNewAxisValues = Rotate1Axis(vTarget.y(), vTarget.z(), Angle2XAxis);
	vTarget.Set(vTarget.x(), pNewAxisValues.first, pNewAxisValues.second);

	pNewAxisValues = Rotate1Axis(vTarget.z(), vTarget.x(), Angle2YAxis);
	vTarget.Set(pNewAxisValues.second, vTarget.y(), pNewAxisValues.first);

	pNewAxisValues = Rotate1Axis(vTarget.x(), vTarget.y(), Angle2ZAxis);
	vTarget.Set(pNewAxisValues.first, pNewAxisValues.second, vTarget.z());

	return vTarget;
}

void DOWorld::Rotate
	(const double& Angle2XAxis,
	 const double& Angle2YAxis,
	 const double& Angle2ZAxis )
{
	NJR::Vector3d vV1, vV2;
	std::pair<double, double> pNewPosition, pNewVelocity, pNewAngularVelocity;
	for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
		cDOStatus[ul]
			->SetPosition
				(Rotate3Axis
					(cDOStatus[ul]->GetPosition(),
					 Angle2XAxis,
					 Angle2YAxis,
					 Angle2ZAxis)                 );

		vV1
			= Rotate3Axis
				(cDOStatus[ul]->GetOrientationX(),
				 Angle2XAxis,
				 Angle2YAxis,
				 Angle2ZAxis                      );

		vV2
			= Rotate3Axis
				(cDOStatus[ul]->GetOrientationZ(),
				 Angle2XAxis,
				 Angle2YAxis,
				 Angle2ZAxis                      );

		cDOStatus[ul]->SetOrientation(vV1, vV2);

		cDOStatus[ul]
			->SetVelocity
				(Rotate3Axis
					(cDOStatus[ul]->GetVelocity(),
					 Angle2XAxis,
					 Angle2YAxis,
					 Angle2ZAxis)                 );

		cDOStatus[ul]
			->SetAngularVelocity
				(Rotate3Axis
					(cDOStatus[ul]->GetAngularVelocity(),
					 Angle2XAxis,
					 Angle2YAxis,
					 Angle2ZAxis)                 );
	}
}

void DOWorld::Rotate
	(const NJR::Vector3d& eX, const NJR::Vector3d& eZ, const std::string& DOName)
{
	NJR::Vector3d eY(eZ * eX);
	NJR::Vector3d position, orientationX, orientationZ;
	for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
		if(cDOStatus[ul]->GetDOName() == DOName)
		{
			position = cDOStatus[ul]->GetPosition();
			cDOStatus[ul]
				->SetPosition
					(NJR::Vector3d(position % eX, position % eY, position % eZ));
			orientationX = cDOStatus[ul]->GetOrientationX();
			orientationZ = cDOStatus[ul]->GetOrientationZ();
			cDOStatus[ul]
				->SetOrientation
					(NJR::Vector3d
						(orientationX % eX, orientationX % eY, orientationX % eZ),
					 NJR::Vector3d
						(orientationZ % eX, orientationZ % eY, orientationZ % eZ) );
		};
	};
};

bool DOWorld::Check() const
{
	std::string doname;
	std::vector<DOStatus *>::const_iterator idos;

	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		doname=(*idos)->GetDOName();
		if (DOWorld::GetDOModel(doname) == 0)
		{
			std::cout
				<<"unknown model of Discrete Object->"
				<< doname.c_str()
				<< std::endl;
			return false;
		}
	}

	if (pSystemParameter->GetDONumber()!=cDOStatus.size())
	{
		std::cout << "Number of Discrete Objects don't equal to DOStatus" << std::endl;
		return false;
	}

	return true;
};

double DOWorld::Volume(double& dMeshLength) const
{
	const Boundary      pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*     pdos    = 0;
	const DOModel*      pdoml   = 0;
	      unsigned long ulNumDO = pSystemParameter->GetDONumber();

	std::vector<unsigned long> TargetedSerialNumber;
	for(unsigned long ul=0; ul<ulNumDO; ul++)
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
		};
	};

	double        dMeshVolume = pow(dMeshLength, 3.0);
	double        dVolume     = 0.0;
	double        dRadius;
	unsigned long ulCounter;
	bool          bVolumeFilled;
	NJR::Vector3d   vMeshCenter;
	for(double dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength;
		dX<pZOI.GetUpperPoint().x();
		dX+=dMeshLength                                   )
	{
		for(double dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength;
			dY<pZOI.GetUpperPoint().y();
			dY+=dMeshLength                                   )
		{
			for(double dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength;
				dZ<pZOI.GetUpperPoint().z();
				dZ+=dMeshLength                                   )
			{
				vMeshCenter.Set(dX, dY, dZ);
				ulCounter     = 0;
				bVolumeFilled = false;
				while((ulCounter<TargetedSerialNumber.size()) && (!bVolumeFilled))
				{
					pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
					pdoml   = GetDOModel(pdos->GetDOName());
					dRadius = pdoml->GetShapeAttributes().sphere.radius;
					if((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
					{
						dVolume       += dMeshVolume;
						bVolumeFilled  = true;
					};
					ulCounter++;
				};
			};
		};
	};

	return dVolume;
};

double DOWorld::ProjectedAreaX(double& dMeshLength) const
{
	const Boundary      pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*     pdos    = 0;
	const DOModel*      pdoml   = 0;
	      unsigned long ulNumDO = pSystemParameter->GetDONumber();

	std::vector<unsigned long> TargetedSerialNumber;
	for(unsigned long ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition
					(NJR::Vector3d
						(0.0,
						 pdos->GetPosition().y(),
						 pdos->GetPosition().z() ));
			};
		};
	};

	double        dMeshArea = dMeshLength * dMeshLength;
	double        dArea     = 0.0;
	double        dRadius;
	unsigned long ulCounter;
	bool          bAreaFilled;
	NJR::Vector3d   vMeshCenter;
	for(double dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength;
		dY<pZOI.GetUpperPoint().y();
		dY+=dMeshLength                                   )
	{
		for(double dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength;
			dZ<pZOI.GetUpperPoint().z();
			dZ+=dMeshLength                                   )
		{
			vMeshCenter.Set(0.0, dY, dZ);
			ulCounter   = 0;
			bAreaFilled = false;
			while((ulCounter<TargetedSerialNumber.size()) && (!bAreaFilled))
			{
				pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
				pdoml   = GetDOModel(pdos->GetDOName());
				dRadius = pdoml->GetShapeAttributes().sphere.radius;
				if((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
				{
					dArea       += dMeshArea;
					bAreaFilled  = true;
				};
				ulCounter++;
			};
		};
	};

	return dArea;
};

double DOWorld::ProjectedAreaY(double& dMeshLength) const
{
	const Boundary      pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*     pdos    = 0;
	const DOModel*      pdoml   = 0;
	      unsigned long ulNumDO = pSystemParameter->GetDONumber();

	std::vector<unsigned long> TargetedSerialNumber;
	for(unsigned long ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition
					(NJR::Vector3d
						(pdos->GetPosition().x(),
						 0.0,
						 pdos->GetPosition().z() ));
			};
		};
	};

	double        dMeshArea = dMeshLength * dMeshLength;
	double        dArea     = 0.0;
	double        dRadius;
	unsigned long ulCounter;
	bool          bAreaFilled;
	NJR::Vector3d   vMeshCenter;
	for(double dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength;
		dX<pZOI.GetUpperPoint().x();
		dX+=dMeshLength                                   )
	{
		for(double dZ=pZOI.GetLowerPoint().z()+0.5*dMeshLength;
			dZ<pZOI.GetUpperPoint().z();
			dZ+=dMeshLength                                   )
		{
			vMeshCenter.Set(dX, 0.0, dZ);
			ulCounter   = 0;
			bAreaFilled = false;
			while((ulCounter<TargetedSerialNumber.size()) && (!bAreaFilled))
			{
				pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
				pdoml   = GetDOModel(pdos->GetDOName());
				dRadius = pdoml->GetShapeAttributes().sphere.radius;
				if((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
				{
					dArea       += dMeshArea;
					bAreaFilled  = true;
				};
				ulCounter++;
			};
		};
	};

	return dArea;
};

double DOWorld::ProjectedAreaZ(double& dMeshLength) const
{
	const Boundary      pZOI    = pSystemParameter->GetZoneOfInterest();
	      DOStatus*     pdos    = 0;
	const DOModel*      pdoml   = 0;
	      unsigned long ulNumDO = pSystemParameter->GetDONumber();

	std::vector<unsigned long> TargetedSerialNumber;
	for(unsigned long ul=0; ul<ulNumDO; ul++)
	{
		pdos     = cDOStatus[ul];
		pdoml    = GetDOModel(pdos->GetDOName());
		DOMap pm = DOMap(ul, pdos, pdoml, 0.0);
		if ((DOMap::ISMobile(pm)) && pdoml->GetShapeType() == Sphere)
		{
			if (pZOI.InBoundary(pdos->GetPosition(), pdoml->GetRange()) )
			{
				TargetedSerialNumber.push_back(ul);
				pdos->SetPosition
					(NJR::Vector3d
						(pdos->GetPosition().x(),
						 pdos->GetPosition().y(),
						 0.0                     ));
			};
		};
	};

	double        dMeshArea = dMeshLength * dMeshLength;
	double        dArea     = 0.0;
	double        dRadius;
	unsigned long ulCounter;
	bool          bAreaFilled;
	NJR::Vector3d   vMeshCenter;
	for(double dX=pZOI.GetLowerPoint().x()+0.5*dMeshLength;
		dX<pZOI.GetUpperPoint().x();
		dX+=dMeshLength                                   )
	{
		for(double dY=pZOI.GetLowerPoint().y()+0.5*dMeshLength;
			dY<pZOI.GetUpperPoint().y();
			dY+=dMeshLength                                   )
		{
			vMeshCenter.Set(dX, dY, 0.0);
			ulCounter   = 0;
			bAreaFilled = false;
			while((ulCounter<TargetedSerialNumber.size()) && (!bAreaFilled))
			{
				pdos    = cDOStatus[TargetedSerialNumber[ulCounter]];
				pdoml   = GetDOModel(pdos->GetDOName());
				dRadius = pdoml->GetShapeAttributes().sphere.radius;
				if((vMeshCenter - (pdos->GetPosition())).length() <= dRadius)
				{
					dArea       += dMeshArea;
					bAreaFilled  = true;
				};
				ulCounter++;
			};
		};
	};

	return dArea;
};

const DOStatus DOWorld::GetDOStatus
	(const std::string& DOName, const unsigned long ulSerialNumber) const
{
	if (ulSerialNumber < cDOStatus.size())
	{
		if (cDOStatus[ulSerialNumber]->GetDOName() == DOName)
		{
			return *(cDOStatus[ulSerialNumber]);
		}
		else
		{
			std::cout
				<< "DOWorld::Trace Error (I) !! Can't find DOStatus ["
				<< ulSerialNumber
				<< "]"
				<< std::endl;
			return *(cDOStatus[0]);
		}
	}
	else
	{
		std::cout
			<< "DOWorld::Trace Error (II) !! Can't find DOStatus ["
			<< ulSerialNumber
			<< "]"
			<< std::endl;
		return *(cDOStatus[0]);
	}
};

};   // namespace VEDO
