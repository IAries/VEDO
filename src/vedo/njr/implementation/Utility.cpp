#include <vedo/constants/interfaces/Constants.h>
#include <vedo/njr/interfaces/Utility.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <time.h>
//#include <ctime>
#include <iostream>
#include <iterator>

namespace njr
{

void ReadString(std::string& os, std::ifstream &idof)
{
	vedo::vedo_uint_t sLength;
	idof.read ((char*) &sLength, sizeof(vedo::vedo_uint_t));
	char* cpBuffer = new char[sLength];
	idof.read(cpBuffer, sLength);
	os.assign(cpBuffer, 0, sLength);
	delete cpBuffer;
}

/*
void ReadString(std::string& os, std::ifstream &idof)
{
	vedo_uint_t ssize;
	char buffer [256];
	idof.read ((char *)&ssize,	sizeof(vedo::vedo_uint_t));
	idof.read ((char *)buffer, ssize);
	os.assign(buffer, 0, ssize);
	buffer[(ssize>255) ? 255 : ssize]='\0';
	os = buffer;
}
*/

void WriteString(const std::string& ostring, std::ofstream &idof)
{
	vedo::vedo_uint_t sLength = (vedo::vedo_uint_t) ostring.length();
	idof.write ((char*) &sLength, sizeof(vedo::vedo_uint_t));
	idof.write (ostring.c_str(), sLength);
}

/*
void WriteString(const std::string& ostring, std::ofstream &idof)
{
	vedo_uint_t ssize = (vedo_uint_t) ostring.length();
	idof.write ((char*)& ssize,	sizeof(vedo::vedo_uint_t));
	idof.write (ostring.c_str(), ssize);
}
*/

bool CheckSubName (std::string FileName, std::string SubName)
{
	return (FileName.rfind(SubName) == (FileName.size() - SubName.size()));
}

std::string RunTime(const std::string& message)
{
	static vedo::vedo_uint_t iteration = 0;
	static time_t start = time(0);
	static time_t last  = time(0);

	time_t now = time(0);

	std::string Mon[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if (iteration == 0)
	{
		tm* t = localtime(&start);
		std::cout
			<< std::endl
			<< "Time Counter Initiation {" << message << "}" << std::endl
			<< "Now !!! (" << 1900 + t->tm_year << "-" << Mon[t->tm_mon] << "-" << t->tm_mday
			<< ") (Hour:" << t->tm_hour << ",Min:" << t->tm_min << ')'
			<< std::endl << std::endl << std::endl;
	}
	else
	{
		tm* t = localtime(&now);
		std::cout
			<< "{" << message << "}"
			<< std::endl
			<< "Now !!! ("
			<< 1900 + t->tm_year << "-" << Mon[t->tm_mon] << "-" << t->tm_mday << " " << t->tm_hour << ":" << t->tm_min << ')'
			<< "(RunTime: " << (vedo::vedo_float_t)(difftime(now, start)) << " s) "
			<< "(Since Last: " << (vedo::vedo_float_t)(difftime(now, last)) << " s)"
			<< std::endl;
	}

	iteration ++;
	last = now;
	return message;
}


void WriteFile(const std::string& FileName, const std::vector<char>& vWord)
{
	std::ofstream of(FileName.c_str(), std::ios::out | std::ios::binary);

	if (of.bad())
	{
		std::cout
			<< "Error!! Code: WriteFile(const std::string&, const std::vector<char>&)" << std::endl
			<< "        Note: Cannot open file \'" << FileName << '\'' << std::endl;
		exit(-1);
	}

	copy(vWord.begin(), vWord.end(), std::ostream_iterator<char>(of));
	of.close();
}

std::vector<char> ReadFile (const std::string& FileName)
{
	char c;
	std::vector<char> vcword;
	std::ifstream f(FileName.c_str(), std::ios::in | std::ios::binary);

	if ( f.bad() )
	{
		std::cout
			<< "Error!! Code: ReadFile(const std::string&)" << std::endl
			<< "        Note: Cannot open file \'" << FileName << '\'' << std::endl;
		exit(-1);
	}

	while ( !f.eof() )
	{
		f.read (&c, sizeof(char));
		vcword.push_back(c);
	}
	vcword.pop_back();

	f.close();
	return vcword;
}

void dos2unix(const std::string& ifile, const std::string& ofile)
{
	std::vector<char> vcword = ReadFile(ifile);
	vcword.erase(remove(vcword.begin(), vcword.end(), 13), vcword.end());
	WriteFile(ofile, vcword);
}

void unix2dos(const std::string& ifile, const std::string& ofile)
{
	std::vector<char> vcword = ReadFile(ifile);
	std::vector<char>::iterator ic;
	std::vector<char> hexdata;

	for (ic=vcword.begin(); ic!=vcword.end(); ic++)
	{
		if ((*ic) == 10)
		{
			hexdata.push_back (13);
		}
		hexdata.push_back (*ic);
	}
	WriteFile(ofile, hexdata);
}

void bin2hex(const std::string& ifile, const std::string& ofile)
{
	std::vector<char> vcword = ReadFile(ifile);
	std::vector<char>::iterator ic;
	std::vector<char> hexdata;
	vedo::vedo_uint_t i = 1;

	for (ic=vcword.begin(); ic!=vcword.end(); ic++)
	{
		vedo::vedo_uint_t first  = *ic/16;
		vedo::vedo_uint_t second = *ic%16;
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
	WriteFile(ofile, hexdata);
}

vedo::vedo_float_t NewtonApproach
	(vedo::vedo_float_t (*F)(vedo::vedo_float_t),
	 vedo::vedo_float_t (*DF)(vedo::vedo_float_t),
	 const vedo::vedo_float_t& dV,
	 const vedo::vedo_float_t& dIG                )
{
	vedo::vedo_float_t x   = dIG;
	vedo::vedo_float_t xs;
	vedo::vedo_uint_t  uic = 0;

oncemore:

	xs = x - (F(x)-dV) / DF(x);

	if ((fabs((F(xs)-dV)/dV)) < 1e-6)
	{
		return xs;
	}

	if ((uic++) > 50)
	{
		return xs;
		std::cerr
			<< "Error!! Code: Utility.cpp" << std::endl
			<< "        Note: Newton Approach can't obtain the answer" << std::endl;
		exit(-1);
	}

	x = xs;
	goto oncemore;
}

}   // namespace njr
