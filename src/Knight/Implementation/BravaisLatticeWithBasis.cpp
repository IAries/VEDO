// BravaisLatticeWithBasis.cpp: implementation of the BravaisLatticeWithBasis class.
//
//////////////////////////////////////////////////////////////////////

#include <Knight/Interfaces/BravaisLatticeWithBasis.h>
#include <NJR/Interfaces/vector3d.h>
#include <limits>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BravaisLatticeWithBasis::BravaisLatticeWithBasis()
{
    for(int coord=0; coord<3; coord++) origin[coord] = 0.;
	// we temporarily set a cubic lattice
    for(int vec=0; vec<3; vec++)
        for(int coord=0; coord<3; coord++)
            latticeVectors[vec][coord] = (vec==coord? 1. :0.);

    vector<NJRvector3d> atomsCoord;
    atomsCoord.push_back(NJRvector3d(0., 0., 0.));
    SetAtomsCoordinatesInCell(atomsCoord);
}

BravaisLatticeWithBasis::~BravaisLatticeWithBasis()
{

}

void BravaisLatticeWithBasis::CartesianCoordinatesFromLatticeCoordinates(
                              double cartesianCoordinates[3],
                              const double latticeCoordinates[3]) const
{
    for (int i=0; i<3; i++)
    {
        cartesianCoordinates[i] = origin[i];
        for (int j=0; j<3; j++)
        {
            cartesianCoordinates[i]
            	+= latticeVectors[j][i] * latticeCoordinates[j];
        }
    }
}

void BravaisLatticeWithBasis::LatticeCoordinatesFromCartesianCoordinates(
                              double latticeCoordinates[3],
                              const double cartesianCoordinates[3]) const
{
    for (int i=0; i<3; i++)
    {
        latticeCoordinates[i]=0;
        for (int j=0; j<3; j++)
        {
            latticeCoordinates[i] += inverseOfLatticeVectors[j][i] *
                                    (cartesianCoordinates[j]-origin[j]);
        }
    }
}

void BravaisLatticeWithBasis::ClosestLatticeSiteIndices(
                              int latticeIndices[3],
                              const double cartesianCoordinates[3]) const
{
    double latticeCoordinates[3] ;

    this
    	->LatticeCoordinatesFromCartesianCoordinates
    		(latticeCoordinates, cartesianCoordinates);

    int i ;
    for (i=0; i<3; ++i)
    {
        latticeIndices[i] = (int)floor(latticeCoordinates[i]+0.5) ; // truncate the double
    }
}

void BravaisLatticeWithBasis::ClosestAtomCartesianCoordinates(
                              double atomCartesianCoordinates[3],
                              const double position[3]) const
{
    int coord;
    double distSq,closestDistanceSq = numeric_limits<double>::max();
	double atomIndices[3],atomPosition[3];
    // find closest lattice site
    int latticeIndices[3];
    this->ClosestLatticeSiteIndices(latticeIndices,position);

    const int nAtomsPerCell = GetNAtomsPerCell();

    if(nAtomsPerCell == 1)
    {
        for(coord=0;coord<3;++coord)
            atomIndices[coord] = latticeIndices[coord];
        CartesianCoordinatesFromLatticeCoordinates
        	(atomCartesianCoordinates,atomIndices);
        return;
    }

    //loop through all neighboring unit cells
    int neighborIndices[3],nCells = 1;
    for(coord=0;coord<3;++coord)
    {
        nCells *= 3;
        neighborIndices[coord] = - 1;
    }

    for(int cell=0;cell<nCells;++cell)
    {
        // loop through all atoms within each unit cell
        for(int basisAtom=0;basisAtom<nAtomsPerCell;++basisAtom)
        {
            // lattice site is latticeIndices plus neighborIndices
            // atom site is lattice site plus atomsCoordinatesInCell
            atomIndices[0]
            	= latticeIndices[0]
            	+ neighborIndices[0]
            	+ atomsCoordinatesInCell[basisAtom].x();
            atomIndices[1]
            	= latticeIndices[1]
            	+ neighborIndices[1]
            	+ atomsCoordinatesInCell[basisAtom].y();
            atomIndices[2]
            	= latticeIndices[2]
            	+ neighborIndices[2]
            	+ atomsCoordinatesInCell[basisAtom].z();
            CartesianCoordinatesFromLatticeCoordinates
            	(atomPosition, atomIndices);
            // find separation and see if smaller than closestDistanceSq
            distSq = 0.;
            for(coord=0;coord<3;++coord)
            {
                double difference = position[coord] - atomPosition[coord];
                distSq += difference*difference;
            }
            if(distSq < closestDistanceSq)
            {
                closestDistanceSq = distSq;
                for(coord=0;coord<3;++coord)
                    atomCartesianCoordinates[coord] = atomPosition[coord];
            }
        }

        ++neighborIndices[0];
        for(coord=0;coord<3-1;++coord)
        {
            if(neighborIndices[coord] > 1)
            {
                neighborIndices[coord] -= 3;
                ++neighborIndices[coord+1];
            }

        }
    }
    assert(neighborIndices[3-1] == 2);
}


