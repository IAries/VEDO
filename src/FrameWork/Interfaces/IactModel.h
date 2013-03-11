#ifndef _IACT_MODEL_H
#define _IACT_MODEL_H

#include <NJR/Interfaces/vector3d.h>
#include <fstream>
#include <string>
#include <vector>

// Data type class for Old .ido file
struct IactMechanism2007
{
	double NormalSpring;
    double TangentSpring;
    double NormalDashpot;
    double TangentDashpot;
    double NormalBond;
    double TangentBond;
    double StaticFriction;
    double KineticFriction;

	IactMechanism2007()
	{
		NormalSpring    = 0.0;
		TangentSpring   = 0.0;
		NormalDashpot   = 0.0;
		TangentDashpot  = 0.0;
		NormalBond      = 0.0;
		TangentBond     = 0.0;
		StaticFriction  = 0.0;
		KineticFriction = 0.0;
	};

	explicit IactMechanism2007
		(double ns, double ts, double nd, double td,
		 double nb, double tb, double sf, double kf)
	{
		NormalSpring    = ns;
		TangentSpring   = ts;
		NormalDashpot   = nd;
		TangentDashpot  = td;
		NormalBond      = nb;
		TangentBond     = tb;
		StaticFriction  = sf;
		KineticFriction = kf;
	};
};

struct IactMechanism2006
{
	double spring, dashpot, binding, friction;

	IactMechanism2006()
	{
		spring   = 0.0;
		dashpot  = 0.0;
		binding  = 0.0;
		friction = 0.0;
	};

	explicit IactMechanism2006(double s, double d, double b, double f)
	{
		spring   = s;
		binding  = b;
		dashpot  = d;
		friction = f;
	};
};

// Data type class
struct IactMechanism
{
	std::string Name;
	double      Value;

	IactMechanism()
	{
		Name  = "NoName";
		Value = 0.0;
	};

	explicit IactMechanism(std::string n, double v)
	{
		Name  = n;
		Value = v;
	};
};



class IactModel
{

public:

	// Function for algorithm std::find_if
	explicit IactModel(const std::string& mg, const std::string& sg);

	IactModel
		(const std::string& sMasterDOGroup,
		const std::string& sSlaveDOGroup,
		const std::string& sEquationType,
		const std::vector<IactMechanism>& svIMs);

	IactModel(const IactModel& iactml);

	IactModel(std::ifstream& idof);

	IactModel(std::ifstream& idof, unsigned int version);   // Used to converse old .ido file

	inline std::string GetSlaveDOGroup() const
	{
		return sSlaveDOGroup;
	};

	inline std::string GetMasterDOGroup() const
	{
		return sMasterDOGroup;
	};

	inline std::string GetEquationType() const
	{
		return sEquationType;
	};

	inline const std::vector<IactMechanism>& GetIactMechanisms() const
	{
		return svIactMechanisms;
	};

	double GetIactMechanism(std::string Name) const;

	const IactModel& operator = (const IactModel& iactml);

	bool operator () (const IactModel* pIactModel) const;

	// Binary output & input
	std::ofstream& operator >> (std::ofstream& idof) const;
	std::ifstream& operator << (std::ifstream& idof);

private:

	std::string sMasterDOGroup;
	std::string sSlaveDOGroup;
	std::string sEquationType;
	std::vector<IactMechanism> svIactMechanisms;
};

#endif // _IACT_MODEL_H
