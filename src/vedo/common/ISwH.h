#ifndef _ISwH_H
#define _ISwH_H

#include <vedo/framework/ImpactSolver.h>
// Hybrid Hertz/linear contact model

namespace vedo
{

class ISwH : public ImpactSolver
{

public:

	ISwH(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml);

	aries::Vector3df NextStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster, _float_t dt);

	virtual bool InitialStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster);

	virtual std::string type () const
	{
		return "ISwH";
	}

private:

    // (Hertz) stiffness of normal spring (4/3 * (Reff)^0.5 / Eff)
    _float_t knHertz;

    // (Linear) stiffness of normal spring
    _float_t kn;

	// Equivalent mass (ma * mb / (ma + mb))
    _float_t Meff;

	// Coefficient 1 of shear stiffness
    _float_t ksc1;

    // Coefficient 2 of shear stiffness
    _float_t ksc2;

    // Damping ratio
	_float_t betan;

	// (Kinetic) Friction coefficient
	_float_t fk;

	// Critical time step
	//_float_t dCriticalTimeStep;
};

}   // namespace vedo


#endif // _ISwH_H