void BravaisLatticeWithBasis::RotateLatticeVectors(double rotation[3][3])
{
    double newLatticeVectors[3][3];
    if(fabs(Determinant(rotation,3)-1.) > 1.0e-11)
	{
        cerr
        	<< "[BravaisLatticeWithBasis::RotateLatticeVectors] "
        	<< "this is not a rotation matrix"
        	<< endl;
		return;
	}
    double test1, test2;   // check that rotation is a rotation matrix (ie det R = +1 , and the norm of its colums and row is 1)
    for(int vec=0; vec<3; vec++)
    {
        test1 = test2 = 0.;
        for(int coord=0; coord<3; coord++)
        {
            test1 += rotation[coord][vec]*rotation[coord][vec];
            test2 += rotation[vec][coord]*rotation[vec][coord];
            newLatticeVectors[vec][coord] = 0.;
            for(int k=0; k<3; k++)
            {
            	newLatticeVectors[vec][coord]
            	+= rotation[coord][k]*latticeVectors[vec][k];
            }
        }
        if((fabs(test1-1.) > 1.0e-10) || (fabs(test2-1.)> 1.0e-10))
		{
			cerr
				<< "[BravaisLatticeWithBasis::RotateLatticeVectors] "
				<< "this is not a rotation matrix"
				<< endl;
			return;
		}
    }
    SetLatticeVectors(newLatticeVectors);
}

void BravaisLatticeWithBasis::ResetOrientation()
{
    assert((3 == 2) || (3 == 3)); // currently set only for 2 or 3 D
    int coord;
    double newVectors[3][3];
    double norms[3];
    for(int vec=0; vec<3; ++vec)
        norms[vec]
        	= sqrt
        		(latticeVectors[vec][0] * latticeVectors[vec][0]
				+ latticeVectors[vec][1] * latticeVectors[vec][1]
				+ latticeVectors[vec][2] * latticeVectors[vec][2]);
    newVectors[0][0] = norms[0];
    for(coord=1; coord<3; coord++) newVectors[0][coord] = 0.;
    double cosTheta = 0., sinTheta;  // Theta is the angle between v1 and v2
    for(coord=0; coord<3; coord++)
        cosTheta += latticeVectors[0][coord] * latticeVectors[1][coord];
    cosTheta /= norms[0]*norms[1];
    sinTheta = sqrt(1.-cosTheta*cosTheta);  // Theta > 0
    newVectors[1][0] = norms[1]*cosTheta;
    newVectors[1][1] = norms[1]*sinTheta;

	for(coord=2; coord<3; coord++) newVectors[1][coord] = 0.;
	double cosPsi, tmp = 0.; // Psi is the angle between v3 and v1 X v2.
	// inverseOfLatticeVectors[*][2] is orthogonal to v1 and v2 and its norm is 1/|v3|cos Psi
	for(coord=0; coord<3; coord++)
	{
		tmp
			+= inverseOfLatticeVectors[coord][2]
			* inverseOfLatticeVectors[coord][2];
	}
	cosPsi = 1./sqrt(tmp)/norms[2];
	newVectors[2][2] = 1./sqrt(tmp);   // = cosPsi * norms[2]
	double proj[3];
	for(coord=0; coord<3; coord++) proj[coord] = latticeVectors[2][coord]
		- inverseOfLatticeVectors[coord][2]/tmp;
	newVectors[2][0] = DotProduct(latticeVectors[0], proj, 3) / norms[0];

	tmp = DotProduct(latticeVectors[1], latticeVectors[2], 3);
	newVectors[2][1]
		= (tmp - newVectors[1][0]*newVectors[2][0])/newVectors[1][1];

	double oldvolume = volumeOfCell;
    SetLatticeVectors(newVectors);
    assert(fabs(volumeOfCell/oldvolume - 1.) < 1.0e-8);
}


