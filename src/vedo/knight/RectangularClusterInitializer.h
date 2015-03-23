#ifndef RECTANGULARCLUSTERINITIALIZER_H
#define RECTANGULARCLUSTERINITIALIZER_H

#include <vedo/knight/ClusterInitializer.h>
#include <aries/utility/Constants.h>
#include <cmath>

class RectangularClusterInitializer : public ClusterInitializer
{
public:

	RectangularClusterInitializer
		(vedo::_float_t rectangleSize[3], BravaisLatticeWithBasis *bravais);

	virtual ~RectangularClusterInitializer();

	void SetRectangleSize(const vedo::_float_t rectangleSize[3]);

/*
	inline vedo::_float_t (GetRectangleSize() const)[3]
	{
		return (vedo::_float_t[3]) rectangleSize;
	}
*/

	inline const vedo::_float_t *GetRectangleSize() const
	{
		return (const vedo::_float_t *)rectangleSize;
	}

protected:

	vedo::_float_t rectangleSize[3];
	vedo::_uint_t SetMaxSize();
	bool Inside(vedo::_float_t X[3]);
};

#endif // !defined(RECTANGULARCLUSTERINITIALIZER_H)
