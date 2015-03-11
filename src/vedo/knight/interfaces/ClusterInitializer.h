// ClusterInitializer.h: interface for the ClusterInitializer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CLUSTERINITIALIZER_H
#define CLUSTERINITIALIZER_H

#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/DOStatus.h>
#include <vedo/knight/interfaces/BravaisLatticeWithBasis.h>
#include <string>

//! Base class for creation of clusters of atoms of certain shapes.
/*!	This is a Factory Method base class.
Each implementation of ClusterInitializer will need to define an Inside function,
set up appropriate internal variables for the shape and size of the cluster,
and set up the maxSize array.
*/

class ClusterInitializer
{
public:
    //! Returns a rough estimate of the largest 3 of the cluster
    /*! in real units. This is meant to be useful for graphics */
    vedo::vedo_float_t GetMaxSize() const;

    //! Constructor.
    /*! When overloaded, must set up maxSize array in initializer
    */
    ClusterInitializer(const BravaisLatticeWithBasis *bravais);
    //! Destructor.
    virtual ~ClusterInitializer();

    //! Factory method to do the actual creation.
    void Create(std::string doName, vedo::DOWorld* pWorld);

    //! Set the center of the cluster.
    void SetCenter(vedo::vedo_float_t clusterCenter[3]);

    //! Returns the center of the cluster (in real units)
    /*! this is meant to be useful for graphics */
    const vedo::vedo_float_t * GetCenter() const {return clusterCenter;}

    //! Set a lattice
    void SetBravaisLattice(const BravaisLatticeWithBasis *bravais) ;

    //! Get the current lattice
    const BravaisLatticeWithBasis *GetBravaisLattice() const {return bravais;}

protected:

    //! The lattice that is used for the creation of the cluster.
    const BravaisLatticeWithBasis *bravais;

    //! Recursive function. Works in any 3
    /*! The \a positions array must be preallocated and is filled by this function.
    \a nAtomsInArrays containts the current number of atoms in the cluster. There's
    one number per type of atoms (and \a positions has one leaf per type) */
    void MakeSlab(vedo::vedo_int_t center[3],
                  vedo::vedo_int_t subDimension,
                  vedo::DOStatus& dos, vedo::DOWorld* pWorld);

    // Called at the begining of Create.
    /*!It sets the bounds on the solid in the lattice coordinates and returns an
    upper bounds to the number of atoms contained in the cluster.
    */
    virtual vedo::vedo_int_t SetMaxSize() = 0;

    //! Tells if point X is inside cluster
    virtual bool Inside(vedo::vedo_float_t X[3]) = 0;

    //! The coordinates of the cluster in real space
    vedo::vedo_float_t clusterCenter[3];

    //! Cluster has maximum extent minSize[i]...maxSize[i] along basis std::vector i
    //@{
    vedo::vedo_int_t minSize[3];
    vedo::vedo_int_t maxSize[3];
    //@}
};

#endif // !defined(CLUSTERINITIALIZER_H)
