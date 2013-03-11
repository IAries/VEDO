#ifndef _DO_FIXEDSPHERE_H
#define _DO_FIXEDSPHERE_H

#include <FrameWork/Interfaces/DiscreteObject.h>

class DOFixedSphere : public DiscreteObject
{

public:

	DOFixedSphere(const DOStatus* pdos, const DOModel* pdoml);

	inline void Response(double dt)
	{
		ClearImpact();
	}

	double CrossAreaToSurface(double& a, double& b, double& c, double& d) const;

	inline double GetSudoContactRadius()
	{
		return cpDOModel->GetShapeAttributes().sphere.radius;
	};

private:

	DOFixedSphere& operator = (const DOFixedSphere&);

	DOFixedSphere(const DOFixedSphere&);

	DOFixedSphere();
};

#endif // _DO_FIXEDSPHERE_H
