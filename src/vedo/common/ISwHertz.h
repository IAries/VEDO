#ifndef _ISwHertz_H
#define _ISwHertz_H

#include <vedo/framework/ImpactSolver.h>
// Hybrid Hertz/linear contact model

namespace vedo
{

class ISwHertz : public ImpactSolver
{

public:

	ISwHertz(const DiscreteObject* cpdoSlave, const DiscreteObject* cpdoMaster, const IactModel* cpiactml);

	aries::Vector3df NextStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster, _float_t dt);

	virtual bool InitialStep(const ContactDetector* pcd, DiscreteObject* pdoSlave, DiscreteObject* pdoMaster);

	virtual std::string type () const
	{
		return "ISwHertz";
	}

private:

    // stiffness of normal spring (4/3 * (Reff)^0.5 / Eff)
    _float_t kn;

	// Damping coefficient
	_float_t cn;
};

}   // namespace vedo


#endif // _ISwHertz_H
