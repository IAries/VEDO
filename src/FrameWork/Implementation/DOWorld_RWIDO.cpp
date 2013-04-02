#include <Framework/Interfaces/DOWorld.h>
#include <Framework/Interfaces/IactRecordTab.h>

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

	idof.read( (char*) &lnum_doml  , sizeof(unsigned long) );
	idof.read( (char*) &lnum_iactml, sizeof(unsigned long) );
	idof.read( (char*) &lnum_dos   , sizeof(unsigned long) );

	for (i=0; i<lnum_doml; ++i)
		cDOModel.push_back(new DOModel(idof));

	for (i=0; i<lnum_iactml; ++i)
		cIactModel.push_back(new IactModel(idof));

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

	idof.read( (char*) &lnum_doml  , sizeof(unsigned long) );
	idof.read( (char*) &lnum_iactml, sizeof(unsigned long) );
	idof.read( (char*) &lnum_dos   , sizeof(unsigned long) );

	for (i=0; i<lnum_doml; ++i)
		cDOModel.push_back(new DOModel(idof));

	for (i=0; i<lnum_iactml; ++i)
		cIactModel.push_back(new IactModel(idof));

	for (i=0; i<lnum_dos; ++i)
		cDOStatus.push_back(new DOStatus(idof));

	idof.close();

	return DOWorld::Check();
};

bool DOWorld::ReadIDO(const char* filename, unsigned int version)
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
			<< "Error!! Code: DOWorld::ReadIDO(const char*, unsigned int)" << std::endl
			<< "        Note: IDOWorld read idofile error" << std::endl;
		return false;
	}

    if(version <= 2011)
        pSystemParameter = new SystemParameter(idof, version);
    else
        pSystemParameter = new SystemParameter(idof);

	idof.read((char*) &lnum_doml  , sizeof(unsigned long));
	idof.read((char*) &lnum_iactml, sizeof(unsigned long));
	idof.read((char*) &lnum_dos   , sizeof(unsigned long));

	for (i=0; i<lnum_doml; ++i)
		cDOModel.push_back(new DOModel(idof, version));

	for (i=0; i<lnum_iactml; ++i)
		cIactModel.push_back(new IactModel(idof, version));

	for (i=0; i<lnum_dos; ++i)
		cDOStatus.push_back(new DOStatus(idof));

	idof.close();

	return DOWorld::Check();
};

bool DOWorld::ReadIDO2011(const char* filename)
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
			<< "Error!! Code: DOWorld::ReadIDO2011(const char*)" << std::endl
			<< "        Note: IDOWorld read idofile error" << std::endl;
		return false;
	}

    pSystemParameter = new SystemParameter(idof, 2011);

	idof.read((char*) &lnum_doml  , sizeof(unsigned long));
	idof.read((char*) &lnum_iactml, sizeof(unsigned long));
	idof.read((char*) &lnum_dos   , sizeof(unsigned long));

	for (i=0; i<lnum_doml; ++i)
		cDOModel.push_back(new DOModel(idof, 2011));

	for (i=0; i<lnum_iactml; ++i)
		cIactModel.push_back(new IactModel(idof));

	for (i=0; i<lnum_dos; ++i)
		cDOStatus.push_back(new DOStatus(idof));

	idof.close();

	return DOWorld::Check();
};

bool DOWorld::ReadIDO2009(const char* filename)
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
			<< "Error!! Code: DOWorld::ReadIDO2009(const char*)" << std::endl
			<< "        Note: IDOWorld read idofile error" << std::endl;
		return false;
	}

    pSystemParameter = new SystemParameter(idof, 2009);

	idof.read( (char*) &lnum_doml  , sizeof(unsigned long) );
	idof.read( (char*) &lnum_iactml, sizeof(unsigned long) );
	idof.read( (char*) &lnum_dos   , sizeof(unsigned long) );

	for (i=0; i<lnum_doml; ++i)
		cDOModel.push_back(new DOModel(idof, 2009));

	for (i=0; i<lnum_iactml; ++i)
		cIactModel.push_back(new IactModel(idof));

	for (i=0; i<lnum_dos; ++i)
		cDOStatus.push_back(new DOStatus(idof));

	idof.close();

	return DOWorld::Check();
};

bool DOWorld::ReadIDO2008(const char* filename)
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
			<< "Error!! Code: DOWorld::ReadIDO2008(const char*)" << std::endl
			<< "        Note: IDOWorld read idofile error" << std::endl;
		return false;
	}

	pSystemParameter = new SystemParameter(idof, 2008);

	idof.read( (char*) &lnum_doml  , sizeof(unsigned long) );
	idof.read( (char*) &lnum_iactml, sizeof(unsigned long) );
	idof.read( (char*) &lnum_dos   , sizeof(unsigned long) );

	for (i=0; i<lnum_doml; ++i)
		cDOModel.push_back(new DOModel(idof));

	for (i=0; i<lnum_iactml; ++i)
		cIactModel.push_back(new IactModel(idof));

	for (i=0; i<lnum_dos; ++i)
		cDOStatus.push_back(new DOStatus(idof));

	idof.close();

	return DOWorld::Check();
};

void DOWorld::WriteIDO(const char* filename) const
{
	std::ofstream idof;
	unsigned long lnum_doml   = (unsigned long) cDOModel.size();
	unsigned long lnum_iactml = (unsigned long) cIactModel.size();
	unsigned long lnum_dos    = (unsigned long) cDOStatus.size();

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

	idof.write((char*) &lnum_doml  , sizeof(unsigned long));
	idof.write((char*) &lnum_iactml, sizeof(unsigned long));
	idof.write((char*) &lnum_dos   , sizeof(unsigned long));

	for(idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
		**idoml >> idof;

	for(iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
		**iactml >> idof;

	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
		**idos >> idof;

    unsigned long lnum_is = 0;
	idof.write((char*) &lnum_is, sizeof(unsigned long));

	idof.close();
};

void DOWorld::WriteIDO(const char* filename, const IactRecordTab* irtp) const
{
	std::ofstream idof;
	unsigned long lnum_doml   = (unsigned long) cDOModel.size();
	unsigned long lnum_iactml = (unsigned long) cIactModel.size();
	unsigned long lnum_dos    = (unsigned long) cDOStatus.size();

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

	idof.write((char*) &lnum_doml  , sizeof(unsigned long));
	idof.write((char*) &lnum_iactml, sizeof(unsigned long));
	idof.write((char*) &lnum_dos   , sizeof(unsigned long));

	for(idoml=cDOModel.begin(); idoml!=cDOModel.end(); ++idoml)
		**idoml >> idof;

	for(iactml=cIactModel.begin(); iactml!=cIactModel.end(); ++iactml)
		**iactml >> idof;

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
