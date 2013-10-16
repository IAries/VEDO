#ifndef _DO_FIXEDQUASIPLATEWITHCIRCULARHOLE_H
#define _DO_FIXEDQUASIPLATEWITHCIRCULARHOLE_H

#include <Framework/Interfaces/DiscreteObject.h>

namespace vedo
{

class DOFixedQuasiPlateWithCircularHole : public DiscreteObject
{

public:

	DOFixedQuasiPlateWithCircularHole(const DOStatus* pdos, const DOModel* pdoml);

	inline void Response(double dt)
	{
		ClearImpact();
	}

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

	DOFixedQuasiPlateWithCircularHole& operator = (const DOFixedQuasiPlateWithCircularHole&);

	DOFixedQuasiPlateWithCircularHole(const DOFixedQuasiPlateWithCircularHole&);

	DOFixedQuasiPlateWithCircularHole();
};

};   // namespace vedo

#endif // _DO_FIXEDQUASIPLATEWITHCIRCULARHOLE_H
