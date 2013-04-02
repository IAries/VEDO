#ifndef _ISwBSDBF_H
#define _ISwBSDBF_H

#include <Framework/Interfaces/ImpactSolver.h>

namespace vedo
{

class ISwBSDBF : public ImpactSolver
{

public:

	ISwBSDBF
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
		return "ISwBSDBF";
	};

private:

	// Spring of Mechanism
	double kn, ks;

	// Dashpot of Mechanism
	double cn, cs;

	// Binding force
	double bn, bs;

	// Friction coefficient
	double fc;
};

};   // namespace vedo

#endif // _ISwBSDBF_H
