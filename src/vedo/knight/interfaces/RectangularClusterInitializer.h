#ifndef RECTANGULARCLUSTERINITIALIZER_H
#define RECTANGULARCLUSTERINITIALIZER_H

#include <vedo/knight/interfaces/ClusterInitializer.h>
#include <vedo/constants/interfaces/Constants.h>
#include <cmath>

class RectangularClusterInitializer : public ClusterInitializer
{
public:

	RectangularClusterInitializer
		(vedo::vedo_float_t rectangleSize[3], BravaisLatticeWithBasis *bravais);

	virtual ~RectangularClusterInitializer();

	void SetRectangleSize(const vedo::vedo_float_t rectangleSize[3]);

/*
	inline vedo::vedo_float_t (GetRectangleSize() const)[3]
	{
		return (vedo::vedo_float_t[3]) rectangleSize;
	}
*/

	inline const vedo::vedo_float_t *GetRectangleSize() const
	{
		return (const vedo::vedo_float_t *)rectangleSize;
	}

protected:

	vedo::vedo_float_t rectangleSize[3];
	vedo::vedo_int_t SetMaxSize();
	bool Inside(vedo::vedo_float_t X[3]);
};

#endif // !defined(RECTANGULARCLUSTERINITIALIZER_H)
