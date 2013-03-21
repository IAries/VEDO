#ifndef _DO_QUASIPLATE_H
#define _DO_QUASIPLATE_H

#include <FrameWork/Interfaces/DiscreteObject.h>

namespace VEDO
{

class DOQuasiPlate : public DiscreteObject
{

public:

	DOQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

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

	DOQuasiPlate& operator = (const DOQuasiPlate&);

	DOQuasiPlate(const DOQuasiPlate&);

	DOQuasiPlate();
};

};   // namespace VEDO

#endif // _DO_QUASIPLATE_H
