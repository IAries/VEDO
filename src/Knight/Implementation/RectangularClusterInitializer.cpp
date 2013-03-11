// RectangularClusterInitializer.cpp: implementation of the RectangularClusterInitializer class.
//
//////////////////////////////////////////////////////////////////////

#include <Knight/Interfaces/RectangularClusterInitializer.h>
#include <Knight/Interfaces/BravaisLatticeWithBasis.h>
#include <string>
#include <cmath>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RectangularClusterInitializer::RectangularClusterInitializer
	(double rectangleSize[3],
	 BravaisLatticeWithBasis *bravais): ClusterInitializer(bravais)
{
	SetRectangleSize(rectangleSize);
}


RectangularClusterInitializer::~RectangularClusterInitializer()
{
	// cout << "RectangularClusterInitializer deleted" << endl;
}

void RectangularClusterInitializer::SetRectangleSize
	(const double rectangleSize[3])
{
	int coord;
    for(coord=0; coord<3; coord++)
    {
    	this->rectangleSize[coord] = rectangleSize[coord];
    }
	// XXX JPS added
	SetMaxSize();
}

int RectangularClusterInitializer::SetMaxSize()
{
 	int i;
	double volumeOfRectangle=1.;
	double corner[3], ind[3], maxi[3], mini[3];
	for(int index=0; index<(int)(pow(2.0, 3.0)); index++)
//	for(int index=0; index<8; index++)
	{
		int itmp = index;
		for(i=0; i<3; i++)
		{
			corner[i]
				= clusterCenter[i]
				+ rectangleSize[i] / 2.0 * (double)(itmp % 2 == 0 ? -1:1);
			itmp /= 2;
		}
		bravais->LatticeCoordinatesFromCartesianCoordinates(ind,corner);
		for(i=0; i<3; i++)
		{

			if((index == 0) || (maxi[i] < ind[i])) maxi[i] = ind[i];
			if((index == 0) || (mini[i] > ind[i])) mini[i] = ind[i];
		}
	}
	for(i=0; i<3; i++)
	{
		volumeOfRectangle *= rectangleSize[i] + 2*bravais->GetLatticeSpacing(); // extend the rectangle
		maxSize[i] = (int)maxi[i] + 1;
		minSize[i] = (int)mini[i] - 1;
	}
	return
		int
			(volumeOfRectangle
			 / bravais->GetVolumeOfCell()
			 * bravais->GetNAtomsPerCell());
}


bool RectangularClusterInitializer::Inside(double X[3])
{
	/* Doesn't check if near boundary!
        bool in=true;
        for(int coord=0; coord<3;coord++)
        {
        	in = in && (fabs(X[coord]) <= rectangleSize[coord] / 2.0);
        }
        return in;
	*/
	// Return outside if near negative boundaries, inside if near positive boundaries

	bool in = true;
	for (int coord=0; coord<3; coord++)
	{
		double smallFraction = 1e-14;

		if ((rectangleSize[coord]!=0) &&
			(fabs
				((fabs(X[coord]-clusterCenter[coord])
				  - 0.5 * rectangleSize[coord]) / rectangleSize[coord])
				< smallFraction))
		{
			// Near boundary
			if (X[coord] - clusterCenter[coord] < 0)
			{
				in = false;
			}
		}
		else
		{
			in
				= in
				&& (fabs(X[coord]-clusterCenter[coord])
					<= 0.5 * rectangleSize[coord]      );
		}
	}
	return in;
}
