#ifndef _VEDO_MODULE_LIST_H
#define _VEDO_MODULE_LIST_H

#include <vedo/framework/Assembler.h>

#include <vedo/common/CDQuasiCylinder2QuasiCylinder.h>
#include <vedo/common/CDSphere2QuasiCylinder.h>
#include <vedo/common/CDSphere2QuasiCylinderAT.h>
#include <vedo/common/CDSphere2QuasiPlate.h>
#include <vedo/common/CDSphere2QuasiPlateWithCircularHole.h>
#include <vedo/common/CDSphere2Sphere.h>
#include <vedo/common/CDSphere2SphereAT.h>

#include <vedo/common/DOConstrainedQuasiCylinder.h>
#include <vedo/common/DOConstrainedQuasiPlate.h>
#include <vedo/common/DOFixedQuasiCylinder.h>
#include <vedo/common/DOFixedQuasiPlate.h>
#include <vedo/common/DOFixedQuasiPlateWithCircularHole.h>
#include <vedo/common/DOFixedSphere.h>
#include <vedo/common/DOOrbitalQuasiCylinder.h>
#include <vedo/common/DOOrbitalQuasiPlate.h>
#include <vedo/common/DOQuasiCylinder.h>
#include <vedo/common/DOQuasiPlate.h>
#include <vedo/common/DOQuasiPlateWithCircularHole.h>
#include <vedo/common/DOSphere.h>

//#include <vedo/Common/GSComplexShape.h>
//#include <vedo/Common/GSCylinder.h>
//#include <vedo/Common/GSEllipsoid.h>
//#include <vedo/Common/GSRectangle.h>
//#include <vedo/Common/GSSphere.h>

#include <vedo/common/ISwBSDBF.h>
#include <vedo/common/ISwBtSDBF.h>
#include <vedo/common/ISwH.h>
#include <vedo/common/ISwHertz.h>

namespace vedo
{

Assembler* CreateNewAssembler()
{
	Assembler* pAssembler = new Assembler;

	pAssembler->AddDO(new DOcf<DOConstrainedQuasiCylinder>(QuasiCylinder, constrained));
	pAssembler->AddDO(new DOcf<DOConstrainedQuasiPlate>(QuasiPlate, constrained));
	pAssembler->AddDO(new DOcf<DOFixedQuasiCylinder>(QuasiCylinder, fixed));
	pAssembler->AddDO(new DOcf<DOFixedQuasiPlate>(QuasiPlate, fixed));
	pAssembler->AddDO(new DOcf<DOFixedQuasiPlateWithCircularHole>(QuasiPlateWithCircularHole, fixed));
	pAssembler->AddDO(new DOcf<DOFixedSphere>(Sphere, fixed));
	pAssembler->AddDO(new DOcf<DOOrbitalQuasiCylinder>(QuasiCylinder, orbital));
	pAssembler->AddDO(new DOcf<DOOrbitalQuasiPlate>(QuasiPlate, orbital));
	pAssembler->AddDO(new DOcf<DOQuasiCylinder>(QuasiCylinder, mobile));
	pAssembler->AddDO(new DOcf<DOQuasiPlate>(QuasiPlate, mobile));
	pAssembler->AddDO(new DOcf<DOQuasiPlateWithCircularHole>(QuasiPlateWithCircularHole, mobile));
	pAssembler->AddDO(new DOcf<DOSphere>(Sphere, mobile));

	pAssembler->AddIS(new IScf<ISwBSDBF>("ISwBSDBF"));
	pAssembler->AddIS(new IScf<ISwBtSDBF>("ISwBtSDBF"));
	pAssembler->AddIS(new IScf<ISwH>("ISwH"));
	pAssembler->AddIS(new IScf<ISwHertz>("ISwHertz"));

	pAssembler->AddCD(new CDcf<CDQuasiCylinder_QuasiCylinder>       (QuasiCylinder, QuasiCylinder             , "List- <CT>ISwBSDBF <CT>ISwBtSDBF <CT>ISwH <CT>ISwHertz"));
	pAssembler->AddCD(new CDcf<CDSphere_Sphere>                     (Sphere       , Sphere                    , "List- <CT>ISwBSDBF <CT>ISwBtSDBF <CT>ISwH <CT>ISwHertz"));
	pAssembler->AddCD(new CDcf<CDSphere_QuasiCylinder>              (Sphere       , QuasiCylinder             , "List- <CT>ISwBSDBF <CT>ISwBtSDBF <CT>ISwH <CT>ISwHertz"));
	pAssembler->AddCD(new CDcf<CDSphere_QuasiPlate>                 (Sphere       , QuasiPlate                , "List- <CT>ISwBSDBF <CT>ISwBtSDBF <CT>ISwH <CT>ISwHertz"));
	pAssembler->AddCD(new CDcf<CDSphere_QuasiPlateWithCircularHole> (Sphere       , QuasiPlateWithCircularHole, "List- <CT>ISwBSDBF <CT>ISwBtSDBF <CT>ISwH <CT>ISwHertz"));

    return pAssembler;
}

}

#endif // _VEDO_MODULE_LIST_H
