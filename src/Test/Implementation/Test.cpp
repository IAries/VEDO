#include <NJR/Interfaces/Constants.h>
#include <NJR/Interfaces/ParameterSet.h>
#include <NJR/Interfaces/EFSTimeHistory.h>
#include <NJR/Interfaces/EFSExtrapolation.h>
#include <NJR/Interfaces/EFPathDependent.h>
#include <NJR/Interfaces/Vector3d.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

int main (int argc, char* argv[])
{
	njr::EFSTimeHistory<double> t("TimeHistory.txt");
	std::cout << t;

	std::ofstream ofOutputFile("output.txt", std::ios::out);

	ofOutputFile << "Time, Value" << std::endl;

	for (double dTime=-2.0; dTime<=12.0; dTime+=0.01)
        ofOutputFile << dTime << ", " << t(dTime) << std::endl;

	ofOutputFile.close();

	system("pause");
	return true;
};
