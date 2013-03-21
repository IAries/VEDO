#ifndef _DOSPHERE_H
#define _DOSPHERE_H

#include <FrameWork/Interfaces/DiscreteObject.h>

namespace VEDO
{

class DOSphere : public DiscreteObject
{

public:

	DOSphere(const DOStatus* pdos, const DOModel* pdoml);

	void Response(double dt);

	double CrossAreaToSurface(double& a, double& b, double& c, double& d) const;

	inline double GetSudoContactRadius()
	{
		return cpDOModel->GetShapeAttributes().sphere.radius;
	};

private:

	DOSphere& operator = (const DOSphere&);

	DOSphere(const DOSphere&);

	DOSphere();
};

};   // namespace VEDO

#endif // _DOSPHERE_H
