// Colletion

#ifndef _NJR_UTILITY_H
#define _NJR_UTILITY_H

#include <njr/DataType.h>

#include <algorithm>
#include <list>
#include <fstream>
#include <string>
#include <vector>

namespace njr
{

void dos2unix(const std::string& ifile, const std::string& ofile);

void unix2dos(const std::string& ifile, const std::string& ofile);

void bin2hex(const std::string& ifile, const std::string& ofile);

void WriteFile(const std::string& FileName, const std::vector<char>& ContentContainer);

std::vector<char> ReadFile(const std::string& FileName);

std::string RunTime(const std::string& message = "RunTime");



// Function object for deleting object by its point
struct Delete_ptr
{
	template<class T> T* operator() (T* p)
	{
		delete p;
		return 0;
	}
};



struct Copy_obj
{
	template<class T> T* operator() (const T* p)
	{
		return new T(*p);
	}
};



_float_t NewtonApproach
	(_float_t (*Function)(_float_t),
	 _float_t (*DifferentialFunction)(_float_t),
	 const _float_t& dValue = 0.0,
	 const _float_t& dInitialGuess = 0.0                  );

}   // namespace njr

#endif // _NJR_UTILITY_H
