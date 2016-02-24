#ifndef _ISwBSDB2F_H
#define _ISwBSDB2F_H

#include <vedo/framework/ImpactSolver.h>

namespace vedo
{

class ISwBSDB2F : public ImpactSolver
{

public:

	ISwBSDB2F(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml);

	Vector3df NextStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster, _float_t dt);

	virtual bool InitialStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster);

	virtual std::string type () const
	{
		return "ISwBSDB2F";
	}

private:

    bool bLastContact;

    bool bLastBond;

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

#endif // _ISwBSDB2F_H
