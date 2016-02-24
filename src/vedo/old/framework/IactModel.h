#ifndef _IACT_MODEL_H
#define _IACT_MODEL_H

#include <vedo/framework/DataType.h>
#include <fstream>
#include <string>
#include <vector>

namespace vedo
{

// Data type class for Old .ido file
struct IactMechanism2007
{
	_float_t NormalSpring;
    _float_t TangentSpring;
    _float_t NormalDashpot;
    _float_t TangentDashpot;
    _float_t NormalBond;
    _float_t TangentBond;
    _float_t StaticFriction;
    _float_t KineticFriction;

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
		(_float_t ns, _float_t ts, _float_t nd, _float_t td,
		 _float_t nb, _float_t tb, _float_t sf, _float_t kf )
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
	_float_t spring, dashpot, binding, friction;

	IactMechanism2006()
	{
		spring   = 0.0;
		dashpot  = 0.0;
		binding  = 0.0;
		friction = 0.0;
	}

	explicit IactMechanism2006(_float_t s, _float_t d, _float_t b, _float_t f)
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
	_float_t Value;

	IactMechanism()
	{
		Name  = "NoName";
		Value = 0.0;
	}

	explicit IactMechanism(std::string n, _float_t v)
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

	_float_t GetIactMechanism(std::string Name) const;

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
