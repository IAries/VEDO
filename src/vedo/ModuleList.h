#ifndef _VEDO_MODULE_LIST_H
#define _VEDO_MODULE_LIST_H

#include <vedo/framework/interfaces/Assembler.h>

#include <vedo/common/interfaces/CDSphere2QuasiCylinder.h>
#include <vedo/common/interfaces/CDSphere2QuasiCylinderAT.h>
#include <vedo/common/interfaces/CDSphere2QuasiPlate.h>
#include <vedo/common/interfaces/CDSphere2QuasiPlateWithCircularHole.h>
#include <vedo/common/interfaces/CDSphere2Sphere.h>
#include <vedo/common/interfaces/CDSphere2SphereAT.h>

#include <vedo/common/interfaces/DOConstrainedQuasiCylinder.h>
#include <vedo/common/interfaces/DOConstrainedQuasiPlate.h>
#include <vedo/common/interfaces/DOFixedQuasiCylinder.h>
#include <vedo/common/interfaces/DOFixedQuasiPlate.h>
#include <vedo/common/interfaces/DOFixedQuasiPlateWithCircularHole.h>
#include <vedo/common/interfaces/DOFixedSphere.h>
#include <vedo/common/interfaces/DOOrbitalQuasiCylinder.h>
#include <vedo/common/interfaces/DOOrbitalQuasiPlate.h>
#include <vedo/common/interfaces/DOQuasiCylinder.h>
#include <vedo/common/interfaces/DOQuasiPlate.h>
#include <vedo/common/interfaces/DOQuasiPlateWithCircularHole.h>
#include <vedo/common/interfaces/DOSphere.h>

//#include <vedo/Common/interfaces/GSComplexShape.h>
//#include <vedo/Common/interfaces/GSCylinder.h>
//#include <vedo/Common/interfaces/GSEllipsoid.h>
//#include <vedo/Common/interfaces/GSRectangle.h>
//#include <vedo/Common/interfaces/GSSphere.h>

#include <vedo/common/interfaces/ISwBSDBF.h>
#include <vedo/common/interfaces/ISwBtSDBF.h>

namespace vedo
{

vedo::Assembler* CreateNewAssembler()
{
	vedo::Assembler* pAssembler = new vedo::Assembler;

	pAssembler->AddDO(new vedo::DOcf<vedo::DOConstrainedQuasiCylinder>(vedo::QuasiCylinder, vedo::constrained));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOConstrainedQuasiPlate>(vedo::QuasiPlate, vedo::constrained));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOFixedQuasiCylinder>(vedo::QuasiCylinder, vedo::fixed));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOFixedQuasiPlate>(vedo::QuasiPlate, vedo::fixed));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOFixedQuasiPlateWithCircularHole>(vedo::QuasiPlateWithCircularHole, vedo::fixed));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOFixedSphere>(vedo::Sphere, vedo::fixed));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOOrbitalQuasiCylinder>(vedo::QuasiCylinder, vedo::orbital));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOOrbitalQuasiPlate>(vedo::QuasiPlate, vedo::orbital));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOQuasiCylinder>(vedo::QuasiCylinder, vedo::mobile));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOQuasiPlate>(vedo::QuasiPlate, vedo::mobile));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOQuasiPlateWithCircularHole>(vedo::QuasiPlateWithCircularHole, vedo::mobile));
	pAssembler->AddDO(new vedo::DOcf<vedo::DOSphere>(vedo::Sphere, vedo::mobile));

	pAssembler->AddIS(new vedo::IScf<vedo::ISwBSDBF>("ISwBSDBF"));
	pAssembler->AddIS(new vedo::IScf<vedo::ISwBtSDBF>("ISwBtSDBF"));

    pAssembler->AddCD(new vedo::CDcf<vedo::CDSphere_Sphere>                     (vedo::Sphere, vedo::Sphere                    , "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
    pAssembler->AddCD(new vedo::CDcf<vedo::CDSphere_QuasiCylinder>              (vedo::Sphere, vedo::QuasiCylinder             , "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
	pAssembler->AddCD(new vedo::CDcf<vedo::CDSphere_QuasiPlate>                 (vedo::Sphere, vedo::QuasiPlate                , "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));
	pAssembler->AddCD(new vedo::CDcf<vedo::CDSphere_QuasiPlateWithCircularHole> (vedo::Sphere, vedo::QuasiPlateWithCircularHole, "List- <CT>ISwBSDBF <CT>ISwBtSDBF"));

    return pAssembler;
};

};

#endif _VEDO_MODULE_LIST_H
