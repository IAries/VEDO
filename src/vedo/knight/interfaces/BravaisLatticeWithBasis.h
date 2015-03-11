// BravaisLatticeWithBasis.h: interface for the BravaisLatticeWithBasis class.
//
//////////////////////////////////////////////////////////////////////

#ifndef BRAVAISLATTICEWITHBASIS_H
#define BRAVAISLATTICEWITHBASIS_H

// Unfortunately, the word "basis" is used both for the basis vectors for the crystal lattice
// and for the coordinates of the atoms within the unit cell.
// We're calling the former "LatticeVector" and the latter "AtomCoordinatesInCell"

#include <vedo/njr/interfaces/Vector3d.h>
#include <vedo/constants/interfaces/Constants.h>
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

    inline void SetOrigin(const vedo::vedo_float_t origin[3])
    {
    	for (vedo::vedo_int_t coord=0; coord<3; coord++)
    	{
    		this->origin[coord] = origin[coord];
    	}
    }

    inline const vedo::vedo_float_t (&GetOrigin() const)[3]
    {
    	return origin;
    }

    inline const vedo::vedo_float_t (&GetLatticeVectors() const)[3][3]
    {
    	return latticeVectors;
    }

    void CartesianCoordinatesFromLatticeCoordinates
    	(vedo::vedo_float_t cartesianCoordinates[3],
    	 const vedo::vedo_float_t latticeCoordinates[3]) const;

    void LatticeCoordinatesFromCartesianCoordinates
    	(vedo::vedo_float_t latticeCoordinates[3],
    	 const vedo::vedo_float_t cartesianCoordinates[3]) const;

    void ClosestLatticeSiteIndices
    	(vedo::vedo_int_t latticeIndices[3],
    	 const vedo::vedo_float_t cartesianCoordinates[3]) const;

    void ClosestAtomCartesianCoordinates
    	(vedo::vedo_float_t atomCartesianCoordinates[3],
    	 const vedo::vedo_float_t position[3]) const;

    //! Rotate the current lattice vectors using \a rotation as a rotation matrix.
    /*! Note that we check that \a rotation is really a rotation matrix, namely
    that \a det \a rotation = +1 , and the norm of its colums and rows is 1.
    */
    void RotateLatticeVectors(vedo::vedo_float_t rotation[3][3]);

    //! aligns the 1st lattice std::vector along the 1st coordinate, the 2nd std::vector in the 1-2 plane etc...
    virtual void ResetOrientation();

    void RescaleLatticeVectors(vedo::vedo_float_t a);

	void RescaleLatticeVectors(vedo::vedo_float_t a[3]);

    vedo::vedo_float_t GetVolumeOfCell() const{ return volumeOfCell;}

    inline vedo::vedo_int_t GetNAtomsPerCell() const
    {
    	return atomsCoordinatesInCell.size();
    }

    inline vedo::vedo_float_t GetLatticeSpacing() const
    {
    	return latticeSpacing;
    }

    inline vedo::vedo_float_t GetMaxIntraCellDistance() const
    {
    	return maxIntraCellDistance;
    }

    //! the minimal distance separating equivalent atoms
    inline vedo::vedo_float_t GetMinInteratomicSpacing() const
    {
    	return minInteratomicSpacing;
    }

    //! This function is swigged in BravaisLatticeWithBasis.i
    void SetAtomsCoordinatesInCell
    	(std::vector<njr::Vector3d> atomsCoord,
    	 std::vector<std::string> *atomTypes=0);

    virtual void AddAtomInCell(njr::Vector3d atomCoord, std::string name="");

    inline const njr::Vector3d &GetAtomsCoordinatesInCell(vedo::vedo_int_t atom) const
    {
    	return atomsCoordinatesInCell[atom];
    }

    inline const std::vector<std::string> &GetAtomTypes() const
    {
    	return atomTypes;
    }

protected:
    vedo::vedo_float_t origin[3];

    //! lattice vectors: first index is which std::vector, second is component
    /*! First index is which std::vector, second is component
        This is natural in that the basisVector[0] is the first basis std::vector, but
        is backwards from the point of view of thinking of the array of basis vectors
        as a transformation matrix.00
    */
    vedo::vedo_float_t latticeVectors[3][3];

    // The transpose of the transformation from real space to the lattice coordinate system
    vedo::vedo_float_t inverseOfLatticeVectors[3][3];

    vedo::vedo_float_t latticeSpacing;  // this is the minimal distance separating two equivallent atoms.
    vedo::vedo_float_t minInteratomicSpacing;
    vedo::vedo_float_t maxIntraCellDistance;

    vedo::vedo_float_t volumeOfCell;
//! The coordinates of the atoms in units of the lattice vectors (coordinates must be between 0 and 1)
    std::vector<njr::Vector3d> atomsCoordinatesInCell;
//! The type name of the atoms in the basis
    std::vector<std::string> atomTypes;

    void UpdateMemberVariables();  //called when we change the latticeVectors

    vedo::vedo_float_t Determinant(const vedo::vedo_float_t M[3][3],const vedo::vedo_int_t dim);
    vedo::vedo_float_t DotProduct(vedo::vedo_float_t const *v1 , vedo::vedo_float_t const *v2, vedo::vedo_int_t n);

    virtual void SetLatticeVectors(const vedo::vedo_float_t latticeVectors[3][3]);
};

class GeneralLattice:public BravaisLatticeWithBasis
{
public:
    void SetLatticeVectors(const vedo::vedo_float_t latticeVectors[3][3])
    {BravaisLatticeWithBasis::SetLatticeVectors(latticeVectors);};
    GeneralLattice(vedo::vedo_float_t a = 1.0);  // by default, a simple cubic lattice of lattice constant a.
    GeneralLattice(const vedo::vedo_float_t latticeVectors[3][3]);
    ~GeneralLattice() {};
};

#endif // !defined(BRAVAISLATTICEWITHBASIS_H)

