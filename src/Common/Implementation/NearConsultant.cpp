#include <Common/Interfaces/NearConsultant.h>
#include <string>

using namespace std;

NearConsultant::NearConsultant
	(DOWorld* DOWorld,
	IactRecordTab* pIactRecordTab,
	char filename[],
	unsigned long ulwrite         )
: Consultant(DOWorld, pIactRecordTab, filename, ulwrite)
{
	NearConsultant::Reset();
};

bool NearConsultant::ISReset()
{
	const SystemParameter* csp = pDOWorld->GetSystemParameter();
	if ( csp->GetTimeCurrent() >= csp->GetTimeStop() )
	{
//		pDOWorld->WriteXML("terminate.xml");
		pDOWorld->WriteIDO("terminate.ido");
		pIRTbl  ->WriteIRT("terminate.irt");
	}
	return false;
};

static bool Detect
	(const DOStatus* dos1,
	const DOStatus* dos2,
	const DOModel* doml1,
	const DOModel* doml2  )
{

	if ( doml1->GetShapeType() > doml2->GetShapeType() )
	{
	    const DOStatus* tdos  = dos1;
		                dos1  = dos2;
		                dos2  = tdos;
		const DOModel*  tdoml = doml1;
		                doml1 = doml2;
		                doml2 = tdoml;
	}

	if (   (doml1->GetShapeType() == Sphere)
		&& (doml2->GetShapeType() == Sphere) )
	{
		NJRvector3d vIm = dos1->GetPosition() - dos2->GetPosition();
		return
			vIm.length()
			<= ( (doml1->GetShapeAttributes().sphere.radius
				+ doml2->GetShapeAttributes().sphere.radius) * 1.05);
	}

	if (   (doml1->GetShapeType() == Sphere       )
		&& (doml2->GetShapeType() == QuasiCylinder) )
	{
		double dHHb = 0.5 * doml2->GetShapeAttributes().quasicylinder.height;
		NJRvector3d     Ca = dos1->GetPosition();
		NJRvector3d     Cb = dos2->GetPosition();
		NJRvector3d Vaxial = dos2->GetOrientationZ();
		NJRvector3d    Cap;
		double Dap = (Ca - Cb)%Vaxial;
		Cap = Cb + (Vaxial * Dap);
		NJRvector3d vIm;

		if ( (Dap < dHHb) && (Dap > -dHHb) )
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
			vIm.length()
			<= ((doml1->GetShapeAttributes().sphere.radius
				+doml2->GetShapeAttributes().quasicylinder.radius) * 1.05);
	}
	cerr << "DOShape is not in the list of LeapConsultant\n";
	exit(0);
};

static bool DetectSphere
	(const DOStatus* dos1,
	const DOStatus* dos2,
	const DOModel* doml1,
	const DOModel* doml2  )
{
	NJRvector3d vIm = dos1->GetPosition() - dos2 ->GetPosition();
	return
		vIm.length()
		<= ((doml1->GetShapeAttributes().sphere.radius
			+doml2->GetShapeAttributes().sphere.radius) * 3);
};

bool NearConsultant::Reset()
{
	unsigned long ulDONum = pDOWorld->GetSystemParameter()->GetDONumber();
	unsigned long ul, uj;
	string doname1, doname2;

	vcDO.clear();
	vcIactMaster.clear();
	vcIactSlave.clear();

	for (ul=0; ul<ulDONum; ++ul)
		vcDO.push_back(ul);

	for (ul=0; ul<ulDONum; ++ul)
	{
		for (uj=ul+1; uj<ulDONum; ++uj)
		{
			const DOStatus*  cpdos1  = pDOWorld->GetDOStatus(ul);
			const DOStatus*  cpdos2  = pDOWorld->GetDOStatus(uj);
			const DOModel*   cpdoml1 = pDOWorld->GetDOModel(cpdos1->GetDOName());
			const DOModel*   cpdoml2 = pDOWorld->GetDOModel(cpdos2->GetDOName());

			const IactModel* cpiactml
				= pDOWorld->GetIactModel
					(cpdoml1->GetDOGroup(), cpdoml2->GetDOGroup());

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

//            if (cpiactml->GetExtend() == "NoExtend")
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
	cerr << "Near Consultant Interaction size = " << vcIactMaster.size() << '\n';

	return true;
};

void NearConsultant::RebuildIactRecordTab(IactContainer& cIact)
{
	CollectUserDefinedData(cIact);
};
