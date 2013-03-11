#include <FrameWork/Interfaces/DOWorld.h>

using namespace std;

bool DOWorld::ReadIDO(const char* filename)
{
	DOWorld::Clear();

	ifstream idof;
	unsigned long lnum_doml;
	unsigned long lnum_iactml;
	unsigned long lnum_dos;

	register unsigned long i;

	idof.open(filename, ios::in | ios::binary);

	if (!idof.is_open())
	{
		cerr << "IDOWorld read idofile error" << endl;
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

	ifstream idof;
	unsigned long lnum_doml;
	unsigned long lnum_iactml;
	unsigned long lnum_dos;

	register unsigned long i;

	idof.open(filename, ios::in | ios::binary);

	if (!idof.is_open())
	{
		cerr << "IDOWorld read idofile error" << endl;
		return false;
	}

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

	ifstream idof;
	unsigned long lnum_doml;
	unsigned long lnum_iactml;
	unsigned long lnum_dos;

	register unsigned long i;

	idof.open(filename, ios::in | ios::binary);

	if (!idof.is_open())
	{
		cerr << "IDOWorld read idofile error" << endl;
		return false;
	}

	pSystemParameter = new SystemParameter(idof);

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

	ifstream idof;
	unsigned long lnum_doml;
	unsigned long lnum_iactml;
	unsigned long lnum_dos;

	register unsigned long i;

	idof.open(filename, ios::in | ios::binary);

	if (!idof.is_open())
	{
		cerr << "IDOWorld read idofile error" << endl;
		return false;
	}

	pSystemParameter = new SystemParameter(idof);

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

	ifstream idof;
	unsigned long lnum_doml;
	unsigned long lnum_iactml;
	unsigned long lnum_dos;

	register unsigned long i;

	idof.open(filename, ios::in | ios::binary);

	if (!idof.is_open())
	{
		cerr << "IDOWorld read idofile error" << endl;
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
	ofstream idof;
	unsigned long lnum_doml   = (unsigned long) cDOModel.size();
	unsigned long lnum_iactml = (unsigned long) cIactModel.size();
	unsigned long lnum_dos    = (unsigned long) cDOStatus.size();

	list<DOModel *>::const_iterator idoml;
	list<IactModel *>::const_iterator iactml;
	vector<DOStatus *>::const_iterator idos;

	idof.open(filename, ios::out | ios::binary);

	if (idof.bad())
	{
		cerr << "DOWorld write idofile error!" << endl;
		exit(0);
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

	idof.close();
};
