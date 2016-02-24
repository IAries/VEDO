#include <vedo/knight/3DLattices.h>
#include <aries/utility/Constants.h>
#include <cmath>

////////////////////////  CUBIC LATTICE //////////////////////////////////////

CubicLattice::CubicLattice(vedo::_float_t a)
{
    for(vedo::_uint_t vec=0; vec<3; vec++)
    {
        for(vedo::_uint_t coord=0; coord<3;coord++)
        {
            latticeVectors[vec][coord] = (vec==coord? a : 0.);
        }
    }
    latticeConstant = a;
    UpdateMemberVariables();
}

void CubicLattice::SetLatticeConstant(vedo::_float_t a)
{
    vedo::_float_t factor = a/latticeConstant;
    for(vedo::_uint_t vec=0; vec<3; vec++)
    {
        for(vedo::_uint_t coord=0; coord<3; coord++)
            latticeVectors[vec][coord] *= factor;
    }
    latticeConstant = a;
    UpdateMemberVariables();
}



////////////////////////  FCC LATTICE //////////////////////////////////////

FCCLattice::FCCLattice(vedo::_float_t a): CubicLattice(a)
{
    std::vector<vedo::Vector3df> atomsCoord;
    atomsCoord.push_back(vedo::Vector3df());
    SetAtomsCoordinatesInCell(atomsCoord);
    UpdateMemberVariables();
}

void FCCLattice::AddAtomInCell(vedo::Vector3df atomCoord, std::string name)
{
    vedo::_float_t shifts[4][3] = {{0.,0.,0.},{0.,0.5,0.5},{0.5,0.,0.5},{0.5,0.5,0.}};
    for(vedo::_uint_t at=0; at<4; ++at)
	{
		BravaisLatticeWithBasis::AddAtomInCell
			(atomCoord+vedo::Vector3df(shifts[at][0], shifts[at][1], shifts[at][2]),
			 name);
	}
}


////////////////////////  PRIMITIVE FCC LATTICE //////////////////////////////////////

PrimitiveFCCLattice::PrimitiveFCCLattice(vedo::_float_t a)
{
    for(vedo::_uint_t vec = 0; vec < 3; vec++)
    {
        for(vedo::_uint_t coord = 0; coord < 3; coord++)
        {
            latticeVectors[vec][coord] = (vec == coord ? 0.0 : 0.5*a);
        }
    }
    latticeConstant = a;
    UpdateMemberVariables();
}

void PrimitiveFCCLattice::ResetOrientation()
{
    vedo::_float_t newVectors[3][3];
    for(vedo::_uint_t vec = 0; vec < 3; vec++)
	{
        for(vedo::_uint_t coord = 0; coord < 3; coord++)
        {
            newVectors[vec][coord] =
                (vec == coord ? 0.0 : 0.5 * latticeConstant);
        }
	}
    SetLatticeVectors(newVectors);
}

void PrimitiveFCCLattice::SetLatticeConstant(vedo::_float_t a)
{
    vedo::_float_t factor = a/latticeConstant;
    for(vedo::_uint_t vec=0; vec<3; vec++)
    {
        for(vedo::_uint_t coord=0; coord<3; coord++)
		{
            latticeVectors[vec][coord] *= factor;
		}
    }
    latticeConstant = a;
    UpdateMemberVariables();
}


////////////////////////  BCC LATTICE //////////////////////////////////////

BCCLattice::BCCLattice(vedo::_float_t a): CubicLattice(a)
{
    std::vector<vedo::Vector3df> atomsCoord;
    atomsCoord.push_back(vedo::Vector3df(0., 0., 0.));
    SetAtomsCoordinatesInCell(atomsCoord);
    UpdateMemberVariables();
}

void BCCLattice::AddAtomInCell(vedo::Vector3df atomCoord, std::string name)
{
    vedo::_float_t shifts[2][3] = {{0.,0.,0.},{0.5,0.5,0.5}};
    for(vedo::_uint_t at=0; at<2; ++at)
	{
        BravaisLatticeWithBasis::AddAtomInCell
        	(atomCoord+vedo::Vector3df(shifts[at][0], shifts[at][1], shifts[at][2]),
        	 name);
	}
}


