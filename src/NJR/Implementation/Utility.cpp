#include <NJR/Interfaces/Utility.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <time.h>
//#include <ctime>
#include <iostream>
#include <iterator>

using namespace std;

void NJR::ReadString(string& os, ifstream &idof)
{
	unsigned int ssize;
	char buffer [256];
	idof.read ((char *)&ssize ,	sizeof(unsigned int));
	idof.read ((char *)buffer , ssize);
	buffer[(ssize>255) ? 255 : ssize]='\0';
	os = buffer;
};

void NJR::WriteString(const string& ostring, ofstream &idof)
{
	unsigned int ssize;
	ssize =(unsigned int) ostring.length();
	idof.write ((char*)& ssize,	sizeof(unsigned int));
	idof.write (ostring.c_str(), ssize);
};

bool NJR::CheckSubName (string FileName, string SubName)
{
	return (FileName.rfind(SubName)
		 == (FileName.size() - SubName.size()) );
};

string NJR::RunTime(const string& message)
{
	static unsigned long iteration = 0;
	static time_t start = time(0); 
	static time_t last  = time(0);

	time_t now = time(0);

	char *Mon[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
				   "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

	if (iteration == 0)
	{
		tm* t = localtime(&start);
		cout
			<< endl
			<< "Time Counter Initiation {" << message << "}"
			<< endl
			<< "Now !!! ("
			<< 1900 + t->tm_year << "-" << Mon[t->tm_mon] << "-" << t->tm_mday
			<< ") (Hour:" << t->tm_hour << ",Min:" << t->tm_min << ')'
			<< endl
			<< endl
			<< endl;
	}
	else
	{
		tm* t = localtime(&now);
		cout
			<< "{" << message << "}"
			<< endl
			<< "Now !!! ("
			<< 1900 + t->tm_year
			<< "-" << Mon[t->tm_mon]
			<< "-" << t->tm_mday
			<< " " << t->tm_hour
			<< ":" << t->tm_min << ')'
			<< "(RunTime: " << (double)(difftime(now, start)) << " s) "
			<< "(Since Last: " << (double)(difftime(now, last)) << " s)"
			<< endl;
	}

	iteration ++;
	last = now;
	return message;
};


void NJR::WriteFile(const string& FileName, const vector<char>& vWord)
{
	ofstream of(FileName.c_str(), ios::out | ios::binary);

	if (of.bad())
	{
		cerr << "open file (" << FileName << ") error" << endl;
		exit(0);
	}

	copy(vWord.begin(), vWord.end(), ostream_iterator<char>(of));
	of.close();
};

vector<char> NJR::ReadFile (const string& FileName)
{
	char c;
	vector<char> vcword;
	ifstream f(FileName.c_str(), ios::in | ios::binary);

	if ( f.bad() )
	{
		cerr << "open file (" << FileName << ") error" << endl;
		exit (0);
	}

	while ( !f.eof() )
	{
		f.read (&c, sizeof(char));
		vcword.push_back(c);
	}
	vcword.pop_back();

	f.close();
	return vcword;
};

void NJR::dos2unix(const string& ifile, const string& ofile)
{
	vector<char> vcword = NJR::ReadFile(ifile);
	vcword.erase(remove(vcword.begin(), vcword.end(), 13), vcword.end());
	NJR::WriteFile(ofile, vcword);
};

void NJR::unix2dos(const string& ifile, const string& ofile)
{
	vector<char> vcword = NJR::ReadFile(ifile);
	vector<char>::iterator ic;
	vector<char> hexdata;

	for (ic=vcword.begin(); ic!=vcword.end(); ic++)
	{
		if ((*ic) == 10)
		{
			hexdata.push_back (13);
		}
		hexdata.push_back (*ic);
	}
	NJR::WriteFile(ofile, hexdata);
};

void NJR::bin2hex(const string& ifile, const string& ofile)
{
	vector<char> vcword = NJR::ReadFile(ifile);
	vector<char>::iterator ic;
	vector<char> hexdata;
	unsigned int i = 1;

	for (ic=vcword.begin(); ic!=vcword.end(); ic++)
	{
		unsigned int first  = *ic/16;
		unsigned int second = *ic%16;
		hexdata.push_back((first < 10)? first + 48: first + 55);
		hexdata.push_back((second < 10)? second + 48: second + 55);
		hexdata.push_back(':');
		hexdata.push_back(( ( ((*ic)>32) && ((*ic)<=126)) )? *ic : ' ');
		hexdata.push_back(' ');

		if (i++ %15 == 0)
		{
			hexdata.push_back(13);
			hexdata.push_back(10);
		}
	}
	NJR::WriteFile(ofile, hexdata);
};

double NJR::NewtonApproach
	(double (*F)(double),
	double (*DF)(double),
	const double& dV,
	const double& dIG)
{
	double x = dIG;
	double xs;
	unsigned int uic = 0;

oncemore:

	xs = x - (F(x)-dV) / DF(x);

	if ( (fabs((F(xs)-dV)/dV)) < 1e-6 )
	{
		return xs;
	}

	if ( (uic++) > 50)
	{
		return xs;
		cerr << "Warning !! Newton Approach can't obtain the answer" << endl;
	}

	x = xs;
	goto oncemore;
};
