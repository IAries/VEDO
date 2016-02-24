#include <vedo/common/NearConsultant.h>
#include <string>

namespace vedo
{

NearConsultant::NearConsultant(DOWorld* DOWorld, IactRecordTab* pIactRecordTab, std::string filename, _uint_t ulwrite):
	Consultant(DOWorld, pIactRecordTab, filename, ulwrite)
{
	NearConsultant::Reset();
}

bool NearConsultant::ISReset()
{
	const SystemParameter* csp = pDOWorld->GetSystemParameter();
	if ( csp->GetTimeCurrent() >= csp->GetTimeStop() )
	{
//		pDOWorld->WriteXML("terminate.xml");
		pDOWorld->WriteIDO("terminate.ido", pIRTbl);
	}
	return false;
}

static bool Detect(const DOStatus* dos1, const DOStatus* dos2, const DOModel* doml1, const DOModel* doml2)
{

	if (doml1->GetShapeType() > doml2->GetShapeType())
	{
	    const DOStatus* tdos  = dos1;
		                dos1  = dos2;
		                dos2  = tdos;
		const DOModel*  tdoml = doml1;
		                doml1 = doml2;
		                doml2 = tdoml;
	}

	if ((doml1->GetShapeType() == Sphere) && (doml2->GetShapeType() == Sphere))
	{
		Vector3df vIm = dos1->GetPosition() - dos2->GetPosition();
		return
			(vIm.length() <= ( (doml1->GetShapeAttributes().sphere.radius + doml2->GetShapeAttributes().sphere.radius) * 1.05));
	}

	if ((doml1->GetShapeType() == Sphere) && (doml2->GetShapeType() == QuasiCylinder))
	{
		_float_t  dHHb   = 0.5 * doml2->GetShapeAttributes().quasicylinder.height;
		Vector3df Ca     = dos1->GetPosition();
		Vector3df Cb     = dos2->GetPosition();
		Vector3df Vaxial = dos2->GetOrientationZ();
		Vector3df Cap;
		_float_t  Dap    = (Ca - Cb).dot(Vaxial);
		Cap = Cb + (Vaxial * Dap);
		Vector3df vIm;

		if ((Dap < dHHb) && (Dap > -dHHb))
		{
			vIm = Cap - Ca;
		}
		else if (Dap >= dHHb)
		{
			vIm = Cb + (Vaxial*dHHb) - Ca;
		}
		else
		{
			vIm = Cb - (Vaxial*dHHb) - Ca;
		}

		return
			(   vIm.length()
			 <= ((doml1->GetShapeAttributes().sphere.radius + doml2->GetShapeAttributes().quasicylinder.radius) * 1.05));
	}

	std::cerr
		<< "Error!! Code: NearConsultant.cpp" << std::endl
		<< "        Note: DOShape is not in the std::list of NearConsultant" << std::endl;
	exit(-1);
}

static bool DetectSphere(const DOStatus* dos1, const DOStatus* dos2, const DOModel* doml1, const DOModel* doml2)
{
	Vector3df vIm = dos1->GetPosition() - dos2 ->GetPosition();
	return (vIm.length() <= ((doml1->GetShapeAttributes().sphere.radius + doml2->GetShapeAttributes().sphere.radius) * 3.0));
}

bool NearConsultant::Reset()
{
	_uint_t ulDONum = pDOWorld->GetSystemParameter()->GetDONumber();
	_uint_t ul, uj;
	std::string doname1, doname2;

	vcDO.clear();
	vcIactMaster.clear();
	vcIactSlave.clear();

	for (ul=0; ul<ulDONum; ++ul)
	{
		vcDO.push_back(ul);
	}

	for (ul=0; ul<ulDONum; ++ul)
	{
		for (uj=ul+1; uj<ulDONum; ++uj)
		{
			const DOStatus*  cpdos1   = pDOWorld->GetDOStatus(ul);
			const DOStatus*  cpdos2   = pDOWorld->GetDOStatus(uj);
			const DOModel*   cpdoml1  = pDOWorld->GetDOModel(cpdos1->GetDOName());
			const DOModel*   cpdoml2  = pDOWorld->GetDOModel(cpdos2->GetDOName());
			const IactModel* cpiactml = pDOWorld->GetIactModel(cpdoml1->GetDOGroup(), cpdoml2->GetDOGroup());

			if (cpiactml == 0)
			{
				continue;
			}

/*
			if (cpiactml->GetExtend()=="Extended")
			{
				if (Detect(cpdos1, cpdos2, cpdoml1, cpdoml2) == false)
				{
					continue;
				}
			}
*/

//			if (cpiactml->GetExtend() == "NoExtend")
//			{
				if (DetectSphere(cpdos1, cpdos2, cpdoml1, cpdoml2) == false)
				{
					continue;
				}
//			}

			vcIactMaster.push_back(ul);
			vcIactSlave.push_back(uj);
		}
	}

	#ifdef _DEBUG
		std::cout << "Near Consultant Interaction size = " << vcIactMaster.size() << std::endl;
	#endif   // _DEBUG

	return true;
}

void NearConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	CollectUserDefinedData(cIact);
}

}   // namespace vedo
