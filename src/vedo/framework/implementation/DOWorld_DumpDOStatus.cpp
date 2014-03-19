#include <vedo/framework/interfaces/DOWorld.h>
#include <fstream>

namespace vedo
{

void DOWorld::DumpDOStatus(const char* filename) const
{
    std::ofstream oCSVFile(filename, std::ios::out);

    oCSVFile
		<< "DOStatusSN, DOStatusID, DOName, "
		<< "Position-X, PositionY, PositionZ, "
		<< "Velocity-X, VelocityY, VelocityZ, "
		<< "AngularVelocity-X, AngularVelocity-Y, AngularVelocity-Z, "
		<< "Orientation-Xx, Orientation-Xy, Orientation-Xz, "
		<< "Orientation-Zx, Orientation-Zy, Orientation-Zz, "
		<< "Impact-X, Impact-Y, Impact-Z, "
		<< "AngularImpact-X, AngularImpact-Y, AngularImpact-Z"
		<< std::endl;

    const DOStatus* dosp;
    njr::Vector3d vV;

    for(unsigned long ul=0; ul<cDOStatus.size(); ul++)
	{
	    dosp = cDOStatus[ul];
	    oCSVFile
			<< ul                << ", "
			<< dosp->GetID()     << ", "
			<< dosp->GetDOName() << ", ";

	    vV = dosp->GetPosition();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

	    vV = dosp->GetVelocity();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

	    vV = dosp->GetAngularVelocity();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

	    vV = dosp->GetOrientationX();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

	    vV = dosp->GetOrientationZ();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

	    vV = dosp->GetImpact();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << ", ";

	    vV = dosp->GetAngularImpact();
        oCSVFile << vV.x() << ", " << vV.y() << ", " << vV.z() << std::endl;
	}

    oCSVFile.close();
};

};   // namespace vedo
