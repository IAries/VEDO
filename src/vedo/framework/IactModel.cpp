#include <aries/utility/BinaryIO.h>
#include <aries/utility/Constants.h>
#include <vedo/framework/IactModel.h>

#include <stdint.h>

namespace vedo
{

IactModel::IactModel(const std::string& mg, const std::string& sg)
{
	sMasterDOGroup = mg;
	sSlaveDOGroup  = sg;
}

bool IactModel::operator () (const IactModel* piactml) const
{
	if (   (sMasterDOGroup == piactml->sMasterDOGroup)
		&& (sSlaveDOGroup  == piactml->sSlaveDOGroup ) )
	{
		return true;
	}

	if(    (sMasterDOGroup == piactml->sSlaveDOGroup )
		&& (sSlaveDOGroup  == piactml->sMasterDOGroup) )
	{
		return true;
	}
	return false;
}

/*
IactModel::IactModel
	(const std::string& MasterDOGroup,
	 const std::string& SlaveDOGroup,
	 const std::string& EquationType,
	 const IactMechanismAttributes& imAttributes): cIactExtend(0)
{
	sMasterDOGroup = MasterDOGroup;
	sSlaveDOGroup  = SlaveDOGroup;
    sEquationType  = EquationType;
	iactAttributes = imAttributes;
	sExtend        = "NoExtend";
}
*/

IactModel::IactModel
	(const std::string& MasterDOGroup,
	 const std::string& SlaveDOGroup,
	 const std::string& EquationType,
	 const std::vector<IactMechanism>& svIMs)
{
	sMasterDOGroup   = MasterDOGroup;
	sSlaveDOGroup    = SlaveDOGroup;
    sEquationType    = EquationType;
	svIactMechanisms = svIMs;
}

IactModel::IactModel(const IactModel& iactml)
{
	*this = iactml;
}

IactModel::IactModel(std::ifstream& idof)
{
	*this << idof;
}

_float_t IactModel::GetIactMechanism(std::string Name) const
{
	for (_uint_t i=0; i<svIactMechanisms.size(); i++)
	{
		if (svIactMechanisms[i].Name == Name)
		{
			return svIactMechanisms[i].Value;
		}
	}

	std::cout
		<< "Interaction between"
		<< sMasterDOGroup.c_str()
		<< " and "
		<< sSlaveDOGroup.c_str()
		<< " has no such interaction mechanism -- "
		<< Name.c_str()
		<< std::endl;

	return 0.0;
}


const IactModel& IactModel::operator = (const IactModel& iactml)
{
	sMasterDOGroup   = iactml.sMasterDOGroup;
	sSlaveDOGroup    = iactml.sSlaveDOGroup;
    sEquationType    = iactml.sEquationType;
	svIactMechanisms = iactml.svIactMechanisms;
	return *this;
}

std::ofstream& IactModel::operator >> (std::ofstream& idof) const
{
	aries::WriteString(sMasterDOGroup, idof);
	aries::WriteString(sSlaveDOGroup,  idof);
	aries::WriteString(sEquationType,  idof);

	_uint_t ims = (_uint_t) svIactMechanisms.size();
	idof.write((const char*) &ims, sizeof(_uint_t));
	IactMechanism im;
	for (_uint_t i=0; i<ims; i++)
	{
		im = svIactMechanisms[i];
		aries::WriteString(im.Name, idof);
		idof.write((const char*) &(im.Value), sizeof(_float_t));
	}
	return idof;
}

std::ifstream& IactModel::operator << (std::ifstream& idof)
{
	aries::ReadString(sMasterDOGroup, idof);
	aries::ReadString(sSlaveDOGroup,  idof);
	aries::ReadString(sEquationType,  idof);

    _uint_t ims;
	idof.read((char*) &ims, sizeof (_uint_t));
	IactMechanism im;
	for (_uint_t i=0; i<ims; i++)
	{
		aries::ReadString(im.Name, idof);
		idof.read((char*) &(im.Value), sizeof(_float_t));
		svIactMechanisms.push_back(im);
	}

	return idof;
}

}   // namespace vedo
