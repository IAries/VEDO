#ifndef _ISwBtSDBF_H
#define _ISwBtSDBF_H

#include <FrameWork/Interfaces/ImpactSolver.h>
// Interaction model with a "transient" shear spring

namespace VEDO
{

class ISwBtSDBF : public ImpactSolver
{

public:

	ISwBtSDBF
		(const DiscreteObject* cpdoSlave,
		const DiscreteObject* cpdoMaster,
		const IactModel* cpiactml        );

	NJR::Vector3d NextStep
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
		return "ISwBtSDBF";
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

};   // namespace VEDO

#endif // _ISwBSDBF_H
