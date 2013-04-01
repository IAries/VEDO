// BravaisLatticeWithBasis.h: interface for the BravaisLatticeWithBasis class.
//
//////////////////////////////////////////////////////////////////////

#ifndef BRAVAISLATTICEWITHBASIS_H
#define BRAVAISLATTICEWITHBASIS_H

// Unfortunately, the word "basis" is used both for the basis vectors for the crystal lattice
// and for the coordinates of the atoms within the unit cell.
// We're calling the former "LatticeVector" and the latter "AtomCoordinatesInCell"

#include <NJR/Interfaces/Vector3d.h>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

class BravaisLatticeWithBasis
{

protected:    // we do not want to instantiate this class.

    BravaisLatticeWithBasis();

public:

    virtual ~BravaisLatticeWithBasis();

    inline void SetOrigin(const double origin[3])
    {
    	for (int coord=0; coord<3; coord++)
    	{
    		this->origin[coord] = origin[coord];
    	}
    }

    inline const double (&GetOrigin() const)[3]
    {
    	return origin;
    }

    inline const double (&GetLatticeVectors() const)[3][3]
    {
    	return latticeVectors;
    }

    void CartesianCoordinatesFromLatticeCoordinates
    	(double cartesianCoordinates[3],
    	 const double latticeCoordinates[3]) const;

    void LatticeCoordinatesFromCartesianCoordinates
    	(double latticeCoordinates[3],
    	 const double cartesianCoordinates[3]) const;

    void ClosestLatticeSiteIndices
    	(int latticeIndices[3],
    	 const double cartesianCoordinates[3]) const;

    void ClosestAtomCartesianCoordinates
    	(double atomCartesianCoordinates[3],
    	 const double position[3]) const;

    //! Rotate the current lattice vectors using \a rotation as a rotation matrix.
    /*! Note that we check that \a rotation is really a rotation matrix, namely
    that \a det \a rotation = +1 , and the norm of its colums and rows is 1.
    */
    void RotateLatticeVectors(double rotation[3][3]);

    //! aligns the 1st lattice std::vector along the 1st coordinate, the 2nd std::vector in the 1-2 plane etc...
    virtual void ResetOrientation();

    void RescaleLatticeVectors(double a);

	void RescaleLatticeVectors(double a[3]);

    double GetVolumeOfCell() const{ return volumeOfCell;}

    inline int GetNAtomsPerCell() const
    {
    	return atomsCoordinatesInCell.size();
    }

    inline double GetLatticeSpacing() const
    {
    	return latticeSpacing;
    }

    inline double GetMaxIntraCellDistance() const
    {
    	return maxIntraCellDistance;
    }

    //! the minimal distance separating equivalent atoms
    inline double GetMinInteratomicSpacing() const
    {
    	return minInteratomicSpacing;
    }

    //! This function is swigged in BravaisLatticeWithBasis.i
    void SetAtomsCoordinatesInCell
    	(std::vector<njr::Vector3d> atomsCoord,
    	 std::vector<std::string> *atomTypes=0);

    virtual void AddAtomInCell(njr::Vector3d atomCoord, std::string name="");

    inline const njr::Vector3d &GetAtomsCoordinatesInCell(int atom) const
    {
    	return atomsCoordinatesInCell[atom];
    }

    inline const std::vector<std::string> &GetAtomTypes() const
    {
    	return atomTypes;
    }

protected:
    double origin[3];

    //! lattice vectors: first index is which std::vector, second is component
    /*! First index is which std::vector, second is component
        This is natural in that the basisVector[0] is the first basis std::vector, but
        is backwards from the point of view of thinking of the array of basis vectors
        as a transformation matrix.00
    */
    double latticeVectors[3][3];

    // The transpose of the transformation from real space to the lattice coordinate system
    double inverseOfLatticeVectors[3][3];

    double latticeSpacing;  // this is the minimal distance separating two equivallent atoms.
    double minInteratomicSpacing;
    double maxIntraCellDistance;

    double volumeOfCell;
//! The coordinates of the atoms in units of the lattice vectors (coordinates must be between 0 and 1)
    std::vector<njr::Vector3d> atomsCoordinatesInCell;
//! The type name of the atoms in the basis
    std::vector<std::string> atomTypes;

    void UpdateMemberVariables();  //called when we change the latticeVectors

    double Determinant(const double M[3][3],const int dim);
    double DotProduct(double const *v1 , double const *v2, int n);

    virtual void SetLatticeVectors(const double latticeVectors[3][3]);
};

class GeneralLattice:public BravaisLatticeWithBasis
{
public:
    void SetLatticeVectors(const double latticeVectors[3][3])
    {BravaisLatticeWithBasis::SetLatticeVectors(latticeVectors);};
    GeneralLattice(double a = 1.0);  // by default, a simple cubic lattice of lattice constant a.
    GeneralLattice(const double latticeVectors[3][3]);
    ~GeneralLattice() {};
};

#endif // !defined(BRAVAISLATTICEWITHBASIS_H)

