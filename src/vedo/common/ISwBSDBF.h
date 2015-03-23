#ifndef _ISwBSDBF_H
#define _ISwBSDBF_H

#include <vedo/framework/ImpactSolver.h>

namespace vedo
{

class ISwBSDBF : public ImpactSolver
{

public:

	ISwBSDBF(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml);

	aries::Vector3df NextStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster, _float_t dt);

	virtual bool InitialStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster);

	virtual std::string type () const
	{
		return "ISwBSDBF";
	}

private:

	// Spring of Mechanism
	_float_t kn, ks;

	// Dashpot of Mechanism
	_float_t cn, cs;

	// Binding force
	_float_t bn, bs;

	// Friction coefficient
	_float_t fc;
};

}   // namespace vedo

#endif // _ISwBSDBF_H
