#ifndef _DOWORLD_WRITE_VTK__H
#define _DOWORLD_WRITE_VTK__H

#include <Framework/Interfaces/DOWorld.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>

namespace vedo
{

struct NullExternalFieldVTKWriter
{	static void writeExternalFields(std::FILE *fpvpf) {} };

class DataFieldVTKWriter
{
public:
	const int getNumOfArrays() { return numOfComp.size(); }
	const int getNumOfComponentsOfArray(const int arrayID) { return numOfComp[arrayID]; }
	std::vector<double>& getArray(const int arrayID) { return fieldArr[arrayID]; }
	const std::string getArrayName(const int arrayID) { return arrName[arrayID]; }

	void addArray(const std::string& name, const int numOfComponent, std::vector<double>& data) {
		numOfComp.push_back(numOfComponent);
		fieldArr.push_back(data);
		arrName.push_back(name);
	}

	void clearAll() { numOfComp.clear(); fieldArr.clear(); arrName.clear(); }

	static DataFieldVTKWriter* Instance() {
		if( !_instance ) _instance = new DataFieldVTKWriter;
		return _instance;
	}

	inline static void writeVTKDataArray(std::FILE *fpvpf, const std::string& name, std::vector<double>& data, const int numOfComponent)
	{
		if( data.size() == 0 ) return;
		std::string s;
		std::ostringstream oss(s);
		if( numOfComponent == 1)
		{
			oss
				<< "        <DataArray type=\"Float64\" Name=\""
				<< name
				<< "\" format=\"ascii\">"
				<< std::endl;
		}
		else
		{
			oss
				<< "        <DataArray type=\"Float64\" Name=\""
				<< name
				<< "\" NumberOfComponents=\""
				<< numOfComponent
				<< "\" format=\"ascii\">"
				<< std::endl;
		}
		fprintf(fpvpf, oss.str().c_str());
		for(std::size_t i=0; i < data.size(); i++)
			fprintf(fpvpf,"%g ", data[i]);
		fprintf(fpvpf,"\n");
		fprintf(fpvpf,"        </DataArray>\n");
	}

	inline static void writeExternalFields(std::FILE *fpvpf)
	{
		DataFieldVTKWriter* wrapArr = DataFieldVTKWriter::Instance();
		for(int i = 0; i < wrapArr->getNumOfArrays(); i++)
		writeVTKDataArray(fpvpf, wrapArr->getArrayName(i), wrapArr->getArray(i), wrapArr->getNumOfComponentsOfArray(i));
	}

private:
	std::vector<int> numOfComp;
	std::vector< std::vector<double> > fieldArr;
	std::vector< std::string > arrName;
	static DataFieldVTKWriter* _instance;


};

/*
template<typename ExtFieldWriterT>
void DOWorld::WriteVTK (const char* filename) const
{
	std::FILE *fpvpf;
	if ((fpvpf = std::fopen(filename, "w"))== NULL )
	{
		std::cout << "Error!! Code: DOWorld::WriteVTK (const char*)" << std::endl;
		exit(-1);
	};

	/// raw data arrays
	std::vector<double> sphere_Positions, sphere_Velocitys, sphere_AngularVelocity;
	std::vector<double> sphere_Radius, sphere_GranularTempV, sphere_GranularTempAV;

	/// read data to arrays
	std::vector<DOStatus *>::const_iterator idos;
	for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
	{
		double Radius;
		switch(GetDOModel((*idos)->GetDOName())->GetShapeType())
		{
			case Sphere:
				Radius
					= DOWorld::GetDOModel
						((*idos)->GetDOName())
							->GetShapeAttributes().sphere.radius;
				sphere_Radius.push_back(Radius);
				sphere_Positions.push_back((*idos)->GetPosition().x());
				sphere_Positions.push_back((*idos)->GetPosition().y());
				sphere_Positions.push_back((*idos)->GetPosition().z());
				sphere_Velocitys.push_back((*idos)->GetVelocity().x());
				sphere_Velocitys.push_back((*idos)->GetVelocity().y());
				sphere_Velocitys.push_back((*idos)->GetVelocity().z());
				sphere_AngularVelocity.push_back((*idos)->GetAngularVelocity().x());
				sphere_AngularVelocity.push_back((*idos)->GetAngularVelocity().y());
				sphere_AngularVelocity.push_back((*idos)->GetAngularVelocity().z());
				sphere_GranularTempV.push_back((*idos)->GetGranularTemperatureV());
				sphere_GranularTempAV.push_back((*idos)->GetGranularTemperatureAV());
				break;
			case Polyhedra:
				// TODO:
				break;
			case QuasiPlate:
				// TODO:
				break;
			case QuasiCylinder:
				// TODO:
				break;
			default:
				std::cerr
					<< "Error!! Code: DOWorld_WriteVTK.h" << std::endl
					<< "        Note: Unknown VTK shape" << std::endl;
				std::exit(-1);
		}
	}

	// write VTK file
	fprintf(fpvpf,"<?xml version=\"1.0\"?>\n");
	fprintf(fpvpf,"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n");
	fprintf(fpvpf,"  <UnstructuredGrid>\n");
	fprintf(fpvpf,"    <Piece NumberOfPoints=\"%d\" NumberOfCells=\"0\">\n", sphere_Radius.size());
	fprintf(fpvpf,"      <PointData Scalars=\"Radius\">\n");
	DataFieldVTKWriter::writeVTKDataArray(fpvpf, "Position", sphere_Positions, 3);
	DataFieldVTKWriter::writeVTKDataArray(fpvpf, "Velocity", sphere_Velocitys, 3);
	DataFieldVTKWriter::writeVTKDataArray(fpvpf, "AngularVelocity", sphere_AngularVelocity, 3);
	DataFieldVTKWriter::writeVTKDataArray(fpvpf, "Radius", sphere_Radius, 1);
	DataFieldVTKWriter::writeVTKDataArray(fpvpf, "GranularTemperatureV", sphere_GranularTempV, 1);
	DataFieldVTKWriter::writeVTKDataArray(fpvpf, "GranularTemperatureAV", sphere_GranularTempAV, 1);
	ExtFieldWriterT::writeExternalFields(fpvpf);

	fprintf(fpvpf,"      </PointData>\n");
 	fprintf(fpvpf,"      <Points>\n");
	fprintf(fpvpf,"        <DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n");
	for(int i=0; i<sphere_Positions.size(); i++)
		fprintf(fpvpf,"%g ", sphere_Positions[i]);
	fprintf(fpvpf,"\n");
	fprintf(fpvpf,"        </DataArray>\n");
	fprintf(fpvpf,"      </Points>\n");
    fprintf(fpvpf,"      <Cells>\n");
	fprintf(fpvpf,"        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n");
	fprintf(fpvpf,"        </DataArray>\n");
	fprintf(fpvpf,"      </Cells>\n");
	fprintf(fpvpf,"    </Piece>\n");
	fprintf(fpvpf,"  </UnstructuredGrid>\n");
	fprintf(fpvpf,"</VTKFile>\n");
	fclose (fpvpf);
};
*/

};   // namespace vedo

#endif // _DOWORLD_WRITE_VTK__H
