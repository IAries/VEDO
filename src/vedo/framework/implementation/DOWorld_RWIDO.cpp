#include <vedo/framework/interfaces/DOWorld.h>
#include <vedo/framework/interfaces/IactRecordTab.h>

#include <stdint.h>

namespace vedo
{

bool DOWorld::ReadIDO(const char* filename, IactRecordTab* irtp)
{
	DOWorld::Clear();

	std::ifstream idof;
	unsigned long lnum_doml;
	unsigned long lnum_iactml;
	unsigned long lnum_dos;

	register unsigned long i;

	idof.open(filename, std::ios::in | std::ios::binary);

	if (!idof.is_open())
	{
		std::cout
			<< "Error!! Code: DOWorld::ReadIDO(const char*)" << std::endl
			<< "        Note: IDOWorld read idofile error" << std::endl;
		return false;
	}

	pSystemParameter = new SystemParameter(idof);

	idof.read((char*) &lnum_doml, sizeof(_VEDO_unsigned_long));
	for (i=0; i<lnum_doml; ++i)
		cDOModel.push_back(new DOModel(idof));

	idof.read((char*) &lnum_iactml, sizeof(_VEDO_unsigned_long));
	for (i=0; i<lnum_iactml; ++i)
		cIactModel.push_back(new IactModel(idof));

	idof.read((char*) &lnum_dos, sizeof(_VEDO_unsigned_long));
	for (i=0; i<lnum_dos; ++i)
		cDOStatus.push_back(new DOStatus(idof));

	*irtp << idof;

	idof.close();

	return DOWorld::Check();
};

bool DOWorld::ReadIDO(const char* filename)
{
	DOWorld::Clear();

	std::ifstream idof;
	unsigned long lnum_doml;
	unsigned long lnum_iactml;
	unsigned long lnum_dos;

	register unsigned long i;

	idof.open(filename, std::ios::in | std::ios::binary);

	if (!idof.is_open())
	{
		std::cout
			<< "Error!! Code: DOWorld::ReadIDO(const char*)" << std::endl
			<< "        Note: IDOWorld read idofile error" << std::endl;
		return false;
	}

	pSystemParameter = new SystemParameter(idof);

	idof.read((char*) &lnum_doml, sizeof(_VEDO_unsigned_long));
	for (i=0; i<lnum_doml; ++i)
		cDOModel.push_back(new DOModel(idof));

	idof.read((char*) &lnum_iactml, sizeof(_VEDO_unsigned_long));
	for (i=0; i<lnum_iactml; ++i)
		cIactModel.push_back(new IactModel(idof));

	idof.read((char*) &lnum_dos, sizeof(_VEDO_unsigned_long));
	for (i=0; i<lnum_dos; ++i)
		cDOStatus.push_back(new DOStatus(idof));

	idof.close();

	return DOWorld::Check();
};

void DOWorld::WriteIDO(const char* filename) const
{
	std::ofstream idof;
	std::list<DOModel *>::const_iterator idoml;
	std::list<IactModel *>::const_iterator iactml;
	std::vector<DOStatus *>::const_iterator idos;

	idof.open(filename, std::ios::out | std::ios::binary);

	if (idof.bad())
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteIDO(const char*)" << std::endl
			<< "        Note: DOWorld cannot write ido file" << std::endl;
		exit(-1);
	}

	*pSystemParameter >> idof;

	unsigned long lnum_doml = (unsigned long) cDOModel.size();
	idof.write((char*) &lnum_doml, sizeof(_VEDO_unsigned_long));
	for(idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
		**idoml >> idof;

	unsigned long lnum_iactml = (unsigned long) cIactModel.size();
	idof.write((char*) &lnum_iactml, sizeof(_VEDO_unsigned_long));
	for(iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
		**iactml >> idof;

	unsigned long lnum_dos = (unsigned long) cDOStatus.size();
	idof.write((char*) &lnum_dos, sizeof(_VEDO_unsigned_long));
	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
		**idos >> idof;

    unsigned long lnum_is = 0;
	idof.write((char*) &lnum_is, sizeof(_VEDO_unsigned_long));

	idof.close();
};

void DOWorld::WriteIDO(const char* filename, const IactRecordTab* irtp) const
{
	std::ofstream idof;
	std::list<DOModel *>::const_iterator idoml;
	std::list<IactModel *>::const_iterator iactml;
	std::vector<DOStatus *>::const_iterator idos;

	idof.open(filename, std::ios::out | std::ios::binary);

	if (idof.bad())
	{
		std::cerr
			<< "Error!! Code: DOWorld::WriteIDO(const char*)" << std::endl
			<< "        Note: DOWorld cannot write ido file" << std::endl;
		exit(-1);
	}

	*pSystemParameter >> idof;

	unsigned long lnum_doml = (unsigned long) cDOModel.size();
	idof.write((char*) &lnum_doml, sizeof(_VEDO_unsigned_long));
	for(idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
		**idoml >> idof;

	unsigned long lnum_iactml = (unsigned long) cIactModel.size();
	idof.write((char*) &lnum_iactml, sizeof(_VEDO_unsigned_long));
	for(iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
		**iactml >> idof;

	unsigned long lnum_dos = (unsigned long) cDOStatus.size();
	idof.write((char*) &lnum_dos, sizeof(_VEDO_unsigned_long));
	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
		**idos >> idof;

    if(irtp)
    {
        *irtp >> idof;
    }
    else
    {
        std::cerr
            << "Error!! Code: DOWorld::WriteIDO(const char*, const IactRecordTab*) const" << std::endl
            << "        Note: IactRecordTab is null" << std::endl;
        exit(-1);
    }

	idof.close();
};

};   // namespace vedo
