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

    void SetLatticeConstant(double a);

    inline double GetLatticeConstant() const
    {
    	return latticeConstant;
    }

    CubicLattice(double a);

    inline ~CubicLattice()
    {
    };

protected:

    double latticeConstant;
};


class FCCLattice:public CubicLattice
{
public:

    FCCLattice(double a=1.0);

    ~FCCLattice(){}

    void AddAtomInCell(njr::Vector3d atomCoord, std::string name="");
};

class PrimitiveFCCLattice:public BravaisLatticeWithBasis
{

public:

    PrimitiveFCCLattice(double a=1.0);

    ~PrimitiveFCCLattice(){};

    //! restores the usual definition of the lattice vectors.
    void ResetOrientation();

    void SetLatticeConstant(double a);

    inline double GetLatticeConstant() const
    {
    	return latticeConstant;
    }

protected:
    //! The lattice constant has the same meaning as in FCCLattice
    double latticeConstant;
};

class BCCLattice:public CubicLattice
{
public:

    BCCLattice(double a=1.0);

    void AddAtomInCell(njr::Vector3d atomCoord, std::string name="");
};

class DiamondLattice: public FCCLattice
{
public:

    //! Note that the lattice spacing is the one of the uderlying cubic lattice
    DiamondLattice(double a=1.0);

	inline ~DiamondLattice()
	{
	}

    void AddAtomInCell(njr::Vector3d atomCoord, std::string name="");
};

class TriclinicLattice:public GeneralLattice
{
public:

    TriclinicLattice
    	(const double latticeConstants[3],
		 const double anglesInUnitCell[3]);

    //! Default constructor needed for serialization

    TriclinicLattice();

	inline ~TriclinicLattice()
	{
	};

    void SetLatticeConstants(const double latticeConstants[3]);

    void SetAnglesInUnitCell(const double anglesInUnitCell[3]);

    inline const double(&GetLatticeConstants()const) [3]
    {
    	return latticeConstants;
    }

    inline const double(&GetAnglesInUnitCell()const) [3]
    {
    	return anglesInUnitCell;
    }

protected:

    double latticeConstants[3];
    double anglesInUnitCell[3];
    void Set3DLatticeVectors();
};

#endif // !defined(THREEDIMLATTICES_H)
