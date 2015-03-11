#ifndef _ISwBtSDBF_H
#define _ISwBtSDBF_H

#include <vedo/framework/interfaces/ImpactSolver.h>
// Interaction model with a "transient" shear spring

namespace vedo
{

class ISwBtSDBF : public ImpactSolver
{

public:

	ISwBtSDBF(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml);

	njr::Vector3d NextStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster, vedo_float_t dt);

	virtual bool InitialStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster);

	virtual std::string type () const
	{
		return "ISwBtSDBF";
	}

private:

	// Spring of Mechanism
	vedo_float_t kn, ks;

	// Dashpot of Mechanism
	vedo_float_t cn, cs;

	// Binding force
	vedo_float_t bn, bs;

	// Friction coefficient
	vedo_float_t fc;
};

}   // namespace vedo

#endif // _ISwBSDBF_H
