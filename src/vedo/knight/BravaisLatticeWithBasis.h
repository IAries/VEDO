// BravaisLatticeWithBasis.h: interface for the BravaisLatticeWithBasis class.
//
//////////////////////////////////////////////////////////////////////

#ifndef BRAVAISLATTICEWITHBASIS_H
#define BRAVAISLATTICEWITHBASIS_H

// Unfortunately, the word "basis" is used both for the basis vectors for the crystal lattice
// and for the coordinates of the atoms within the unit cell.
// We're calling the former "LatticeVector" and the latter "AtomCoordinatesInCell"

#include <vedo/framework/DataType.h>
#include <aries/utility/Constants.h>
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

    inline void SetOrigin(const vedo::_float_t origin[3])
    {
    	for (vedo::_uint_t coord=0; coord<3; coord++)
    	{
    		this->origin[coord] = origin[coord];
    	}
    }

    inline const vedo::_float_t (&GetOrigin() const)[3]
    {
    	return origin;
    }

    inline const vedo::_float_t (&GetLatticeVectors() const)[3][3]
    {
    	return latticeVectors;
    }

    void CartesianCoordinatesFromLatticeCoordinates
    	(vedo::_float_t cartesianCoordinates[3],
    	 const vedo::_float_t latticeCoordinates[3]) const;

    void LatticeCoordinatesFromCartesianCoordinates
    	(vedo::_float_t latticeCoordinates[3],
    	 const vedo::_float_t cartesianCoordinates[3]) const;

    void ClosestLatticeSiteIndices
    	(vedo::_uint_t latticeIndices[3],
    	 const vedo::_float_t cartesianCoordinates[3]) const;

    void ClosestAtomCartesianCoordinates
    	(vedo::_float_t atomCartesianCoordinates[3],
    	 const vedo::_float_t position[3]) const;

    //! Rotate the current lattice vectors using \a rotation as a rotation matrix.
    /*! Note that we check that \a rotation is really a rotation matrix, namely
    that \a det \a rotation = +1 , and the norm of its colums and rows is 1.
    */
    void RotateLatticeVectors(vedo::_float_t rotation[3][3]);

    //! aligns the 1st lattice std::vector along the 1st coordinate, the 2nd std::vector in the 1-2 plane etc...
    virtual void ResetOrientation();

    void RescaleLatticeVectors(vedo::_float_t a);

	void RescaleLatticeVectors(vedo::_float_t a[3]);

    vedo::_float_t GetVolumeOfCell() const{ return volumeOfCell;}

    inline vedo::_uint_t GetNAtomsPerCell() const
    {
    	return atomsCoordinatesInCell.size();
    }

    inline vedo::_float_t GetLatticeSpacing() const
    {
    	return latticeSpacing;
    }

    inline vedo::_float_t GetMaxIntraCellDistance() const
    {
    	return maxIntraCellDistance;
    }

    //! the minimal distance separating equivalent atoms
    inline vedo::_float_t GetMinInteratomicSpacing() const
    {
    	return minInteratomicSpacing;
    }

    //! This function is swigged in BravaisLatticeWithBasis.i
    void SetAtomsCoordinatesInCell
    	(std::vector<aries::Vector3df> atomsCoord,
    	 std::vector<std::string> *atomTypes=0);

    virtual void AddAtomInCell(aries::Vector3df atomCoord, std::string name="");

    inline const aries::Vector3df &GetAtomsCoordinatesInCell(vedo::_uint_t atom) const
    {
    	return atomsCoordinatesInCell[atom];
    }

    inline const std::vector<std::string> &GetAtomTypes() const
    {
    	return atomTypes;
    }

protected:

    vedo::_float_t origin[3];

    //! lattice vectors: first index is which std::vector, second is component
    /*! First index is which std::vector, second is component
        This is natural in that the basisVector[0] is the first basis std::vector, but
        is backwards from the point of view of thinking of the array of basis vectors
        as a transformation matrix.00
    */
    vedo::_float_t latticeVectors[3][3];

    // The transpose of the transformation from real space to the lattice coordinate system
    vedo::_float_t inverseOfLatticeVectors[3][3];

    vedo::_float_t latticeSpacing;  // this is the minimal distance separating two equivallent atoms.
    vedo::_float_t minInteratomicSpacing;
    vedo::_float_t maxIntraCellDistance;

    vedo::_float_t volumeOfCell;
//! The coordinates of the atoms in units of the lattice vectors (coordinates must be between 0 and 1)
    std::vector<aries::Vector3df> atomsCoordinatesInCell;
//! The type name of the atoms in the basis
    std::vector<std::string> atomTypes;

    void UpdateMemberVariables();  //called when we change the latticeVectors

    vedo::_float_t Determinant(const vedo::_float_t M[3][3],const vedo::_uint_t dim);
    vedo::_float_t DotProduct(vedo::_float_t const *v1, vedo::_float_t const *v2, vedo::_uint_t n);

    virtual void SetLatticeVectors(const vedo::_float_t latticeVectors[3][3]);
};

class GeneralLattice:public BravaisLatticeWithBasis
{
public:
    void SetLatticeVectors(const vedo::_float_t latticeVectors[3][3])
    {BravaisLatticeWithBasis::SetLatticeVectors(latticeVectors);};
    GeneralLattice(vedo::_float_t a = 1.0);  // by default, a simple cubic lattice of lattice constant a.
    GeneralLattice(const vedo::_float_t latticeVectors[3][3]);
    ~GeneralLattice() {};
};

#endif // !defined(BRAVAISLATTICEWITHBASIS_H)

