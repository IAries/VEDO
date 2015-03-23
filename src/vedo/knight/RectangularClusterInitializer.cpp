// RectangularClusterInitializer.cpp: implementation of the RectangularClusterInitializer class.
//
//////////////////////////////////////////////////////////////////////

#include <vedo/knight/RectangularClusterInitializer.h>
#include <vedo/knight/BravaisLatticeWithBasis.h>
#include <aries/utility/Constants.h>
#include <string>
#include <cmath>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RectangularClusterInitializer::RectangularClusterInitializer
	(vedo::_float_t rectangleSize[3],
	 BravaisLatticeWithBasis *bravais): ClusterInitializer(bravais)
{
	SetRectangleSize(rectangleSize);
}


RectangularClusterInitializer::~RectangularClusterInitializer()
{
	// std::cout << "RectangularClusterInitializer deleted" << std::endl;
}

void RectangularClusterInitializer::SetRectangleSize
	(const vedo::_float_t rectangleSize[3])
{
	vedo::_uint_t coord;
    for(coord=0; coord<3; coord++)
    {
    	this->rectangleSize[coord] = rectangleSize[coord];
    }
	// XXX JPS added
	SetMaxSize();
}

vedo::_uint_t RectangularClusterInitializer::SetMaxSize()
{
 	vedo::_uint_t i;
	vedo::_float_t volumeOfRectangle=1.;
	vedo::_float_t corner[3], ind[3], maxi[3], mini[3];
	for(vedo::_uint_t index=0; index<(vedo::_uint_t)(pow(2.0, 3.0)); index++)
//	for(vedo::_uint_t index=0; index<8; index++)
	{
		vedo::_uint_t itmp = index;
		for(i=0; i<3; i++)
		{
			corner[i]
				= clusterCenter[i]
				+ rectangleSize[i] / 2.0 * (vedo::_float_t)(itmp % 2 == 0 ? -1:1);
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
		maxSize[i] = (vedo::_uint_t)maxi[i] + 1;
		minSize[i] = (vedo::_uint_t)mini[i] - 1;
	}
	return
		vedo::_uint_t
			(volumeOfRectangle
			 / bravais->GetVolumeOfCell()
			 * bravais->GetNAtomsPerCell());
}


bool RectangularClusterInitializer::Inside(vedo::_float_t X[3])
{
	/* Doesn't check if near boundary!
        bool in=true;
        for(vedo::_uint_t coord=0; coord<3;coord++)
        {
        	in = in && (fabs(X[coord]) <= rectangleSize[coord] / 2.0);
        }
        return in;
	*/
	// Return outside if near negative boundaries, inside if near positive boundaries

	bool in = true;
	for (vedo::_uint_t coord=0; coord<3; coord++)
	{
		vedo::_float_t smallFraction = 1e-14;

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