////////////////////////  DIAMOND LATTICE //////////////////////////////////////

DiamondLattice::DiamondLattice(vedo::_float_t a): FCCLattice(a)
{
    std::vector<vedo::Vector3df> atomsCoord;
    atomsCoord.push_back(vedo::Vector3df(0., 0., 0.));
    SetAtomsCoordinatesInCell(atomsCoord);
    UpdateMemberVariables();
}


void DiamondLattice::AddAtomInCell
	(vedo::Vector3df atomCoord, std::string name)
{
    vedo::_float_t shifts[2][3] = {{0.,0.,0.},{0.25,0.25,0.25}};
	FCCLattice::AddAtomInCell
		(atomCoord+vedo::Vector3df(shifts[0][0], shifts[0][1], shifts[0][2]), name);
    FCCLattice::AddAtomInCell
    	(atomCoord+vedo::Vector3df(shifts[1][0], shifts[1][1], shifts[1][2]), name);
}


////////////////////////  TRICLINIC LATTICE //////////////////////////////////////


TriclinicLattice::TriclinicLattice(const vedo::_float_t latticeConstants[3],
                                   const vedo::_float_t anglesInUnitCell[3])
{
    for (vedo::_uint_t vec=0; vec<3; vec++)
    {
        this->latticeConstants[vec] = latticeConstants[vec];
        this->anglesInUnitCell[vec] = anglesInUnitCell[vec];
    }
    Set3DLatticeVectors();
    UpdateMemberVariables();
}

TriclinicLattice::TriclinicLattice()
{
    for(vedo::_uint_t vec=0; vec<3; ++vec)
    {
//        anglesInUnitCell[vec] = M_PI/2;
        anglesInUnitCell[vec] = 1.5707963267949;
        latticeConstants[vec] = 1.0;
    }
    Set3DLatticeVectors();
    UpdateMemberVariables();
}

void TriclinicLattice::SetLatticeConstants(const vedo::_float_t latticeConstants[3])
{
    for(vedo::_uint_t vec=0; vec<3; vec++)
    {
        vedo::_float_t factor = latticeConstants[vec]/this->latticeConstants[vec];
        for(vedo::_uint_t coord=0; coord<3; coord++)
        {
        	this->latticeVectors[vec][coord] *= factor;
        }
        this->latticeConstants[vec] = latticeConstants[vec];
    }
    UpdateMemberVariables();
}

void TriclinicLattice::SetAnglesInUnitCell(const vedo::_float_t anglesInUnitCell[3])
{
    for(vedo::_uint_t coord=0; coord<3; coord++)
        this->anglesInUnitCell[coord] = anglesInUnitCell[coord];
    Set3DLatticeVectors();
    UpdateMemberVariables();
}

void TriclinicLattice::Set3DLatticeVectors()
{
    latticeVectors[0][0] = latticeConstants[0];
    latticeVectors[0][1] = 0.;
    latticeVectors[0][2] = 0.;
    latticeVectors[1][0] = latticeConstants[1]*cos(anglesInUnitCell[2]);
    latticeVectors[1][1] = latticeConstants[1]*sin(anglesInUnitCell[2]);
    latticeVectors[1][2] = 0.;
    latticeVectors[2][0] = latticeConstants[2]* cos(anglesInUnitCell[1]);
    latticeVectors[2][1] = latticeConstants[2]*(cos(anglesInUnitCell[0])-
                           cos(anglesInUnitCell[1])*cos(anglesInUnitCell[2]))
                           /sin(anglesInUnitCell[2]);
    latticeVectors[2][2] = sqrt(latticeConstants[2]*latticeConstants[2]-
                                latticeVectors[2][0]*latticeVectors[2][0]-
                                latticeVectors[2][1]*latticeVectors[2][1]);
    if(latticeVectors[1][1] < 0)
        latticeVectors[2][2] = -latticeVectors[2][2];

    UpdateMemberVariables();
}
