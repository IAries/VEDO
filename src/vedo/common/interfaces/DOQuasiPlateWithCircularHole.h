#ifndef _DO_QUASIPLATEWITHCIRCULARHOLE_H
#define _DO_QUASIPLATEWITHCIRCULARHOLE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOQuasiPlateWithCircularHole : public DiscreteObject
{

public:

	DOQuasiPlateWithCircularHole(const DOStatus* pdos, const DOModel* pdoml);

	void Response(double dt);

	inline double CrossAreaToSurface
		(double& a, double& b, double& c, double& d) const
	{
		return 0.0;
	}

	inline double GetSudoContactRadius()
	{
//		return cpDOModel->GetShapeAttributes().quasiplatewithcircularhole.height;
		return 0.0;
	};

private:

	DOQuasiPlateWithCircularHole& operator = (const DOQuasiPlateWithCircularHole&);

	DOQuasiPlateWithCircularHole(const DOQuasiPlateWithCircularHole&);

	DOQuasiPlateWithCircularHole();
};

};   // namespace vedo

#endif // _DO_QUASIPLATEWITHCIRCULARHOLE_H
