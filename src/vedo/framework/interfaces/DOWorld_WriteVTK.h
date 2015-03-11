#ifndef _DOWORLD_WRITE_VTK__H
#define _DOWORLD_WRITE_VTK__H

#include <vedo/framework/interfaces/DOWorld.h>

#include <cstdio>
#include <cstdlib>
#include <sstream>

namespace vedo
{

struct NullExternalFieldVTKWriter
{
	static void writeExternalFields(std::ofstream* ofpVTK)
	{
	}
};



class DataFieldVTKWriter
{

public:

	const vedo_int_t getNumOfArrays()
	{
		return numOfComp.size();
	}

	const vedo_int_t getNumOfComponentsOfArray(const vedo_int_t arrayID)
	{
		return numOfComp[arrayID];
	}

	std::vector<vedo_float_t>& getArray(const vedo_int_t arrayID)
	{
		return fieldArr[arrayID];
	}

	const std::string getArrayName(const vedo_int_t arrayID)
	{
		return arrName[arrayID];
	}

	void addArray(const std::string& name, const vedo_int_t numOfComponent, std::vector<vedo_float_t>& data)
	{
		numOfComp.push_back(numOfComponent);
		fieldArr.push_back(data);
		arrName.push_back(name);
	}

	void clearAll()
	{
		numOfComp.clear();
		fieldArr.clear();
		arrName.clear();
	}

	static DataFieldVTKWriter* Instance()
	{
		if (_instance == 0)
		{
			_instance = new DataFieldVTKWriter;
		}
		return _instance;
	}

	inline static void writeVTKDataArray
		(std::ofstream* ofpVTK, const std::string& name, std::vector<vedo_float_t>& data, const vedo_int_t numOfComponent)
	{
		if (data.size() == 0 )
		{
			return;
		}
		std::string s;

		if (numOfComponent == 1)
		{
			*ofpVTK
				<< "\t\t\t\t<DataArray type=\"Float64\" Name=\"" << name << "\" format=\"ascii\">" << std::endl;
		}
		else
		{
			*ofpVTK
				<< "\t\t\t\t<DataArray type=\"Float64\" Name=\"" << name
				<< "\" NumberOfComponents=\"" << numOfComponent << "\" format=\"ascii\">"          << std::endl;
		}
		for (vedo_uint_t i=0; i<(vedo_uint_t)data.size(); i++)
		{
			*ofpVTK
				<< data[i] << " "                                                                  << std::endl;
		}
			*ofpVTK
				<< std::endl
				<< "\t\t\t\t</DataArray>"                                                          << std::endl;
	}

	inline static void writeExternalFields(std::ofstream* ofpVTK)
	{
		DataFieldVTKWriter* wrapArr = DataFieldVTKWriter::Instance();
		for (vedo_int_t i=0; i<(vedo_int_t)wrapArr->getNumOfArrays(); i++)
		{
			writeVTKDataArray(ofpVTK, wrapArr->getArrayName(i), wrapArr->getArray(i), wrapArr->getNumOfComponentsOfArray(i));
		}
	}

private:

	std::vector<vedo_int_t> numOfComp;

	std::vector<std::vector<vedo_float_t> > fieldArr;

	std::vector<std::string> arrName;

	static DataFieldVTKWriter* _instance;

};



#ifdef _STD_CPP_11
	template<typename ExtFieldWriterT>
	void DOWorld::WriteVTK(const std::string filename) const
	{
		std::ofstream ofVTK(filename, std::ios::out);
		if (!ofVTK)
		{
			std::cerr
				<< "Error!! Code: DOWorld::WriteVTK(const std::string, const DOWorld*)" << std::endl
				<< "        Note: DOWorld cannot write vtk file"                        << std::endl;
			exit(-1);
		}

		/// raw data arrays
		std::vector<vedo_float_t> sphere_Positions, sphere_Velocitys, sphere_AngularVelocity;
		std::vector<vedo_float_t> sphere_Radius, sphere_GranularTempV, sphere_GranularTempAV;

		/// read data to arrays
		std::vector<DOStatus *>::const_iterator idos;
		for (idos=cDOStatus.begin(); idos!=cDOStatus.end(); ++idos)
		{
			vedo_float_t Radius;
			switch (GetDOModel((*idos)->GetDOName())->GetShapeType())
			{
				case Sphere:
					Radius = DOWorld::GetDOModel((*idos)->GetDOName())->GetShapeAttributes().sphere.radius;
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
				case QuasiPlateWithCircularHole:
					// TODO:
					break;
				case QuasiCylinder:
					// TODO:
					break;
				default:
					std::cerr
						<< "Error!! Code: DOWorld_WriteVTK.h" << std::endl
						<< "		Note: Unknown VTK shape" << std::endl;
					std::exit(-1);
			}
		}

		// write VTK file
		ofVTK
			<< "<?xml version=\"1.0\"?>"                                                           << std::endl
			<< "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">"   << std::endl
			<< "\t<UnstructuredGrid>"                                                              << std::endl
			<< "\t\t<Piece NumberOfPoints=\"" << sphere_Radius.size() << "\" NumberOfCells=\"0\">" << std::endl
			<< "\t\t\t<PointData Scalars=\"Radius\">"                                              << std::endl;
		DataFieldVTKWriter::writeVTKDataArray(&ofVTK, "Position"             , sphere_Positions      , 3);
		DataFieldVTKWriter::writeVTKDataArray(&ofVTK, "Velocity"             , sphere_Velocitys      , 3);
		DataFieldVTKWriter::writeVTKDataArray(&ofVTK, "AngularVelocity"      , sphere_AngularVelocity, 3);
		DataFieldVTKWriter::writeVTKDataArray(&ofVTK, "Radius"               , sphere_Radius         , 1);
		DataFieldVTKWriter::writeVTKDataArray(&ofVTK, "GranularTemperatureV" , sphere_GranularTempV  , 1);
		DataFieldVTKWriter::writeVTKDataArray(&ofVTK, "GranularTemperatureAV", sphere_GranularTempAV , 1);
		ExtFieldWriterT::writeExternalFields(&ofVTK);

		ofVTK
			<< "\t\t\t</PointData>"                                                                << std::endl
			<< "\t\t\t<Points>"                                                                    << std::endl
			<< "\t\t\t\t<DataArray type=\"Float64\" NumberOfComponents=\"3\" format=\"ascii\">"    << std::endl;
		for (vedo_uint_t u=0; u<(vedo_uint_t)sphere_Positions.size(); u++)
		{
			ofVTK << sphere_Positions[u] << " ";
		}
		ofVTK
			<< std::endl
			<< "\t\t\t\t</DataArray>"                                                              << std::endl
			<< "\t\t\t</Points>"                                                                   << std::endl
			<< "\t\t\t<Cells>"                                                                     << std::endl
			<< "\t\t\t\t<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">"                << std::endl
			<< "\t\t\t\t</DataArray>"                                                              << std::endl
			<< "\t\t\t</Cells>"                                                                    << std::endl
			<< "\t\t</Piece>"                                                                      << std::endl
			<< "\t</UnstructuredGrid>"                                                             << std::endl
			<< "</VTKFile>"                                                                        << std::endl;

		ofVTK.close();
	}
#endif   // _STD_CPP_11

}   // namespace vedo

#endif // _DOWORLD_WRITE_VTK__H
