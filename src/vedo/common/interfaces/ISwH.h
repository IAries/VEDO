#ifndef _ISwH_H
#define _ISwH_H

#include <vedo/framework/interfaces/ImpactSolver.h>
// Hybrid Hertz/linear contact model

namespace vedo
{

class ISwH : public ImpactSolver
{

public:

	ISwH(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml);

	njr::Vector3d NextStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster, vedo_float_t dt);

	virtual bool InitialStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster);

	virtual std::string type () const
	{
		return "ISwH";
	}

private:

    // (Hertz) stiffness of normal spring (4/3 * (Reff)^0.5 / Eff)
    vedo_float_t knHertz;

    // (Linear) stiffness of normal spring
    vedo_float_t kn;

	// Equivalent mass (ma * mb / (ma + mb))
    vedo_float_t Meff;

	// Coefficient 1 of shear stiffness
    vedo_float_t ksc1;

    // Coefficient 2 of shear stiffness
    vedo_float_t ksc2;

    // Damping ratio
	vedo_float_t betan;

	// (Kinetic) Friction coefficient
	vedo_float_t fk;

	// Critical time step
	//vedo_float_t dCriticalTimeStep;
};

}   // namespace vedo


#endif // _ISwH_H
