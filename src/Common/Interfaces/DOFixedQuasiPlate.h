#ifndef _DO_FIXEDQUASIPLATE_H
#define _DO_FIXEDQUASIPLATE_H

#include <FrameWork/Interfaces/DiscreteObject.h>

class DOFixedQuasiPlate : public DiscreteObject
{

public:

	DOFixedQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

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
//		return cpDOModel->GetShapeAttributes().quasiplate.height;
		return 0.0;
	};

private:

	DOFixedQuasiPlate& operator = (const DOFixedQuasiPlate&);

	DOFixedQuasiPlate(const DOFixedQuasiPlate&);

	DOFixedQuasiPlate();
};

#endif // _DO_FIXEDQUASIPLATE_H
