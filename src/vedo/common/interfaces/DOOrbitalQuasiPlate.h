#ifndef _DO_ORBITAL_QUASIPLATE_H
#define _DO_ORBITAL_QUASIPLATE_H

#include <vedo/framework/interfaces/DiscreteObject.h>

namespace vedo
{

class DOOrbitalQuasiPlate : public DiscreteObject
{

public:

	DOOrbitalQuasiPlate(const DOStatus* pdos, const DOModel* pdoml);

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

	DOOrbitalQuasiPlate& operator = (const DOOrbitalQuasiPlate&);

	DOOrbitalQuasiPlate(const DOOrbitalQuasiPlate&);

	DOOrbitalQuasiPlate();
};

};   // namespace vedo

#endif // _DO_ORBITAL_QUASIPLATE_H
