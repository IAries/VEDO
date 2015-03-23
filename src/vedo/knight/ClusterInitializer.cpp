// ClusterInitializer.cpp: implementation of the ClusterInitializer class.
//
//////////////////////////////////////////////////////////////////////

#include <aries/utility/Vector3d.h>
#include <vedo/framework/DOWorld.h>
#include <vedo/knight/ClusterInitializer.h>
#include <vedo/knight/BravaisLatticeWithBasis.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ClusterInitializer::ClusterInitializer(const BravaisLatticeWithBasis *bravais)
{

    for (vedo::_uint_t coord=0; coord<3; coord++) clusterCenter[coord]=0.0;
    this->bravais = bravais;
}

ClusterInitializer::~ClusterInitializer()
{
}

void ClusterInitializer::SetBravaisLattice(const BravaisLatticeWithBasis *bravais)
{
    this->bravais = bravais;
}

void ClusterInitializer::Create(std::string doName, vedo::DOWorld* pWorld)
{
    vedo::_uint_t zeroCenter[3];
    for (vedo::_uint_t coord=0; coord<3; coord++) zeroCenter[coord] = 0;

	vedo::DOStatus dos
		(doName,
		aries::Vector3df(),
		aries::Vector3df(),
		aries::Vector3df(1.0, 0.0, 0.0),
		aries::Vector3df(0.0, 0.0, 1.0),
		aries::Vector3df(),
		aries::Vector3df(),
		aries::Vector3df()              );

    // Recursion to work in general 3:
    MakeSlab(zeroCenter, 3, dos, pWorld);
}

void ClusterInitializer::SetCenter(vedo::_float_t clusterCenter[3])
{
    for (vedo::_uint_t coord=0; coord<3; coord++)
    {
        this->clusterCenter[coord] = clusterCenter[coord];
    }
}

void ClusterInitializer::MakeSlab(vedo::_uint_t center[3],
                                  vedo::_uint_t sub3,
                                  vedo::DOStatus& dos, vedo::DOWorld* pWorld)
{
    vedo::_uint_t newCenter[3];
    for(vedo::_uint_t coord=sub3; coord<3; coord++)
        newCenter[coord] = center[coord];
    if (sub3 > 1)
    {
        newCenter[sub3-1] = minSize[sub3-1];
        for (vedo::_uint_t index = minSize[sub3-1]; index <= maxSize[sub3-1]; index++)
        {
            MakeSlab(newCenter, sub3-1, dos, pWorld);
            newCenter[sub3-1]++;
        }
    }
    else	// One 3 left
    {
        vedo::_float_t potentialPos[3];
        vedo::_float_t latticePotentialPos[3];
        const vedo::_uint_t nAtomsPerCell = bravais->GetNAtomsPerCell();
        for (vedo::_uint_t index = minSize[0]; index <= maxSize[0]; index++)
        {
            newCenter[0] = index;
            for(vedo::_uint_t intraCell=0; intraCell<nAtomsPerCell; intraCell++)
            {
                latticePotentialPos[0] = newCenter[0] +
                        bravais->GetAtomsCoordinatesInCell(intraCell).x();
                latticePotentialPos[1] = newCenter[1] +
                        bravais->GetAtomsCoordinatesInCell(intraCell).y();
                latticePotentialPos[2] = newCenter[2] +
                        bravais->GetAtomsCoordinatesInCell(intraCell).z();
                bravais
                	->CartesianCoordinatesFromLatticeCoordinates
                		(potentialPos, latticePotentialPos);
                if (Inside(potentialPos))
                {
					dos.SetPosition
						(aries::Vector3df
							(potentialPos[0],
							 potentialPos[1],
							 potentialPos[2]));
					pWorld->AddDOStatus(new vedo::DOStatus(dos));
                }
            }
        }
    }

}


vedo::_float_t ClusterInitializer::GetMaxSize() const
{
    // Returns rough maximum size for graphics

    vedo::_float_t max = 0.0;
    for (vedo::_uint_t coord=0; coord<3; coord++)
    {
        vedo::_float_t br2 = 0;	// Squared length of Bravais lattice std::vector
        for (vedo::_uint_t coord2=0; coord2<3; coord2++)
        {
            vedo::_float_t brComponent = bravais->GetLatticeVectors()[coord][coord2];
            br2 += brComponent * brComponent;
        }
        vedo::_float_t extent = (vedo::_float_t) (maxSize[coord] - minSize[coord])*sqrt(br2);
        if (extent > max) max = extent;
    }
    return max;
}
