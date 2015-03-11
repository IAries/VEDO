#ifndef _IACT_MODEL_H
#define _IACT_MODEL_H

#include <vedo/njr/interfaces/Vector3d.h>
#include <fstream>
#include <string>
#include <vector>

namespace vedo
{

// Data type class for Old .ido file
struct IactMechanism2007
{
	vedo_float_t NormalSpring;
    vedo_float_t TangentSpring;
    vedo_float_t NormalDashpot;
    vedo_float_t TangentDashpot;
    vedo_float_t NormalBond;
    vedo_float_t TangentBond;
    vedo_float_t StaticFriction;
    vedo_float_t KineticFriction;

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
	}

	explicit IactMechanism2007
		(vedo_float_t ns, vedo_float_t ts, vedo_float_t nd, vedo_float_t td,
		 vedo_float_t nb, vedo_float_t tb, vedo_float_t sf, vedo_float_t kf )
	{
		NormalSpring    = ns;
		TangentSpring   = ts;
		NormalDashpot   = nd;
		TangentDashpot  = td;
		NormalBond      = nb;
		TangentBond     = tb;
		StaticFriction  = sf;
		KineticFriction = kf;
	}
};



struct IactMechanism2006
{
	vedo_float_t spring, dashpot, binding, friction;

	IactMechanism2006()
	{
		spring   = 0.0;
		dashpot  = 0.0;
		binding  = 0.0;
		friction = 0.0;
	}

	explicit IactMechanism2006(vedo_float_t s, vedo_float_t d, vedo_float_t b, vedo_float_t f)
	{
		spring   = s;
		binding  = b;
		dashpot  = d;
		friction = f;
	}
};



// Data type class
struct IactMechanism
{
	std::string  Name;
	vedo_float_t Value;

	IactMechanism()
	{
		Name  = "NoName";
		Value = 0.0;
	}

	explicit IactMechanism(std::string n, vedo_float_t v)
	{
		Name  = n;
		Value = v;
	}
};



class IactModel
{

public:

	// Function for algorithm std::find_if
	explicit IactModel(const std::string& mg, const std::string& sg);

	IactModel
		(const std::string& sMasterDOGroup, const std::string& sSlaveDOGroup,
		 const std::string& sEquationType, const std::vector<IactMechanism>& svIMs);

	IactModel(const IactModel& iactml);

	IactModel(std::ifstream& idof);

	inline std::string GetSlaveDOGroup() const
	{
		return sSlaveDOGroup;
	}

	inline std::string GetMasterDOGroup() const
	{
		return sMasterDOGroup;
	}

	inline std::string GetEquationType() const
	{
		return sEquationType;
	}

	inline const std::vector<IactMechanism>& GetIactMechanisms() const
	{
		return svIactMechanisms;
	}

	vedo_float_t GetIactMechanism(std::string Name) const;

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

}   // namespace vedo

#endif // _IACT_MODEL_H