void BravaisLatticeWithBasis::RescaleLatticeVectors(double a)
{
    int vec, coord;
    for(vec=0; vec<3; vec++)
    {
        for(coord=0; coord<3; coord++) latticeVectors[vec][coord] *= a;
    }
    UpdateMemberVariables();
}

void BravaisLatticeWithBasis::RescaleLatticeVectors(double a[3])
{
    int vec, coord;
    for(vec=0; vec<3; vec++)
    {
        for(coord=0; coord<3; coord++) latticeVectors[vec][coord] *= a[vec];
    }
    UpdateMemberVariables();
}

void BravaisLatticeWithBasis::UpdateMemberVariables()
{  // called when we change the latticeVectors. Update Lengths, inverseOfLatticeVectors, volumeOfCell
    volumeOfCell = Determinant(latticeVectors,3);
    assert(volumeOfCell > 0.) ; // we impose a positively oriented basis

    // we look for the smallest length of any vector of the form \sum a_i v_i,
    //			where v_i is a lattice vector and a_i = -1, 0, 1
    double maxi = 0.0;
    double mini = numeric_limits<double>::max();
    double miniinside = numeric_limits<double>::max();
    double vect[3] ,norm;
    int i, k, index[3];

    const int nAtomsPerCell = GetNAtomsPerCell();
    assert(nAtomsPerCell > 0);
	int threeToTheDIM = 3;
	for(k = 1; k < 3; ++k) threeToTheDIM *= 3;

    for(i=0;i<threeToTheDIM;i++)
    {
        int tmp = i;
        for(k=0;k<3;k++)
        {
            index[k] = tmp%3;
            tmp /= 3;
        }
        norm = 0.;
        for(k=0; k<3; k++)
        {
            vect[k] = 0.;
            for(int l=0; l<3; l++)
                vect[k] += (index[l]-1)*latticeVectors[l][k];
            norm += vect[k]*vect[k];
        }
        norm = sqrt(norm);
        if(norm>maxi) maxi = norm;
        if(norm<mini && norm != 0.) mini = norm;
        for(int l=0; l<nAtomsPerCell; l++)
            for(int m=0; m<nAtomsPerCell; m++)
            {
                norm = 0;
                for(k=0; k<3; k++)
                {
                    vect[k] = 0.;
					vect[k]
						+= (index[0]
							+ atomsCoordinatesInCell[l].x()
							- atomsCoordinatesInCell[m].x())
						* latticeVectors[0][k];
					vect[k]
						+= (index[1]
							+ atomsCoordinatesInCell[l].y()
							- atomsCoordinatesInCell[m].y())
						* latticeVectors[1][k];
					vect[k]
						+= (index[2]
							+ atomsCoordinatesInCell[l].z()
							- atomsCoordinatesInCell[m].z())
						* latticeVectors[2][k];
                    norm += vect[k]*vect[k];
                }
                norm = sqrt(norm);
                if(norm<miniinside && norm != 0.) miniinside = norm;
            }
    }
    latticeSpacing = mini;
    maxIntraCellDistance = maxi;
    minInteratomicSpacing = miniinside;

    // Inverse. this routine is very basic, but we'll use it a few times and for small matrices
    double tempMat[3][3];
    for(i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            for(k=0;k<i;k++)
            {
                int l;
                for(l=0;l<j;l++) tempMat[k][l] = latticeVectors[k][l];
                for(l=j+1;l<3;l++) tempMat[k][l-1] = latticeVectors[k][l];
            }
            for(k=i+1;k<3;k++)
            {
                int l;
                for(l=0;l<j;l++) tempMat[k-1][l] = latticeVectors[k][l];
                for(l=j+1;l<3;l++) tempMat[k-1][l-1] = latticeVectors[k][l];
            }
            inverseOfLatticeVectors[j][i] =
                Determinant(tempMat,3-1)/volumeOfCell*( (i+j)%2 == 0? 1.:-1.);
        }
    }
}


