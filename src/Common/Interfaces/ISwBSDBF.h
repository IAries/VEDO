#ifndef _ISwBSDBF_H
#define _ISwBSDBF_H

#include <FrameWork/Interfaces/ImpactSolver.h>

class ISwBSDBF : public ImpactSolver
{

public:

	ISwBSDBF
		(const DiscreteObject* cpdoSlave,
		const DiscreteObject* cpdoMaster,
		const IactModel* cpiactml        );

	NJRvector3d NextStep
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

#endif // _ISwBSDBF_H
