#ifndef RECTANGULARCLUSTERINITIALIZER_H
#define RECTANGULARCLUSTERINITIALIZER_H

#include <Knight/Interfaces/ClusterInitializer.h>
#include <cmath>

class RectangularClusterInitializer : public ClusterInitializer
{
public:

	RectangularClusterInitializer
		(double rectangleSize[3], BravaisLatticeWithBasis *bravais);

	virtual ~RectangularClusterInitializer();

	void SetRectangleSize(const double rectangleSize[3]);

/*
	inline double (GetRectangleSize() const)[3]
	{
		return (double[3]) rectangleSize;
	}
*/

	inline const double *GetRectangleSize() const
	{
		return (const double *)rectangleSize;
	}

protected:

	double rectangleSize[3];
	int SetMaxSize();
	bool Inside(double X[3]);
};

#endif // !defined(RECTANGULARCLUSTERINITIALIZER_H)