double BravaisLatticeWithBasis::Determinant(const double M[3][3],const int dim)
{
    // this routine is very basic, but we'll use it a few times and for small matrices (TC)
    if(dim==1)
    {
        return M[0][0];
    }
    else if(dim==2)
    {
        return M[0][0]*M[1][1] - M[1][0]*M[0][1];
    }
    else
    {
        double temp = 0., matrixtemp[3][3];
        int i, j, k;
        {
            for(i=0; i<dim; i++)
            {
                for(j=1;j<dim;j++)
                {
                    for(k=0;k<i;k++) matrixtemp[j-1][k] = M[j][k];
                    for(k=i+1;k<dim;k++) matrixtemp[j-1][k-1] = M[j][k];
                }
                temp
                	+= M[0][i]
                	* Determinant(matrixtemp,dim-1)
                	* (i%2 == 0 ? 1. : -1.);
            }
            return temp;
        }
    }
}

double BravaisLatticeWithBasis::DotProduct
	(double const *v1, double const *v2, int n)
{
    double tmp=0.;
    for(int coord=0; coord<n; coord++) tmp += v1[coord] * v2[coord];
    return tmp;
}

void BravaisLatticeWithBasis::SetLatticeVectors
	(const double latticeVectors[3][3])
{
    for (int vec = 0; vec < 3; vec++)
    {
        for (int coord = 0; coord < 3; coord++)
        {
            this->latticeVectors[vec][coord] = latticeVectors[vec][coord];
        }
    }
    UpdateMemberVariables();
}

void BravaisLatticeWithBasis::SetAtomsCoordinatesInCell(
                              vector<NJRvector3d> atomsCoord,
                              vector<string> *pAtomTypes)
{
    atomTypes.clear();
    atomsCoordinatesInCell.clear();
    const int nAtomsPerCell = atomsCoord.size();
    for(int at=0; at<nAtomsPerCell; ++at)
    {
        string name = (pAtomTypes ? (*pAtomTypes)[at] : "");
        AddAtomInCell(atomsCoord[at], name);
    }
}

void BravaisLatticeWithBasis::AddAtomInCell(NJRvector3d atomCoord,
                                            string name)
{
    // make sure the atoms coordinates are between 0 and 1.
    double x = fmod(atomCoord.x(), 1.);
    if(x < 0.) x += 1.;
    double y = fmod(atomCoord.y(), 1.);
    if(y < 0.) y += 1.;
    double z = fmod(atomCoord.z(), 1.);
    if(z < 0.) z += 1.;
    atomsCoordinatesInCell.push_back(NJRvector3d(x,y,z));
    atomTypes.push_back(name);
    UpdateMemberVariables();
}

GeneralLattice::GeneralLattice(double a)
{
    double tmpMat[3][3];
    for(int vec=0; vec<3; vec++)
    {
		for(int coord=0; coord<3; coord++)
		{
			tmpMat[vec][coord] = (vec==coord? a :0.0);
		}
	}
    SetLatticeVectors(tmpMat);
}

GeneralLattice::GeneralLattice(const double latticeVectors[3][3])
{
    SetLatticeVectors(latticeVectors);
}


