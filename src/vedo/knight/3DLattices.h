// 3DLattices.h: interface for the 3DLattice classes.
//
//////////////////////////////////////////////////////////////////////

#ifndef THREEDIMLATTICES_H
#define THREEDIMLATTICES_H

#include <vedo/knight/BravaisLatticeWithBasis.h>
#include <aries/utility/Vector3d.h>

class CubicLattice: public BravaisLatticeWithBasis
{
public:

    void SetLatticeConstant(vedo::_float_t a);

    inline vedo::_float_t GetLatticeConstant() const
    {
    	return latticeConstant;
    }

    CubicLattice(vedo::_float_t a);

    inline ~CubicLattice()
    {
    };

protected:

    vedo::_float_t latticeConstant;
};


class FCCLattice:public CubicLattice
{
public:

    FCCLattice(vedo::_float_t a=1.0);

    ~FCCLattice(){}

    void AddAtomInCell(aries::Vector3df atomCoord, std::string name="");
};

class PrimitiveFCCLattice:public BravaisLatticeWithBasis
{

public:

    PrimitiveFCCLattice(vedo::_float_t a=1.0);

    ~PrimitiveFCCLattice(){};

    //! restores the usual definition of the lattice vectors.
    void ResetOrientation();

    void SetLatticeConstant(vedo::_float_t a);

    inline vedo::_float_t GetLatticeConstant() const
    {
    	return latticeConstant;
    }

protected:
    //! The lattice constant has the same meaning as in FCCLattice
    vedo::_float_t latticeConstant;
};

class BCCLattice:public CubicLattice
{
public:

    BCCLattice(vedo::_float_t a=1.0);

    void AddAtomInCell(aries::Vector3df atomCoord, std::string name="");
};

class DiamondLattice: public FCCLattice
{
public:

    //! Note that the lattice spacing is the one of the uderlying cubic lattice
    DiamondLattice(vedo::_float_t a=1.0);

	inline ~DiamondLattice()
	{
	}

    void AddAtomInCell(aries::Vector3df atomCoord, std::string name="");
};

class TriclinicLattice:public GeneralLattice
{
public:

    TriclinicLattice
    	(const vedo::_float_t latticeConstants[3],
		 const vedo::_float_t anglesInUnitCell[3]);

    //! Default constructor needed for serialization

    TriclinicLattice();

	inline ~TriclinicLattice()
	{
	};

    void SetLatticeConstants(const vedo::_float_t latticeConstants[3]);

    void SetAnglesInUnitCell(const vedo::_float_t anglesInUnitCell[3]);

    inline const vedo::_float_t(&GetLatticeConstants()const) [3]
    {
    	return latticeConstants;
    }

    inline const vedo::_float_t(&GetAnglesInUnitCell()const) [3]
    {
    	return anglesInUnitCell;
    }

protected:

    vedo::_float_t latticeConstants[3];
    vedo::_float_t anglesInUnitCell[3];
    void Set3DLatticeVectors();
};

#endif // !defined(THREEDIMLATTICES_H)
