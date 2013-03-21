#ifndef _DO_CONSTRAINEDQUASIPLATE_H
#define _DO_CONSTRAINEDQUASIPLATE_H

#include <FrameWork/Interfaces/DiscreteObject.h>

namespace VEDO
{

class DOConstrainedQuasiPlate : public DiscreteObject
{

public:

	DOConstrainedQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

	void Response(double dt);

	inline double CrossAreaToSurface
		(double& a, double& b, double& c, double& d) const
	{
		return 0.0;
	};

	inline double GetSudoContactRadius()
	{
//		return cpDOModel->GetShapeAttributes().quasiplate.height;
		return 0.0;
	};

private:

	DOConstrainedQuasiPlate& operator = (const DOConstrainedQuasiPlate&);

	DOConstrainedQuasiPlate(const DOConstrainedQuasiPlate&);

	DOConstrainedQuasiPlate();
};

};   // namespace VEDO

#endif // _DO_CONSTRAINEDQUASIPLATE_H
