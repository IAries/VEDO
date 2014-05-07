#ifndef _ISwHertz_H
#define _ISwHertz_H

#include <vedo/framework/interfaces/ImpactSolver.h>
// Hybrid Hertz/linear contact model

namespace vedo
{

class ISwHertz : public ImpactSolver
{

public:

	ISwHertz
		(const DiscreteObject* cpdoSlave,
		const DiscreteObject* cpdoMaster,
		const IactModel* cpiactml        );

	njr::Vector3d NextStep
		(const ContactDetector* pcd,
		DiscreteObject* pdoSlave,
		DiscreteObject* pdoMaster,
		double dt                   );

	virtual bool InitialStep
		(const ContactDetector* pcd,
		DiscreteObject* pdoSlave,
		DiscreteObject* pdoMaster   );

	virtual std::string type () const
	{
		return "ISwHertz";
	};

private:

    // stiffness of normal spring (4/3 * (Reff)^0.5 / Eff)
    double kn;

	// Damping coefficient
	double cn;
};

};   // namespace vedo


#endif // _ISwHertz_H
