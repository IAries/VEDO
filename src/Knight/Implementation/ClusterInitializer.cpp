// ClusterInitializer.cpp: implementation of the ClusterInitializer class.
//
//////////////////////////////////////////////////////////////////////

#include <NJR/Interfaces/Vector3d.h>
#include <FrameWork/Interfaces/DOWorld.h>
#include <Knight/Interfaces/ClusterInitializer.h>
#include <Knight/Interfaces/BravaisLatticeWithBasis.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ClusterInitializer::ClusterInitializer(const BravaisLatticeWithBasis *bravais)
{

    for (int coord=0; coord<3; coord++) clusterCenter[coord]=0.0;
    this->bravais = bravais;
}

ClusterInitializer::~ClusterInitializer()
{
}

void ClusterInitializer::SetBravaisLattice(const BravaisLatticeWithBasis *bravais)
{
    this->bravais = bravais;
}

void ClusterInitializer::Create(std::string doName, VEDO::DOWorld* pWorld)
{
    int zeroCenter[3];
    for (int coord=0; coord<3; coord++) zeroCenter[coord] = 0;

	VEDO::DOStatus dos
		(doName,
		NJR::Vector3d(NJRDXF::ZERO),
		NJR::Vector3d(NJRDXF::ZERO),
		NJR::Vector3d(NJRDXF::AXIALX),
		NJR::Vector3d(NJRDXF::AXIALZ),
		NJR::Vector3d(NJRDXF::ZERO));

    // Recursion to work in general 3:
    MakeSlab(zeroCenter, 3, dos, pWorld);
}

void ClusterInitializer::SetCenter(double clusterCenter[3])
{
    for (int coord=0; coord<3; coord++)
    {
        this->clusterCenter[coord] = clusterCenter[coord];
    }
}

void ClusterInitializer::MakeSlab(int center[3],
                                  int sub3,
                                  VEDO::DOStatus& dos, VEDO::DOWorld* pWorld)
{
    int newCenter[3];
    for(int coord=sub3; coord<3; coord++)
        newCenter[coord] = center[coord];
    if (sub3 > 1)
    {
        newCenter[sub3-1] = minSize[sub3-1];
        for (int index = minSize[sub3-1]; index <= maxSize[sub3-1]; index++)
        {
            MakeSlab(newCenter, sub3-1, dos, pWorld);
            newCenter[sub3-1]++;
        }
    }
    else	// One 3 left
    {
        double potentialPos[3];
        double latticePotentialPos[3];
        const int nAtomsPerCell = bravais->GetNAtomsPerCell();
        for (int index = minSize[0]; index <= maxSize[0]; index++)
        {
            newCenter[0] = index;
            for(int intraCell=0; intraCell<nAtomsPerCell; intraCell++)
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
						(NJR::Vector3d
							(potentialPos[0],
							 potentialPos[1],
							 potentialPos[2]));
					pWorld->AddDOStatus(new VEDO::DOStatus(dos));
                }
            }
        }
    }

}


double ClusterInitializer::GetMaxSize() const
{
    // Returns rough maximum size for graphics

    double max = 0;
    for (int coord=0; coord<3; coord++)
    {
        double br2 = 0;	// Squared length of Bravais lattice std::vector
        for (int coord2=0; coord2<3; coord2++)
        {
            double brComponent = bravais->GetLatticeVectors()[coord][coord2];
            br2 += brComponent * brComponent;
        }
        double extent = (double) (maxSize[coord] - minSize[coord])*sqrt(br2);
        if (extent > max) max = extent;
    }
    return max;
}
