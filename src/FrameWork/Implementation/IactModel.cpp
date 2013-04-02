#include <NJR/Interfaces/Utility.h>
#include <Framework/Interfaces/IactModel.h>

namespace vedo
{

IactModel::IactModel(const std::string& mg, const std::string& sg)
{
	sMasterDOGroup = mg;
	sSlaveDOGroup  = sg;
};

bool IactModel::operator () (const IactModel* piactml) const
{
	if((sMasterDOGroup == piactml->sMasterDOGroup) &&
		(sSlaveDOGroup == piactml->sSlaveDOGroup)    )
	{
		return true;
	}

	if((sMasterDOGroup == piactml->sSlaveDOGroup) &&
		(sSlaveDOGroup == piactml->sMasterDOGroup)  )
	{
		return true;
	}
	return false;
};

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
};
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
};

IactModel::IactModel(const IactModel& iactml)
{
	*this = iactml;
};

IactModel::IactModel(std::ifstream& idof)
{
	*this << idof;
};

double IactModel::GetIactMechanism(std::string Name) const
{
	for (unsigned int i=0; i<svIactMechanisms.size(); i++)
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
};


const IactModel& IactModel::operator = (const IactModel& iactml)
{
	sMasterDOGroup   = iactml.sMasterDOGroup;
	sSlaveDOGroup    = iactml.sSlaveDOGroup;
    sEquationType    = iactml.sEquationType;
	svIactMechanisms = iactml.svIactMechanisms;
	return *this;
};

std::ofstream& IactModel::operator >> (std::ofstream& idof) const
{
	njr::WriteString(sMasterDOGroup, idof);
	njr::WriteString(sSlaveDOGroup,  idof);
	njr::WriteString(sEquationType,  idof);

	unsigned int ims = (unsigned int) svIactMechanisms.size();
	idof.write((const char*) &ims, sizeof(int));
	IactMechanism im;
	for (unsigned int i=0; i<ims; i++)
	{
		im = svIactMechanisms[i];
		njr::WriteString(im.Name, idof);
		idof.write((const char*) &(im.Value), sizeof(double));
	}
	return idof;
};

std::ifstream& IactModel::operator << (std::ifstream& idof)
{
	njr::ReadString(sMasterDOGroup, idof);
	njr::ReadString(sSlaveDOGroup,  idof);
	njr::ReadString(sEquationType,  idof);

    unsigned int ims;
	idof.read((char*) &ims, sizeof (int));
	IactMechanism im;
	for (unsigned int i=0; i<ims; i++)
	{
		njr::ReadString(im.Name, idof);
		idof.read((char*) &(im.Value), sizeof(double));
		svIactMechanisms.push_back(im);
	}

	return idof;
};

IactModel::IactModel(std::ifstream& idof, unsigned int _version)
{
	njr::ReadString(sMasterDOGroup, idof);
	njr::ReadString(sSlaveDOGroup,  idof);
	njr::ReadString(sEquationType,  idof);

	IactMechanism im;
    IactMechanism2006 im2006;
    IactMechanism2007 im2007;
    int imExtendSize;
    switch (_version)
	{
		case 2006:
        	idof.read((char*) &im2006, sizeof(IactMechanism2006));
            im.Name  = "spring";
            im.Value = im2006.spring;
       		svIactMechanisms.push_back(im);
            im.Name  = "dashpot";
            im.Value = im2006.dashpot;
       		svIactMechanisms.push_back(im);
            im.Name  = "binding";
            im.Value = im2006.binding;
       		svIactMechanisms.push_back(im);
            im.Name  = "friction";
            im.Value = im2006.friction;
       		svIactMechanisms.push_back(im);
            // Start to read the extend information
            njr::ReadString(im.Name, idof);
        	idof.read((char*) &imExtendSize, sizeof (int));
        	for (int i=0; i<imExtendSize; i++)
	        {
                njr::ReadString(im.Name, idof);
                idof.read((char*) &(im.Value), sizeof(double));
           		svIactMechanisms.push_back(im);
            }
            break;
		case 2007:
        	idof.read((char*) &im2007, sizeof(IactMechanism2007));
            im.Name  = "NormalSpring";
            im.Value = im2007.NormalSpring;
       		svIactMechanisms.push_back(im);
            im.Name  = "TangentSpring";
            im.Value = im2007.TangentSpring;
       		svIactMechanisms.push_back(im);
            im.Name  = "NormalDashpot";
            im.Value = im2007.NormalDashpot;
       		svIactMechanisms.push_back(im);
            im.Name  = "TangentDashpot";
            im.Value = im2007.TangentDashpot;
       		svIactMechanisms.push_back(im);
            im.Name  = "NormalBond";
            im.Value = im2007.NormalBond;
       		svIactMechanisms.push_back(im);
            im.Name  = "TangentBond";
            im.Value = im2007.TangentBond;
       		svIactMechanisms.push_back(im);
            im.Name  = "StaticFriction";
            im.Value = im2007.StaticFriction;
       		svIactMechanisms.push_back(im);
            im.Name  = "KineticFriction";
            im.Value = im2007.KineticFriction;
       		svIactMechanisms.push_back(im);
            njr::ReadString(im.Name, idof);
            // Start to read the extend information
        	idof.read((char*) &imExtendSize, sizeof (int));
        	for (int i=0; i<imExtendSize; i++)
	        {
                njr::ReadString(im.Name, idof);
                idof.read((char*) &(im.Value), sizeof(double));
           		svIactMechanisms.push_back(im);
            }
            break;
		default:
      		svIactMechanisms.push_back(im);
	}
};

};   // namespace vedo
