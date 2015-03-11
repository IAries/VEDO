#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/IactRecordTab.h>

#include <stdint.h>

namespace vedo
{

bool DOWorld::ReadIDO(std::string filename, IactRecordTab* irtp)
{
	DOWorld::Clear();

	std::ifstream idof;
	vedo::vedo_uint_t lnum_doml;
	vedo::vedo_uint_t lnum_iactml;
	vedo::vedo_uint_t lnum_dos;

	register vedo::vedo_uint_t i;

	idof.open(filename.c_str(), std::ios::in | std::ios::binary);

	if (!idof.is_open())
	{
		std::cout
			<< "Error!! Code: DOWorld::ReadIDO(const char*)" << std::endl
			<< "        Note: IDOWorld read idofile error" << std::endl;
		return false;
	}

	pSystemParameter = new SystemParameter(idof);

	idof.read((char*) &lnum_doml, sizeof(vedo_uint_t));
	for (i=0; i<lnum_doml; ++i)
	{
		cDOModel.push_back(new DOModel(idof));
	}

	idof.read((char*) &lnum_iactml, sizeof(vedo_uint_t));
	for (i=0; i<lnum_iactml; ++i)
	{
		cIactModel.push_back(new IactModel(idof));
	}

	idof.read((char*) &lnum_dos, sizeof(vedo_uint_t));
	for (i=0; i<lnum_dos; ++i)
	{
		cDOStatus.push_back(new DOStatus(idof));
	}

	*irtp << idof;

	idof.close();

	return DOWorld::Check();
}

bool DOWorld::ReadIDO(std::string filename)
{
	DOWorld::Clear();

	std::ifstream idof;
	vedo::vedo_uint_t lnum_doml;
	vedo::vedo_uint_t lnum_iactml;
	vedo::vedo_uint_t lnum_dos;

	register vedo::vedo_uint_t i;

	idof.open(filename.c_str(), std::ios::in | std::ios::binary);

	if (!idof.is_open())
	{
		std::cout
			<< "Error!! Code: DOWorld::ReadIDO(const char*)" << std::endl
			<< "        Note: IDOWorld read idofile error"   << std::endl;
		return false;
	}

	pSystemParameter = new SystemParameter(idof);

	idof.read((char*) &lnum_doml, sizeof(vedo_uint_t));
	for (i=0; i<lnum_doml; ++i)
	{
		cDOModel.push_back(new DOModel(idof));
	}

	idof.read((char*) &lnum_iactml, sizeof(vedo_uint_t));
	for (i=0; i<lnum_iactml; ++i)
	{
		cIactModel.push_back(new IactModel(idof));
	}

	idof.read((char*) &lnum_dos, sizeof(vedo_uint_t));
	for (i=0; i<lnum_dos; ++i)
	{
		cDOStatus.push_back(new DOStatus(idof));
	}

	idof.close();

	return DOWorld::Check();
}

void DOWorld::WriteIDO(const std::string filename) const
{
	std::ofstream idof;
	std::list<DOModel *>::const_iterator idoml;
	std::list<IactModel *>::const_iterator iactml;
	std::vector<DOStatus *>::const_iterator idos;

	idof.open(filename.c_str(), std::ios::out | std::ios::binary);

	if (idof.bad())
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteIDO(const char*)" << std::endl
			<< "        Note: DOWorld cannot write ido file"  << std::endl;
		exit(-1);
	}

	*pSystemParameter >> idof;

	vedo::vedo_uint_t lnum_doml = (vedo::vedo_uint_t) cDOModel.size();
	idof.write((char*) &lnum_doml, sizeof(vedo_uint_t));
	for (idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
	{
		**idoml >> idof;
	}

	vedo::vedo_uint_t lnum_iactml = (vedo::vedo_uint_t) cIactModel.size();
	idof.write((char*) &lnum_iactml, sizeof(vedo_uint_t));
	for (iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
	{
		**iactml >> idof;
	}

	vedo::vedo_uint_t lnum_dos = (vedo::vedo_uint_t) cDOStatus.size();
	idof.write((char*) &lnum_dos, sizeof(vedo_uint_t));
	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		**idos >> idof;
	}

    vedo::vedo_uint_t lnum_is = 0;
	idof.write((char*) &lnum_is, sizeof(vedo_uint_t));

	idof.close();
}

void DOWorld::WriteIDO(const std::string filename, const IactRecordTab* irtp) const
{
	std::ofstream idof;
	std::list<DOModel *>::const_iterator idoml;
	std::list<IactModel *>::const_iterator iactml;
	std::vector<DOStatus *>::const_iterator idos;

	idof.open(filename.c_str(), std::ios::out | std::ios::binary);

	if (idof.bad())
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteIDO(const char*)" << std::endl
			<< "        Note: DOWorld cannot write ido file" << std::endl;
		exit(-1);
	}

	*pSystemParameter >> idof;

	vedo::vedo_uint_t lnum_doml = (vedo::vedo_uint_t) cDOModel.size();
	idof.write((char*) &lnum_doml, sizeof(vedo_uint_t));
	for(idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
	{
		**idoml >> idof;
	}

	vedo::vedo_uint_t lnum_iactml = (vedo::vedo_uint_t) cIactModel.size();
	idof.write((char*) &lnum_iactml, sizeof(vedo_uint_t));
	for(iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
	{
		**iactml >> idof;
	}

	vedo::vedo_uint_t lnum_dos = (vedo::vedo_uint_t) cDOStatus.size();
	idof.write((char*) &lnum_dos, sizeof(vedo_uint_t));
	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		**idos >> idof;
	}

    if (irtp)
    {
        *irtp >> idof;
    }
    else
    {
        std::cerr
            << "Error!! Code: DOWorld::WriteIDO(const char*, const IactRecordTab*) const" << std::endl
            << "        Note: IactRecordTab is null"                                      << std::endl;
        exit(-1);
    }

	idof.close();
}

}   // namespace vedo
