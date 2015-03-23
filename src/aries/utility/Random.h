#ifndef _ARIES_RANDOM_H
#define _ARIES_RANDOM_H

#include <aries/utility/DataType.h>
#include <cstdlib>
#include <ctime>
#include <random>

namespace aries
{

template <typename T>
std::vector<T> GereratingRandomNumber(const T& tLowerBound, const T& tUpperBound, const _uint_t uNum)
{
	std::srand(std::time(0));
	T tRange = tUpperBound - tLowerBound;
	std::vector<T> tvrandom;
	for(_uint_t u=0; u<uNum; u++)
	{
		tvrandom.push_back(tLowerBound + (T)(std::rand()) / (T)(RAND_MAX) * tRange);
	}
	return tvrandom;
}

}   // namespace aries

#endif // _ARIES_RANDOM_H
