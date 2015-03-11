// 3DLattices.h: interface for the 3DLattice classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef THREEDIMLATTICES_H
#define THREEDIMLATTICES_H

#include <vedo/knight/interfaces/BravaisLatticeWithBasis.h>
#include <vedo/njr/interfaces/Vector3d.h>

class CubicLattice: public BravaisLatticeWithBasis
{
public:

    void SetLatticeConstant(vedo::vedo_float_t a);

    inline vedo::vedo_float_t GetLatticeConstant() const
    {
    	return latticeConstant;
    }

    CubicLattice(vedo::vedo_float_t a);

    inline ~CubicLattice()
    {
    };

protected:

    vedo::vedo_float_t latticeConstant;
};


class FCCLattice:public CubicLattice
{
public:

    FCCLattice(vedo::vedo_float_t a=1.0);

    ~FCCLattice(){}

    void AddAtomInCell(njr::Vector3d atomCoord, std::string name="");
};

class PrimitiveFCCLattice:public BravaisLatticeWithBasis
{

public:

    PrimitiveFCCLattice(vedo::vedo_float_t a=1.0);

    ~PrimitiveFCCLattice(){};

    //! restores the usual definition of the lattice vectors.
    void ResetOrientation();

    void SetLatticeConstant(vedo::vedo_float_t a);

    inline vedo::vedo_float_t GetLatticeConstant() const
    {
    	return latticeConstant;
    }

protected:
    //! The lattice constant has the same meaning as in FCCLattice
    vedo::vedo_float_t latticeConstant;
};

class BCCLattice:public CubicLattice
{
public:

    BCCLattice(vedo::vedo_float_t a=1.0);

    void AddAtomInCell(njr::Vector3d atomCoord, std::string name="");
};

class DiamondLattice: public FCCLattice
{
public:

    //! Note that the lattice spacing is the one of the uderlying cubic lattice
    DiamondLattice(vedo::vedo_float_t a=1.0);

	inline ~DiamondLattice()
	{
	}

    void AddAtomInCell(njr::Vector3d atomCoord, std::string name="");
};

class TriclinicLattice:public GeneralLattice
{
public:

    TriclinicLattice
    	(const vedo::vedo_float_t latticeConstants[3],
		 const vedo::vedo_float_t anglesInUnitCell[3]);

    //! Default constructor needed for serialization

    TriclinicLattice();

	inline ~TriclinicLattice()
	{
	};

    void SetLatticeConstants(const vedo::vedo_float_t latticeConstants[3]);

    void SetAnglesInUnitCell(const vedo::vedo_float_t anglesInUnitCell[3]);

    inline const vedo::vedo_float_t(&GetLatticeConstants()const) [3]
    {
    	return latticeConstants;
    }

    inline const vedo::vedo_float_t(&GetAnglesInUnitCell()const) [3]
    {
    	return anglesInUnitCell;
    }

protected:

    vedo::vedo_float_t latticeConstants[3];
    vedo::vedo_float_t anglesInUnitCell[3];
    void Set3DLatticeVectors();
};

#endif // !defined(THREEDIMLATTICES_H)
